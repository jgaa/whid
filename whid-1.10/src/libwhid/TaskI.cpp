#include "libwhid.h"
#include "server/TaskI.h"   // class implemented

#include "server/WhidEngine.h"
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>

#include "server/ProjectI.h"
#include "server/CustomerI.h"
#include "server/WorkI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::TaskPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const TaskI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

TaskI::TaskI()
{
}// TaskI

TaskI::~TaskI()
{
}// ~TaskI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================




void TaskI::MakeId(const war_int64_t TaskId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "Task", TaskId);
}

WHID::TaskPrx TaskI::GetProxy(const war_int64_t TaskId)
{
    Ice::Identity id;
    MakeId(TaskId, id);

    return WHID::TaskPrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::WHID::NodePrx
TaskI::GetParent(const Ice::Current& current)
{
    VALIDATE_SESSION;
    return ProjectI::GetProxy(mProjectId);
}

void TaskI::GetChildren(::WHID::NodeInfoList& children,
			 const Ice::Current& current) const
{
    VALIDATE_SESSION;
    // We have no tree-node children.
}


::WHID::WorkPrx
TaskI::CreateWork(const ::WHID::UserPrx& forUser, const ::WHID::WorkData& data,
			const Ice::Current& current)
{
    VALIDATE_SESSION_FOR_USER;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);

        //war_int64_t parent = GetParentId();

        query = "INSERT INTO Work "
            " (UserId, TaskId, ProjectId, CustomerId, Notes, "
            " FromTime, ToTime, MinutesPaused, InvoiceId, Status) "
            " VALUES (", (war_int64_t)forUser->GetId(current.ctx),
            ",", GetNativeKey(),
            ",", mProjectId,
            ",", mCustomerId,
            ",", data.mNotes,
            ",", data.mFromTime,
            ",", data.mToTime,
            ",", data.mMinutesPaused,
            ", 0, 1)";

        query.Exec();

        return WorkI::GetProxy(query.GetLastInsertId());
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

::WHID::WorkPrx
TaskI::StartTimer(const ::WHID::UserPrx& forUser, const Ice::Current& current)
{
    VALIDATE_SESSION_FOR_USER;

    WHID::WorkData data;

    // Find the appropriate Rates record
    WHID::RateInfoList rates;
    GetDefaultRates(forUser, rates, current);

    WarTime start_time, end_time;
    
    WHID::RateData rate_data;
    if (!rates.empty())
        rates[0].mRate->GetData(rate_data, current.ctx);
    {
        // Find an appropriate start-time
        //start_time.AlignDown(rate_data.mAlign * 1000 * 60, 
        //    rate_data.mAlignThreshold * 1000 * 60);

        // Align to minute 
        start_time.mTime.mTime /= 60000;
        start_time.mTime.mTime *= 60000;
    }

    if (end_time.mTime < start_time.mTime)
        end_time.mTime = start_time.mTime;

    data.mFromTime = start_time.FormatLT();
    data.mToTime = end_time.FormatLT();
    data.mMinutesPaused = 0;

    ::WHID::WorkPrx work = CreateWork(forUser, data, current);

    work->SetStatus(WHID::WSACTIVE, current.ctx);
    work->SetRates(rates, current.ctx);
    
    return work;
}

void TaskI::GetDefaultRates(const ::WHID::UserPrx& forUser,
                            ::WHID::RateInfoList& rates,
                            const Ice::Current& current) const
{
    VALIDATE_SESSION;
    // Get default rates from the closest object that
    // have them set. Charges are not recursive.

    // Start by our self
    GetRates(rates, current);
    if (rates.empty())
    {
        // Try parent.
        ProjectI::GetProxy(mProjectId)->GetRates(rates, current.ctx);
        if (rates.empty())
        {
            CustomerI::GetProxy(mCustomerId)->GetRates(
                rates, current.ctx);
            if (rates.empty())
            {
                // TODO Get default for user
                if (rates.empty())
                {
                    // Take the entry in the database
                    WHID::RateInfoList all_rates;
                    WhidEngine::GetEngine().GetWhidControl()->GetRates(
                        true, all_rates, current.ctx);
                    WHID::RateInfoList::iterator P;
                    for(P = all_rates.begin(); P != all_rates.end(); P++)
                    {
                        if (P->mChargeType == WHID::RCTABSOLUTE)
                        {
                            // Use this
                            rates.push_back(*P);
                            break;
                        }
                    }
                }
            }
        }
    }
}


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================

bool TaskI::IsActive() const
{
    bool rval;
    {
        WAR_AUTO_LOCK;
        rval = (WHID::ASDISABLED != mData.mActiveStatus);
    }

    return rval;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

void TaskI::GetChildrenClause(WarDbQuery& query) const throw (WarException)
{
}

void TaskI::OnLoadAddJoins(WarDbQuery& query) throw (WarException)
{
}

void TaskI::OnLoadAddToSelect(WarDbQuery& query) throw (WarException)
{
    query += " , CustomerId, ProjectId ";
}

void TaskI::OnLoadAddToWhere(WarDbQuery& query) throw (WarException)
{
}

void TaskI::OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException)
{
    row->Get(C_CUSTOMERID, mCustomerId);
    row->Get(C_PROJECTID, mProjectId);
}

void TaskI::SetActive(const bool active)
{
    WAR_AUTO_LOCK;
    {
        mData.mActiveStatus = active 
            ? WHID::ASACTIVE : WHID::ASDISABLED;
        Flush(-1);
    }
}


/////////////////////////////// PRIVATE    ///////////////////////////////////

