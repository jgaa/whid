#include "libwhid.h"
#include "server/WhidControlI.h"   // class implemented

#include "server/WhidEngine.h"
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>
#include <WarTime.h>

#include "server/CustomerI.h"
#include "server/RateI.h"
#include "server/ProjectI.h"
#include "server/UserI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::WhidControlPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const WhidControlI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

WhidControlI::WhidControlI()
{
    
}// WhidControlI

WhidControlI::~WhidControlI()
{
}// ~WhidControlI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================



//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::WHID::CustomerPrx
WhidControlI::CreateCustomer(const ::WHID::NodeData& data,
				   const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);

        query = "INSERT INTO Customers "
            " (Name, Notes, ActiveStatus) "
            " VALUES (", data.mName, 
            ",", data.mNotes,
            ",", data.mActiveStatus,
            ")";

        query.Exec();

        WHID::CustomerPrx cust = CustomerI::GetProxy(query.GetLastInsertId());
        cust->SetData(data, current.ctx);

        return cust;
    }
    catch(WarExceptionAlreadyExist)
    {
        WAR_NOTICELOG;
        notice_log << "A customer witn name \""
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
WhidControlI::GetCustomers(::WHID::CustFilterE filter,
				 const ::std::string& pattern,
                 bool onlyActive,
				 ::WHID::NodeInfoList& customers,
				 const Ice::Current& current) const
{
    VALIDATE_SESSION;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;
        std::string my_pattern;

        enum RowsE {C_ID, C_NAME, C_ACTIVE };
        query = "SELECT C.Id, C.Name, C.ActiveStatus "
            " FROM Customers AS C ";

        if (!pattern.empty())
        {
            my_pattern = '%';
            my_pattern += pattern;
            my_pattern += '%';

            query += " WHERE Name like (", my_pattern, ") ";
        }

        query += " ORDER BY Name ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::NodeInfo n;
            war_int64_t node_id = rows->GetInt64(C_ID);
            rows->Get(C_NAME, n.mName);
            n.mNode = CustomerI::GetProxy(node_id);
            n.mActiveStatus = (WHID::ActiveStatusE)rows->GetInt32(C_ACTIVE);
            n.mType = WHID::NODECUSTOMER;

            if (onlyActive && !n.mNode->IsActive(current.ctx))
                continue; // Filtered away

            customers.push_back(n);
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


::std::string
WhidControlI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    msg << "WhidControl";

    return msg.str();
}

void
WhidControlI::GetRates(bool onlyActive,
			     ::WHID::RateInfoList& rates,
			     const Ice::Current& current) const
{
    VALIDATE_SESSION;
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;
        std::string my_pattern;

        enum RowsE 
        { 
            C_ID, C_NAME, C_CHARGETYPE, C_PRIORITY, C_FREE, 
            C_CHARGEPERHOUR, C_ACTIVE, C_COST 
        };

        query = "SELECT Id, Name, ChargeType, Priority, Free, "
            " ChargePerHour, Active, Cost "
            " FROM Rates ";

        if (onlyActive)
            query += " WHERE Active = 1 ";

        query += " ORDER BY Name ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::RateInfo n;
            war_int64_t node_id = rows->GetInt64(C_ID);
            rows->Get(C_NAME, n.mName);
            rows->Get(C_PRIORITY, n.mPriority);
            rows->Get(C_FREE, n.mFree);
            rows->Get(C_CHARGEPERHOUR, n.mChargePerHour);
            rows->Get(C_COST, n.mCost);
            rows->Get(C_ACTIVE, n.mActive);
            n.mRate = RateI::GetProxy(node_id);
            n.mChargeType = (WHID::RateChargeTypeE)rows->GetInt32(C_CHARGETYPE);

            rates.push_back(n);
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

::WHID::RatePrx
WhidControlI::AddRate(const ::WHID::RateData& data,
			    const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;

    ::WHID::RatePrx rate;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        
        query = "INSERT INTO Rates "
            " (Name, ChargePerHour, ChargeType) "
            " VALUES (", data.mName, 
            ",", data.mChargePerHour,
            ",", data.mChargeType,
            ")";

        query.Exec();
        rate = RateI::GetProxy(query.GetLastInsertId());
    }
    catch(WarExceptionAlreadyExist)
    {
        WAR_NOTICELOG;
        notice_log << *this << " An rate with the name \""
            << data.mName
            << "\" already exist."
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

    rate->SetData(data, current.ctx);
    return rate;
}





void
WhidControlI::GetOptions(::WHID::Options& opt,
			       const Ice::Current& current) const
{
    // We can't authenticate this, as is is called before login.

    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);    

    for(int retries = 0; retries < 2; retries++)
    {
        try
        {
            WarDbQuery query(db);
            war_db_resultset_ptr_t row;

            enum colsE {C_MULTIUSER};
            query = "SELECT Multiuser "
                " FROM Options "
                " WHERE Id = 1";

            query.Exec(row);

            if (row->IsEof())
            {
                query.Reset();

                // Insert a new record with default options.
                query = "INSERT INTO Options (Id) VALUES (1)";
                query.Exec();
                continue; // Back to retry loop
            }

            row->Get(C_MULTIUSER, opt.mMultiuser);

            return; // Got the options OK
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

    WAR_ERRLOG;
    err_log << *this << "Failed to initialize the Options-table."
        << war_endl;

    throw WHID::InternalErrorException();
}

void
WhidControlI::SetOptions(const ::WHID::Options& opt,
			       const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);

    try
    {
        WarDbQuery query(db);
        
        // We assume that GetOptions() always are called prior to SetOptions. The
        // data-record will therefore exist.


        query = "UPDATE Options "
            " SET Multiuser = ", opt.mMultiuser,
            " WHERE Id = 1";
        query.Exec();
    }
    catch(WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }

    WhidEngine::GetEngine().UpdateMultiuser();
}

void
WhidControlI::GetUserList(::WHID::UserInfoList& users,
				const Ice::Current& current) const
{
    VALIDATE_SESSION;
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    war_db_resultset_ptr_t rows;

    try
    {
        WarDbQuery query(db);
        
        enum ColsE {C_ID, C_NAME, C_LOGINNAME, C_ACTIVE, C_ADMIN };

        query = "SELECT Id, Name, LoginName, Active, Admin "
            " FROM Users "
            " ORDER BY Name ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::UserInfo u;

            war_int64_t id = 0;
            rows->Get(C_ID, id);
            rows->Get(C_NAME, u.mName);
            rows->Get(C_LOGINNAME, u.mLoginName);
            rows->Get(C_ACTIVE, u.mActive);
            rows->Get(C_ADMIN, u.mAdmin);

            u.mUser = UserI::GetProxy(id);

            users.push_back(u);
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

void
WhidControlI::AddUser(const ::WHID::UserData& data,
			    ::WHID::UserPrx& usr,
			    const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);

    try
    {
        WAR_NOTICELOG;

        notice_log << *this 
            << " is adding a new user user: LoginName = \"" 
            << data.mLoginName
            << "\", Name = \"" 
            << data.mName
            << "\"."
            << war_endl;

        WarDbQuery query(db);
        
        query = "INSERT INTO Users (Name, LoginName, Notes, Active, Admin ) "
            " VALUES(", data.mName, ",", 
            data.mLoginName, ",",
            data.mNotes, ",",
            data.mActive, ",",
            data.mAdmin, ")";

        query.Exec();

        usr = UserI::GetProxy(query.GetLastInsertId());
    }
    catch(WarExceptionAlreadyExist)
    {
        throw WHID::AlreadyExistException();
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

void
WhidControlI::GetUser(::Ice::Long userId,
			    ::WHID::UserPrx& usr,
			    const Ice::Current& current) const
{
    VALIDATE_SESSION;

    usr = UserI::GetProxy(userId);
}

//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

