#include "libwhid.h"
#include "server/ProjectI.h"   // class implemented

#include "server/WhidEngine.h"
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>
#include "server/CustomerI.h"
#include "server/TaskI.h"
#include "server/WorkI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::ProjectPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const ProjectI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

ProjectI::ProjectI()
{
}// ProjectI

ProjectI::~ProjectI()
{
}// ~ProjectI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================



void ProjectI::MakeId(const war_int64_t ProjectId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "Project", ProjectId);
}

WHID::ProjectPrx ProjectI::GetProxy(const war_int64_t ProjectId)
{
    Ice::Identity id;
    MakeId(ProjectId, id);

    return WHID::ProjectPrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::WHID::NodePrx
ProjectI::GetParent(const Ice::Current& current)
{
    VALIDATE_SESSION;
    return CustomerI::GetProxy(mCustomerId);
}

void ProjectI::GetTasks(bool onlyActive,
                        ::WHID::TaskList& children,
                        const Ice::Current& current) const
{
    VALIDATE_SESSION;

    WHID::NodeInfoList list;
    GetChildren(onlyActive, list, current);
    WHID::NodeInfoList::iterator child;
    for(child = list.begin(); child != list.end(); child++)
    {
        WHID::TaskPrx prx = WHID::TaskPrx::uncheckedCast(child->mNode);

        children.push_back(prx);
    }
}

::WHID::TaskPrx
ProjectI::CreateTask(const ::WHID::NodeData& data,
			   const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);

        //war_int64_t parent = GetParentId();

        query = "INSERT INTO Tasks "
            " (ProjectId, CustomerId, Name, Notes, ActiveStatus) "
            " VALUES (", GetNativeKey(),
            ",", mCustomerId,
            ",", data.mName, 
            ",", data.mNotes,
            ",", data.mActiveStatus,
            ")";

        query.Exec();

        WHID::TaskPrx task = TaskI::GetProxy(query.GetLastInsertId());
        task->SetData(data, current.ctx);
        return task;
    }
    catch(WarExceptionAlreadyExist)
    {
        WAR_NOTICELOG;
        notice_log << "A task witn name \""
            << data.mName
            << "\" already exist. Cannot create another one with the same name."
            << war_endl;

        throw WHID::AlreadyExistException();
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
ProjectI::RecalculatePending(const ::WHID::UserPrx& forUser,
                             const ::std::string& fromDate,
                             const ::std::string& toDate,
                             const Ice::Current& current)
{
    VALIDATE_SESSION_FOR_USER;

    try
    {
        WHID::WorkList list;
        GetAllPendingWorkItems(forUser, fromDate, toDate, list, current);

        WHID::WorkList::iterator work;
        for(work = list.begin(); work != list.end(); work++)
        {
            (*work)->CalculateCharges(current.ctx);
        }
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
ProjectI::ClearPending(const ::WHID::UserPrx& forUser, const Ice::Current& current)
{
    VALIDATE_SESSION_FOR_USER;

    try
    {
        WHID::WorkList list;
        GetAllPendingWorkItems(forUser, "", "", list, current);

        WHID::WorkList::iterator work;
        for(work = list.begin(); work != list.end(); work++)
        {
            (*work)->DeleteCharges(current.ctx);
        }
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
ProjectI::GetAllPendingWorkItems(const ::WHID::UserPrx& forUser,
                                 const ::std::string& fromDate,
                                 const ::std::string& toDate,
                                 ::WHID::WorkList& list,
                                 const Ice::Current& current) const
{
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;
        std::string my_to_str, my_from_str;

        enum ColsE { C_ID };

        query = "SELECT W.Id "
            " FROM Work W "
            " WHERE W.ProjectId = ", GetNativeKey(),
            " AND W.Status = ", WHID::WSPENDINGINV,
            " AND W.UserId = ", (war_int64_t)forUser->GetId(current.ctx);

        if (!fromDate.empty())
        {
            WarTime my_from;
            my_from.AssignFromAnsiDateTime(fromDate.c_str());
            my_from.ToLT();
            my_from_str = my_from.FormatLT();
            query += " AND W.FromTime >= ", my_from_str;
        }

        if (!toDate.empty())
        {
            WarTime my_to;
            my_to.AssignFromAnsiDateTime(toDate.c_str());
            my_to.ToLT();
            my_to_str = my_to.FormatLT();

            query += " AND W.FromTime <= ", my_to_str;
        }


        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            list.push_back(WorkI::GetProxy(rows->GetInt64(C_ID)));
        }

    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}



//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================

bool ProjectI::IsActive() const
{
    return mIsActive;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

void ProjectI::GetChildrenClause(WarDbQuery& query) const throw (WarException)
{
    query += " WHERE ProjectId = ", GetNativeKey();
}

void ProjectI::OnLoadAddJoins(WarDbQuery& query) throw (WarException)
{
}

void ProjectI::OnLoadAddToSelect(WarDbQuery& query) throw (WarException)
{
    query += ", CustomerId, IsActive ";
}

void ProjectI::OnLoadAddToWhere(WarDbQuery& query) throw (WarException)
{
}

void ProjectI::OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException)
{
    row->Get(C_IS_ACTIVE, mIsActive);
    row->Get(C_CUSTOMERID, mCustomerId);
}

void ProjectI::OnLoadAddToSet(WarDbQuery& query) throw (WarException)
{
    query += ", IsActive = ", mIsActive;
}

void ProjectI::SetActive(const bool active)
{
    WAR_AUTO_LOCK;
    if (mIsActive != active)
    {
        mIsActive = active;
        Flush(-1);
    }
}


/////////////////////////////// PRIVATE    ///////////////////////////////////

