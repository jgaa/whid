#include "libwhid.h"
#include "XXI.h"   // class implemented

#include <WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::XXPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const XXI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

XXI::XXI()
{
}// XXI

XXI::~XXI()
{
}// ~XXI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void XXI::Load () throw (WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t rows;

    enum ColsE {  };
    query = "SELECT  FROM ", GetTableName(),
        " WHERE   = ", ,
        " AND ;

    query.Exec(rows);
    if (rows->IsEof())
        WAR_THROW_NOT_FOUND("Category not found");

    mDirty = 0;
}

void XXI::Save () throw (WarException)
{
    if (mDirty)
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        query = "UPDATE ", GetTableName(), " SET "
            WHERE mId = ", GetNstiveKey();

        query.Exec();
    }
}

void XXI::MakeId(const war_int64_t XXId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "XX", XXId);
}

WHID::XXPrx XXI::GetProxy(const war_int64_t XXId)
{
    Ice::Identity id;
    MakeId(XXId, id);

    return WHID::XXPrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::std::string
XXI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    {
        WAR_AUTO_LOCK;
        msg << GetClassName() << " \"" << mName << "\"";
    }

    return msg.str();
}

//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

