#include "libwhid.h"
#include "server/UserI.h"   // class implemented

#include <server/WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>
#include <WarAtoi.h>
#include <WarTime.h>

#include "server/GatewayI.h"
#include "server/CustomerI.h"
#include "server/RateI.h"
#include "server/ProjectI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::UserPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const UserI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

UserI::UserI()
{
}// UserI

UserI::~UserI()
{
    for(std::string::iterator P = mPassword.begin(); P < mPassword.end(); P++)
        *P = ' ';
}// ~UserI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void UserI::Load () throw (WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t row;

    enum ColsE { C_NAME, C_LOGINNAME, C_PASSWORD, C_EMAIL, C_NOTES, C_ACTIVE, C_ADMIN };
    query = "SELECT "
        " Name, LoginName, Password, Email, Notes, Active, Admin "
        " FROM Users ",
        " WHERE Id = ", GetNativeKey();

    query.Exec(row);
    if (row->IsEof())
        WAR_THROW_NOT_FOUND("Category not found");

    row->Get(C_NAME, mName);
    row->Get(C_LOGINNAME, mLoginName);
    row->Get(C_PASSWORD, mPassword);
    row->Get(C_EMAIL, mEmail);
    row->Get(C_NOTES, mNotes);
    row->Get(C_ACTIVE, mActive);
    row->Get(C_ADMIN, mAdmin);

    mDirty = 0;
}

void UserI::Save () throw (WarException)
{
    if (mDirty)
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        query = "UPDATE Users SET "
            " Name = ", mName, ","
            " LoginName = ", mLoginName, ","
            " Password = ", mPassword, ","
            " Email = ", mEmail, ",",
            " Notes = ", mNotes, ","
            " Active = ", mActive, ","
            " Admin = ", mAdmin, 
            " WHERE Id = ", GetNativeKey();

        query.Exec();
    }
}

void UserI::MakeId(const war_int64_t UserId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "User", UserId);
}

WHID::UserPrx UserI::GetProxy(const war_int64_t UserId)
{
    Ice::Identity id;
    MakeId(UserId, id);

    return WHID::UserPrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}




//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::Ice::Long
UserI::GetId(const Ice::Current& current) const
{
    VALIDATE_SESSION;
    return WarAtoi<war_int64_t, char>(GetNativeKey().c_str());
}

::std::string
UserI::GetName(const Ice::Current& current) const
{
    VALIDATE_SESSION;
    ::std::string name;
    {
        WAR_AUTO_LOCK;
        name = mName;
    }

    return name;
}

void
UserI::GetData(::WHID::UserData& data,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;
    {
        WAR_AUTO_LOCK;
        data.mId = WarAtoi<war_int64_t, char>(GetNativeKey().c_str());
        data.mName = mName;
        data.mLoginName = mLoginName;
        data.mEmail = mEmail;
        data.mNotes = mNotes;
        data.mActive = mActive;
        data.mAdmin = mAdmin;
    }
}

void
UserI::GetInfo(::WHID::UserInfo& info,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;
    {
        WAR_AUTO_LOCK;
        const war_int64_t id = WarAtoi<war_int64_t, char>(GetNativeKey().c_str());
        info.mName = mName;
        info.mLoginName = mLoginName;
        info.mActive = mActive;
        info.mAdmin = mAdmin;
        info.mUser = GetProxy(id);
    }
}

void
UserI::SetData(const ::WHID::UserData& data,
		     const Ice::Current& current)
{
    VALIDATE_SESSION_USER;

    {
        WAR_AUTO_LOCK;

        if (!is_admin)
        {
            if ((mActive != data.mActive)
                || (mAdmin != data.mAdmin))
            {
                my_lock.Release();
                WAR_NOTICELOG;
                notice_log << *this << " tried to alter admin or active status. "
                    << "Access was denied, as this user is not an admin."
                    << war_endl;

                throw WHID::AccessDeniedException();
            }
        }

        mName = data.mName;
        mLoginName = data.mLoginName;
        mEmail = data.mEmail;
        mNotes = data.mNotes;
        mActive = data.mActive;
        mAdmin = data.mAdmin;

        Flush(-1);
    }
}

void
UserI::SetPassword(const ::std::string& passwd,
			 const Ice::Current& current)
{
    VALIDATE_SESSION_USER;
    {
        WAR_AUTO_LOCK;
        mPassword = passwd;
        Flush(-1);
    }
}


::std::string
UserI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    {
        WAR_AUTO_LOCK;
        msg << "User #" << GetNativeKey() << " \"" << mName << "\"";
    }

    return msg.str();
}

void UserI::GetProjectSummary(::WHID::ProjectSummaryModeE mode,
				      const ::std::string& period,
				      ::WHID::ProjectSummaryList& result,
				      const Ice::Current& current) const
{
    VALIDATE_SESSION_USER;
    try
    {
        switch(mode)
        {
        case WHID::PSMWEEK:
            {
                WarTime from_date, to_date;
                GetWeekDates(period, from_date, to_date);
                GetProjectSummaryWeek(from_date, to_date, result, current);
            }
            break;
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
UserI::GetProjectsForBilling(const ::std::string& fromDate,
					  const ::std::string& toDate,
					  ::WHID::ProjectForBillingInfoList& list,
					  const Ice::Current& current) const
{
    VALIDATE_SESSION_USER;
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        enum ColsE {C_C_ID, C_C_NAME, C_P_ID, C_P_NAME, C_MINUTES};

        WarTime my_from, my_to;
        my_from.AssignFromAnsiDateTime(fromDate.c_str());
        my_from.ToLT();
        my_to.AssignFromAnsiDateTime(toDate.c_str());
        my_to.ToLT();
        std::string my_from_str = my_from.FormatLT();
        std::string my_to_str = my_to.FormatLT();

        query = "SELECT C.Id, C.Name, P.Id, P.Name, SUM(W.MinutesUsed) "
            " FROM Work AS W "
            " JOIN Projects AS P ON P.Id = W.ProjectId "
            " JOIN Customers AS C ON C.Id = W.CustomerId "
            " WHERE W.Status = ", WHID::WSPENDINGINV,
            "   AND W.UserId = ", GetNativeKey(),
            "   AND W.FromTime >= ", my_from_str,
            "   AND W.FromTime <= ", my_to_str,
            " GROUP BY (W.ProjectId) "
            " ORDER BY C.Name, P.Name ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::ProjectForBillingInfo n;
            rows->Get(C_C_NAME, n.mCompanyName);
            rows->Get(C_P_NAME, n.mProjectName);
            rows->Get(C_C_ID, n.mCompanyId);
            rows->Get(C_P_ID, n.mProjectId);
            rows->Get(C_MINUTES, n.mHoursToBill);

            n.mProject = ProjectI::GetProxy(rows->GetInt64(C_P_ID));
            n.mCustomer = CustomerI::GetProxy(rows->GetInt64(C_C_ID));

            list.push_back(n);
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
UserI::GetProjectsReadyForInvoicing(::WHID::ProjectWithChargeForBillingInfoList& list,
						 const Ice::Current& current) const
{
    VALIDATE_SESSION_USER;
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        enum ColsE {C_C_ID, C_C_NAME, C_P_ID, C_P_NAME, C_MINUTES, C_CHARGE};

        query = "SELECT C.Id, C.Name, P.Id, P.Name, SUM(W.MinutesUsed), SUM(CH.Charge) "
            " FROM Work AS W "
            " JOIN Projects AS P ON P.Id = W.ProjectId "
            " JOIN Customers AS C ON C.Id = W.CustomerId "
            " JOIN Charge AS CH ON CH.WorkId = W.Id "
            " WHERE W.Status = ", WHID::WSPENDINGINV,
            "   AND W.UserId = ", GetNativeKey(),
            " GROUP BY (W.ProjectId) "
            " ORDER BY C.Name, C.Id, P.Name ";

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            WHID::ProjectWithChargeForBillingInfo n;
            rows->Get(C_C_NAME, n.mCompanyName);
            rows->Get(C_P_NAME, n.mProjectName);
            rows->Get(C_C_ID, n.mCompanyId);
            rows->Get(C_P_ID, n.mProjectId);
            rows->Get(C_MINUTES, n.mHoursToBill);
            rows->Get(C_CHARGE, n.mCharge);

            n.mProject = ProjectI::GetProxy(rows->GetInt64(C_P_ID));
            n.mCustomer = CustomerI::GetProxy(rows->GetInt64(C_C_ID));

            list.push_back(n);
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
UserI::MakeInvoices(::WHID::InvoiceInfoList& list,
				 const Ice::Current& current)
{
    VALIDATE_SESSION_USER;

    WHID::ProjectWithChargeForBillingInfoList prj_list;
    GetProjectsReadyForInvoicing(prj_list, current);

    // Loop trough the companies
    WHID::ProjectWithChargeForBillingInfoList::iterator prj, sibling;
    war_int64_t last_company = -1;
    for(prj = prj_list.begin(); prj != prj_list.end(); prj++)
    {
        if (last_company == prj->mCompanyId)
            continue; // Already processed

        // Get a list over all the projcets we want for this company
        WHID::ProjectList projects;
        last_company = prj->mCompanyId;
        for(sibling = prj
            ; (sibling != prj_list.end()) 
            && (last_company == sibling->mCompanyId)
            ; sibling++)
        {
            projects.push_back(sibling->mProject);
        }

        // Make the invoice
        WHID::InvoicePrx invoice = prj->mCustomer->MakeInvoice(projects, 
            GetProxy(atoi(GetNativeKey().c_str())),
            current.ctx);
        WHID::InvoiceInfo n;
        invoice->GetInfo(n, current.ctx);
        list.push_back(n);
    }   
}

void
UserI::GetInvoicesAsXml(const ::WHID::InvoiceList& invoices,
				     ::std::string& xml,
				     const Ice::Current& current) const
{
    //WarFastBuffer
}


void
UserI::ClearAllChargesNotInvoiced(const Ice::Current& current)
{
    VALIDATE_SESSION_USER;

    try
    {
        // Get a list of the charges involved
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        query = "SELECT C.Id FROM Charge C "
            " JOIN Work W ON W.Id = WorkId "
            " WHERE W.Status < ", WHID::WSINVOICED,
            " AND W.userId = " , GetNativeKey();

        std::vector<war_int64_t> items;
        items.reserve(1000);

        for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
        {
            items.push_back(rows->GetInt64(0));
        }

        // Delete all items in the list
        rows = NULL;
        war_int64_t my_id = 0;
        query = "DELETE FROM Charge WHERE Id = ", my_id;

        std::vector<war_int64_t>::const_iterator id;
        for(id = items.begin(); id != items.end(); id++)
        {
            my_id = *id;
            query.Exec();
            query.ReBind();
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
/////////////////////////////// PROTECTED  ///////////////////////////////////

void UserI::GetProjectSummaryWeek(const WarTime& fromDate ,
        const WarTime& toDate,
        ::WHID::ProjectSummaryList& result,
        const Ice::Current& current) const
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t rows;
    

    enum ColsE 
    {
        C_SUM, C_PNAME, C_CNAME, C_PID, C_DAY
    };

    /*query = "SELECT ";
    
    if (WhidEngine::GetEngine().mDbType == "mysql")
        query += "CAST(SUM(W.MinutesUsed) AS SIGNED INTEGER), ";
    else
        query = "SUM(W.MinutesUsed), ";
    */

    std::string from_date = fromDate.FormatLT("%Y-%m-%d 00:00:00");
    std::string to_date = toDate.FormatLT("%Y-%m-%d 23:59:59");

    query = "SELECT "
        " SUM(W.MinutesUsed), P.Name, C.name, P.Id,  DATE(W.FromTime) AS Day "
        " FROM Work W "
        " JOIN Projects P ON P.Id = W.ProjectId "
        " JOIN Customers C ON C.Id = W.CustomerId "
        " WHERE W.FromTime >= ", from_date,
        "  AND W.ToTime <= ", to_date,
        "  AND W.userId = ", GetNativeKey(),
        " GROUP BY ProjectId, Day "
        " ORDER BY C.Name, P.Name, Day ";

#define FLUSH_PROJECT(cleanup) \
    if (cleanup || (current_prj_id != prj_id)) { \
        if (-1 != current_prj_id) \
            result.push_back(s);\
        if (!rows->IsEof()) {\
            s.mTotalMinutes = 0;\
            s.mWorked.clear();\
            s.mWorked.resize(7, 0);\
            s.mProject = NULL;\
            rows->Get(C_PNAME, s.mProjectName);\
            rows->Get(C_CNAME, s.mCompanyName);\
            s.mProject = ProjectI::GetProxy(prj_id);\
        }\
        current_prj_id = prj_id;\
    }

    WHID::ProjectSummary s;
    war_int64_t current_prj_id = -1, prj_id = 0;
    for(query.Exec(rows); !rows->IsEof(); rows->MoveNext())
    {
        rows->Get(C_PID, prj_id);
        FLUSH_PROJECT(false);

        std::string day;
        rows->Get(C_DAY, day);
        WarTime when;
        when.AssignFromAnsiDateTime(day.c_str(), false, true);
        when.ToLT();
        struct tm tm = when.GetTm();
        const int day_of_week = ((tm.tm_wday == 0) ? 6 : tm.tm_wday -1);

        war_int32_t minutes = rows->GetInt32(C_SUM);

        s.mWorked[day_of_week] += minutes;
        s.mTotalMinutes += minutes;
    }

    FLUSH_PROJECT(true);
}

void UserI::GetWeekDates(const std::string& anyDate, 
        WarTime& fromDate, 
        WarTime& toDate) const
        throw(WarException, Ice::Exception)
{

    const war_time_t ms_in_a_day = 1000 * 60 * 60 * 24;

    // Find the week
    fromDate.AssignFromAnsiDateTime(anyDate.c_str(), false, true);

    struct tm tm = fromDate.GetTm();
    int day_of_week = ((tm.tm_wday == 0) ? 6 : tm.tm_wday) -1;
    fromDate.mTime -= (day_of_week * ms_in_a_day);

    // Align to day boundry
    fromDate.mTime = (fromDate.mTime / ms_in_a_day) * ms_in_a_day;

    // Set to-date one week ahead.
    toDate.mTime = fromDate.mTime + (ms_in_a_day * 7);
}

/////////////////////////////// PRIVATE    ///////////////////////////////////

