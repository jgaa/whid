#include "libwhid.h"
#include "server/NodeI.h"   // class implemented

#include "server/CustomerI.h"
#include "server/ProjectI.h"
#include "server/TaskI.h"
#include "server/RateI.h"
#include "server/WorkI.h"

#include "server/WhidEngine.h"
#include <database/WarDbConnection.h>
#include <ice/WarIceCore.h>
#include <ice/WarIceException.h>

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::NodePrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const NodeI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

NodeI::NodeI()
{
}// NodeI

NodeI::~NodeI()
{
}// ~NodeI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void NodeI::Load () throw (WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t row;

    query = "SELECT N.Name, N.Notes, N.ActiveStatus ";

    OnLoadAddToSelect(query);

    query += " FROM ", GetTableName(), " AS N ";

    OnLoadAddJoins(query);

    query += " WHERE N.Id = ", GetNativeKey();

    OnLoadAddToWhere(query);

    query.Exec(row);
    if (row->IsEof())
        WAR_THROW_NOT_FOUND(NULL);
    
    row->Get(C_NAME, mData.mName);
    row->Get(C_NOTES, mData.mNotes);
    mData.mActiveStatus = (WHID::ActiveStatusE)row->GetInt32(C_ACTIVESTATUS);

    OnLoadFetch(row);

    mDirty = 0;
}

void NodeI::Save () throw (WarException)
{
    if (mDirty)
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        query = "UPDATE ", GetTableName(), " SET "
            " Name = ", mData.mName,
            ", Notes = ", mData.mNotes,
            ", ActiveStatus = ", mData.mActiveStatus;

        OnLoadAddToSet(query);

        query += " WHERE Id = ", GetNativeKey();

        query.Exec();
        mDirty = 0;
    }
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::std::string
NodeI::GetName(const Ice::Current& current) const
{
    VALIDATE_SESSION;

    std::string name;
    {
        WAR_AUTO_LOCK;
        name = mData.mName;
    }

    return name;
}

void
NodeI::GetData(::WHID::NodeData& data,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;

    WAR_AUTO_LOCK;
    data = mData;
}

void
NodeI::SetData(const ::WHID::NodeData& data,
		     const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;
    bool is_rename = false;

    {
        WAR_AUTO_LOCK;
        is_rename = war_strcasecmp(data.mName.c_str(), mData.mName.c_str()) != 0;
    }

    if (is_rename)
    {
        // Check for duplicate name
        try
        {
            WarDbConnection db;
            WhidEngine::GetEngine().GetDbPool().GetConnection(db);
            WarDbQuery query(db);
            war_db_resultset_ptr_t row;

            war_int64_t parent = GetParentId();

            query = "SELECT COUNT(*) FROM ",
                GetTableName(),
                " WHERE Name = ", data.mName;

            if (parent)
                query += " AND ", GetParentTableName(), " = ", parent;

            query.Exec(row);
            if (row->GetInt32(0) > 0)
            {
                WAR_NOTICELOG;
                notice_log << "A " << GetTableName() << " \""
                    << data.mName
                    << "\" already exist. Cannot rename."
                    << war_endl;

                throw WHID::AlreadyExistException();
            }

        }
        catch(WarException& ex)
        {
            WAR_ERRLOG;
            err_log << *this << " caught an exception: "
                << ex.what()
                << war_endl;

            throw WHID::InternalErrorException();
        }
    }

    if (mData.mActiveStatus != data.mActiveStatus)
        UpdateActiveStatus(current);


    {
        WAR_AUTO_LOCK;
        mData = data;
        Flush(-1);
    }
}


::std::string
NodeI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;

    {
        WAR_AUTO_LOCK;
        msg << GetClassName() << " \"" << mData.mName << "\"";
    }

    return msg.str();
}

void NodeI::GetChildren(bool onlyActive,
                        ::WHID::NodeInfoList& children,
			 const Ice::Current& current) const
{
    VALIDATE_SESSION;
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        enum RowsE {C_ID, C_NAME, C_ACTIVE };
        query = "SELECT Id, Name, ActiveStatus "
            " FROM ", GetChildTableName();

        GetChildrenClause(query);

        if (onlyActive)
        {
            if (GetNodeType() == WHID::NODECUSTOMER)
            {
                query += " AND IsActive = 1 ";
            }
            else
            {
                // The children (tasks) don't have the IsActve field
                war_int32_t active = (war_int32_t)WHID::ASACTIVE;
                query += " AND ActiveStatus = ", active;
            }
        }

        query += " ORDER BY Name ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::NodeInfo n;
            war_int64_t node_id = rows->GetInt64(C_ID);
            rows->Get(C_NAME, n.mName);
            n.mActiveStatus = (WHID::ActiveStatusE)rows->GetInt32(C_ACTIVE);
            
            switch(GetNodeType())
            {
            case WHID::NODECUSTOMER:
                n.mNode = ProjectI::GetProxy(node_id);
                n.mType = WHID::NODEPROJECT;
                break;
            case WHID::NODEPROJECT:
                n.mNode = TaskI::GetProxy(node_id);
                n.mType = WHID::NODETASK;
                break;
            default:
                continue; // Tasks does not have chilren..
            }

            children.push_back(n);
        }
    }
    catch(WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

::WHID::NodeTypeE
NodeI::GetNodeType(const Ice::Current& current)
{
    VALIDATE_SESSION;
    return GetNodeType();
}

bool
NodeI::IsActive(const Ice::Current& current)
{
    VALIDATE_SESSION;
    return IsActive();
}

void
NodeI::UpdateActiveStatus(const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;
    bool new_active_state = false;
    WHID::ActiveStatusE status;
    {
        WAR_AUTO_LOCK;
        status = mData.mActiveStatus;
    }

    if (WHID::ASAUTO == status)
    {
        if (WHID::NODETASK == GetNodeType())
        {
            new_active_state = 1; // Tasks have no auto-mode.
        }
        else
        {
            WHID::NodeInfoList children;
            GetChildren(false, children, current);

            WHID::NodeInfoList::const_iterator child;
            for(child = children.begin(); child != children.end(); child++)
            {
                if (child->mNode->IsActive(current.ctx))
                {
                    new_active_state = true;
                    break;
                }
            }
        }
    }
    else
        new_active_state = (WHID::ASACTIVE == status);

    SetActive(new_active_state);
    
    // Update parent. If it's on auto, our status might affect it
    WHID::NodePrx parent = GetParent(current);
    if (parent)
        parent->UpdateActiveStatus(current.ctx);
}


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

