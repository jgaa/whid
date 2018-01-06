#include "libwhid.h"
#include "server/HaveChargesI.h"   // class implemented

#include "server/CustomerI.h"
#include "server/ProjectI.h"
#include "server/TaskI.h"
#include "server/RateI.h"
#include "server/WorkI.h"
#include "server/InvoiceI.h"
#include "server/UserI.h"

#include <server/WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>

#include <server/RateI.h>
#include <ice/WarIceException.h>
#include <Ice/Config.h>

/////////////////////////////// PUBLIC ///////////////////////////////////////


WarLog& operator << (WarLog& o, const WHID::HaveChargesPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const HaveChargesI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

HaveChargesI::HaveChargesI()
{
}// HaveChargesI

HaveChargesI::~HaveChargesI()
{
}// ~HaveChargesI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

void
HaveChargesI::GetCharges(::WHID::WorkFilterE filter,
                         const ::WHID::UserPrx& forUser,
                         ::WHID::ChargeDataList& list,
                         const Ice::Current& current) const
{
    VALIDATE_SESSION;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        enum ColsE {C_ID, C_USERID, C_PROJECTID, C_CUSTOMERID, C_WORKID, 
                C_TASKID, C_RATEID, C_NUMHOURS, C_CHARGE, C_DISCOUNT,
                C_T_NAME, C_R_NAME, C_R_CHARGEPERHOUR, C_W_FROMTIME, 
                C_W_TOTIME, C_FROMTIME, C_TOTIME, C_COST, C_PROFIT };

        std::string charge_field = "C.";
        charge_field += GetChargeField();

        query = "SELECT C.Id, C.UserId, C.ProjectId, C.CustomerId, C.WorkId, "
            " C.TaskId, C.RateId, C.NumHours, C.Charge, C.Discount, "
            " T.Name, R.Name, R.ChargePerHour, W.FromTime, W.ToTime, "
            " C.FromTime, C.ToTime, C.Cost, C.Profit "
            " FROM Charge AS C "
            " JOIN Tasks as T ON T.Id = C.TaskId "
            " LEFT JOIN Rates AS R ON R.Id = C.RateId "
            " JOIN Work AS W ON W.Id = C.WorkId "
            " JOIN Projects P ON P.Id = C.ProjectId "
            " WHERE ", charge_field.c_str(), " = ", GetNativeKey(),
	   "  AND W.userId = ", forUser->GetId(current.ctx);

        if (WHID::WFOPEN== filter)
        {
            query += " AND W.Status IN(", 
                (const war_int32_t)WHID::WFACTIVE,
                ",", (const war_int32_t)WHID::WFPENDINGINV,
                ",", (const war_int32_t)WHID::WFHOLDBACK,
                ") ";
        }
        else if (WHID::WFALL != filter)
        {
            query += " AND W.Status = ", filter;
        }

        query += " ORDER BY C.CustomerId, "
            " P.Name, C.ProjectId, T.Name, C.TaskId, W.FromTime, C.WorkId ";

        query.Exec(rows);

#ifdef DEBUG
        // BUG: Resultset comes out in reverse/random order
        std::cout << std::endl;
        std::cout << query.GetQuery() << std::endl;
        std::cout << "Key: " << GetNativeKey() << std::endl;
#endif

        for(; !rows->IsEof(); rows->MoveNext())
        {
            WHID::ChargeData n;

            rows->Get(C_ID, n.mId);
            rows->Get(C_NUMHOURS, n.mNumHours);
            rows->Get(C_CHARGE, n.mCharge);
            rows->Get(C_COST, n.mCost);
            rows->Get(C_PROFIT, n.mProfit);
            rows->Get(C_DISCOUNT, n.mDiscount);

            rows->Get(C_R_CHARGEPERHOUR, n.mChargePrHour);
            rows->Get(C_T_NAME, n.mTaskName);
            rows->Get(C_R_NAME, n.mRateName);

            rows->Get(C_FROMTIME, n.mWorkFrom);
            if ((n.mWorkFrom == "") || (n.mWorkFrom == "0000-00-00 00:00:00"))
                rows->Get(C_W_FROMTIME, n.mWorkFrom);

            rows->Get(C_TOTIME, n.mWorkTo);
            if ((n.mWorkTo == "") || (n.mWorkTo == "0000-00-00 00:00:00"))
                rows->Get(C_W_TOTIME, n.mWorkTo);

            rows->Get(C_WORKID, n.mWorkId);

            if (rows->GetInt64(C_RATEID))
                n.mRate = RateI::GetProxy(rows->GetInt64(C_RATEID));
            else
            {
                n.mRate = 0;
                n.mRateName = "Break";
            }

            n.mWork = WorkI::GetProxy(rows->GetInt64(C_WORKID));
            n.mProject = ProjectI::GetProxy(rows->GetInt64(C_PROJECTID));
            n.mCustomer = CustomerI::GetProxy(rows->GetInt64(C_CUSTOMERID));
            n.mTask = TaskI::GetProxy(rows->GetInt64(C_TASKID));

            list.push_back(n);
            //list.insert(list.begin(), n);
        }
    }
    catch(WarException& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
HaveChargesI::GetInvoices(const ::WHID::UserPrx& forUser,
                          ::WHID::InvoiceInfoList& invoices,
                          const Ice::Current& current) const
{
    VALIDATE_SESSION_FOR_USER;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        enum ColsE {C_ID, C_CUST_NAME, C_CUST_ID, C_USER_NAME, C_USER_ID, 
            C_CHARGE, C_HOURS, C_DATE};

        std::string charge_field = "CH.";
        charge_field += GetChargeField();

        query = "SELECT DISTINCT "
            " I.Id, C.Name, C.Id, U.Name, U.Id, I.Charge, I.Hours, I.InvoiceDate "
            " FROM Invoice AS I "
            " JOIN Customers AS C ON C.Id = I.CustomerId "
            " JOIN Users AS U ON U.Id = I.UserId "
            " JOIN Charge AS CH ON CH.InvoiceId = I.Id "
            " WHERE ", charge_field.c_str(), " = ", GetNativeKey(),
            "  AND I.UserId = ", forUser->GetId(current.ctx),
            "  AND I.Frozen = 1 ";
           
        query += " ORDER BY I.InvoiceDate, C.Name, I.Id ";

        query.Exec(rows);

#ifdef DEBUG
        std::cout << std::endl;
        std::cout << query.GetQuery() << std::endl;
        std::cout << "Key: " << GetNativeKey() << std::endl;
#endif

        for(; !rows->IsEof(); rows->MoveNext())
        {
            ::WHID::InvoiceInfo i;
            i.mFrozen = true;

            rows->Get(C_ID, i.mId);
            rows->Get(C_CUST_NAME, i.mCustomerName);
            rows->Get(C_USER_NAME, i.mUserName);
            rows->Get(C_CHARGE, i.mCharge);
            rows->Get(C_HOURS, i.mHours);
            rows->Get(C_DATE, i.mInvoiceDate);

            i.mInvoice = InvoiceI::GetProxy(i.mId);
            i.mCustomer = CustomerI::GetProxy(rows->GetInt64(C_CUST_ID));
            i.mUser = UserI::GetProxy(rows->GetInt64(C_USER_ID));

            invoices.push_back(i);
        }
    }
    catch(WarException& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void HaveChargesI::GetWork(const ::WHID::UserPrx& forUser,
                    ::WHID::WorkFilterE statusFilter,
                    const ::std::string& fromTime,
                    const ::std::string& toTime,
                    ::WHID::WorkInfoList& children,
                    const Ice::Current& current) const
{
    VALIDATE_SESSION;
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;
        //int where_cnt = 0;

        enum RowsE 
        {   C_ID, C_FROMTIME, C_TOTIME, C_MINUTESUSED, 
            C_MINUTESPAUSED, C_CHARGE, C_STATUS, C_NOTES,
            C_COST, C_PROFIT, C_CUSTOMER, C_PROJECT, C_TASK,
            C_CUSTOMER_NAME, C_PROJECT_NAME, C_TASK_NAME
        };
        query = "SELECT W.Id, W.FromTime, W.ToTime, W.MinutesUsed, W.MinutesPaused, "
            " W.Charge, W.Status, W.Notes, W.Cost, W.Profit, "
            " W.CustomerId, W.ProjectId, W.TaskId, "
            " C.Name, P.Name, T.Name "
            " FROM Work AS W "
            " JOIN Customers AS C ON C.Id = W.CustomerId "
            " JOIN Projects AS P ON P.Id = W.ProjectId "
            " JOIN Tasks AS T ON T.Id = W.TaskId "
            " WHERE W.", GetWorkField(), " = ", GetNativeKey();

            

        if (WHID::WFOPEN == statusFilter)
        {
            query += " AND W.Status IN(", 
                (const war_int32_t)WHID::WFACTIVE,
                ",", (const war_int32_t)WHID::WFPENDINGINV,
                ",", (const war_int32_t)WHID::WFHOLDBACK,
                ") ";
        }
        else if (WHID::WFALL != statusFilter)
        {
            query += " AND W.Status = ", (const war_int32_t)statusFilter;
        }

        if (!fromTime.empty())
        {
            query += " AND W.FromTime >= ", fromTime;
        }

        if (!toTime.empty())
        {
            query += " AND W.FromTime <= ", toTime;
        }

        query += " ORDER BY W.FromTime, W.TaskId, W.Id ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::WorkInfo n;
            rows->Get(C_FROMTIME, n.mFromTime);
            rows->Get(C_TOTIME, n.mToTime);
            rows->Get(C_MINUTESUSED, n.mMinutesUsed);
            rows->Get(C_MINUTESPAUSED, n.mMinutesPaused);
            rows->Get(C_CHARGE, n.mCharge);
            rows->Get(C_COST, n.mCost);
            rows->Get(C_PROFIT, n.mProfit);
            rows->Get(C_NOTES, n.mNotes);
            rows->Get(C_CUSTOMER_NAME, n.mCustomerName);
            rows->Get(C_PROJECT_NAME, n.mProjectName);
            rows->Get(C_TASK_NAME, n.mTaskName);

            n.mStatus = (WHID::WorkStatusE)rows->GetInt32(C_STATUS);
            n.mNode = WorkI::GetProxy(rows->GetInt64(C_ID));
            n.mCustomer = CustomerI::GetProxy(rows->GetInt64(C_CUSTOMER));
            n.mProject = ProjectI::GetProxy(rows->GetInt64(C_PROJECT));
            n.mTask = TaskI::GetProxy(rows->GetInt64(C_CUSTOMER));
            
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


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

