#include "libwhid.h"
#include "server/WorkI.h"   // class implemented

#include "server/WhidEngine.h"
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>
#include <WarAutoDeletePointer.h>

#include "server/ProjectI.h"
#include "server/CustomerI.h"
#include "server/TaskI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::WorkPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const WorkI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

WorkI::WorkI()
{
}// WorkI

WorkI::~WorkI()
{
}// ~WorkI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void WorkI::Load () throw (WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t row;

    enum ColsE 
    { 
        C_USERID, C_TASKID, C_PROJECTID, C_CUSTOMERID, C_INVOICEID, 
        C_NOTES, C_FROMTIME, C_TOTIME, C_MINUTESPAUSED, C_MINUTESUSED, 
        C_CHARGE, C_STATUS, C_COST, C_PROFIT 
    };
    query = "SELECT UserId, TaskId, ProjectId, CustomerId, InvoiceId, "
        " Notes, FromTime, ToTime, MinutesPaused, MinutesUsed, Charge, "
        " Status, Cost, profit "
        " FROM Work "
        " WHERE Id = ", GetNativeKey();

    query.Exec(row);
    if (row->IsEof())
        WAR_THROW_NOT_FOUND("Work not found");

    row->Get(C_USERID, mUserId);
    row->Get(C_TASKID, mTaskId);
    row->Get(C_PROJECTID, mProjectId);
    row->Get(C_CUSTOMERID, mCustomerId);
    row->Get(C_INVOICEID, mInvoiceId);
    row->Get(C_NOTES, mNotes);
    row->Get(C_FROMTIME, mFromTime);
    row->Get(C_TOTIME, mToTime);
    row->Get(C_MINUTESPAUSED, mMinutesPaused);
    row->Get(C_MINUTESUSED, mMinutesUsed);
    row->Get(C_CHARGE, mCharge);
    row->Get(C_COST, mCost);
    row->Get(C_PROFIT, mProfit);
    mStatus = (WHID::WorkStatusE)row->GetInt32(C_STATUS);
   
    mDirty = 0;
}

void WorkI::Save () throw (WarException)
{
    Resync();

    if (mDirty)
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);

        query = "UPDATE Work SET "
            "UserId = ", mUserId,
            ", TaskId = ", mTaskId,
            ", ProjectId = ", mProjectId,
            ", CustomerId = ", mCustomerId,
            ", InvoiceId = ", mInvoiceId,
            ", Notes = ", mNotes,
            ", FromTime = ", mFromTime,
            ", ToTime = ", mToTime,
            ", MinutesPaused = ", mMinutesPaused,
            ", MinutesUsed = ", mMinutesUsed,
            ", Charge = ", mCharge,
            ", Cost = ", mCost,
            ", Profit = ", mProfit,
            ", Status = ", (const war_int32_t)mStatus,
            " WHERE Id = ", GetNativeKey();

        query.Exec();

        mDirty = 0;
    }
}

void WorkI::MakeId(const war_int64_t WorkId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "Work", WorkId);
}

WHID::WorkPrx WorkI::GetProxy(const war_int64_t WorkId)
{
    Ice::Identity id;
    MakeId(WorkId, id);

    return WHID::WorkPrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}

void WorkI::ValidateSetRatesAccess(const Ice::Current& current)
        throw(Ice::Exception)
{
    VALIDATE_SESSION;
}

//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::std::string
WorkI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    {
        WAR_AUTO_LOCK;
        msg << "Work #" << " \"" << GetNativeKey() << "\"";
    }

    return msg.str();
}

::WHID::TaskPrx
WorkI::GetParent(const Ice::Current& current) const
{
    VALIDATE_SESSION;
    return TaskI::GetProxy(mTaskId);
}

void
WorkI::SetData(const ::WHID::WorkData& data,
		      const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    {
        WAR_AUTO_LOCK;

        if (WHID::WSINVOICED == mStatus)
        {
            WAR_ERRLOG;
            err_log << "Cannot update work-item " << *this
                << ". This item is invoiced and likely referenced by the invoice."
                << war_endl;

            throw WHID::IntegrityErrorException();
        }

        mNotes = data.mNotes;
        mFromTime = data.mFromTime;
        mToTime = data.mToTime;
        mMinutesPaused = data.mMinutesPaused;

        try
        {
            Resync();
        }
        catch(const WarException& ex)
        {
            WAR_ERRLOG;
            err_log << *this << " caught an exception: "
                << ex.what()
                << war_endl;

            throw WHID::InternalErrorException();
        }
        
        Flush(-1);
    }
}

void
WorkI::GetData(::WHID::WorkData& data,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    {
        WAR_AUTO_LOCK;
        data.mNotes = mNotes;
        data.mFromTime = mFromTime;
        data.mToTime = mToTime;
        data.mMinutesPaused = mMinutesPaused;
    }
}

void
WorkI::GetInfo(::WHID::WorkInfo& info,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;

    {
        WAR_AUTO_LOCK;
        
        info.mNotes = mNotes;
        info.mFromTime = mFromTime;
        info.mToTime = mToTime;
        info.mMinutesUsed = mMinutesUsed;
        info.mMinutesPaused = mMinutesPaused;
        info.mCharge = mCharge;
        info.mCost = mCost;
        info.mProfit = mProfit;
        info.mStatus = mStatus;

        info.mNode = GetProxy(atoi(GetNativeKey().c_str()));
    }
}

::WHID::WorkStatusE
WorkI::GetStatus(const Ice::Current& current) const
{
    VALIDATE_SESSION;
    
    ::WHID::WorkStatusE rval;

    {
        WAR_AUTO_LOCK;
        rval = mStatus;
    }

    return rval;
}   

void
WorkI::SetStatus(::WHID::WorkStatusE status,
		       const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    {
        WAR_AUTO_LOCK;

        if (WHID::WSINVOICED == mStatus)
        {
            WAR_ERRLOG;
            err_log << "Cannot delete work-item " << *this
                << ". This item is invoiced and likely referenced by the invoice."
                << war_endl;

            throw WHID::IntegrityErrorException();
        }

        mStatus = status;
        Flush(-1);
    }
}


void
WorkI::CloseTimer(const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    if (GetStatus(current) == WHID::WSACTIVE)
    {
        WHID::WorkStatusE new_status = WHID::WSPENDINGINV;
        
        WarTime end_time;

        WHID::RatePrx rate = GetMainRate(current);
        if (rate)
        {
           WHID::RateData data;
           rate->GetData(data, current.ctx);
           if (data.mFree)
              new_status = WHID::WSFREE;
        }

        SetStatus(new_status, current);

        {
            WAR_AUTO_LOCK;
            mToTime = end_time.FormatLT();
            Flush(-1);
        }
    }
}

::WHID::RatePrx
WorkI::GetMainRate(const Ice::Current& current) const
{
    VALIDATE_SESSION;
    
    WHID::RateInfoList rates;
    HaveRatesI::GetRates(rates, current);

    // Find the first normal type
    WHID::RateInfoList::iterator rate;

    for(rate = rates.begin(); rate != rates.end(); rate++)
    {
        if (!rate->mActive)
            continue;

        if (rate->mChargeType == WHID::RCTABSOLUTE)
            return rate->mRate;
    }

    // Try parents
    WHID::NodePrx node = GetParent(current);
    for(; node; node = node->GetParent(current.ctx))
    {
        node->GetRates(rates, current.ctx);
        for(rate = rates.begin(); rate != rates.end(); rate++)
        {
            if (!rate->mActive)
                continue;

            if (rate->mChargeType == WHID::RCTABSOLUTE)
                return rate->mRate;
        }
    }

    // TODO: Try user
    
    return 0;
}

void
WorkI::AddPause(::Ice::Int minutes,
		      const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    WAR_AUTO_LOCK;
    mMinutesPaused += minutes;
    Flush(-1);
}

void
WorkI::SetEndtimeToNow(const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    if (GetStatus(current) == WHID::WSACTIVE)
    {
        WAR_AUTO_LOCK;
        mToTime = WarTime().FormatLT();
        Flush(-1);
    }
    else
    {
        throw WHID::IntegrityErrorException();
    }
}

void
WorkI::DeleteCharges(const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);

        query = "DELETE FROM Charge WHERE WorkId = ", GetNativeKey();
        query.Exec();
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

struct auto_delete_array
{
    auto_delete_array(int(*ptr)[WorkI::CHARGE_TYPES])
        : mPtr(ptr)
    {
    }

    auto_delete_array()
    {
        if (mPtr)
        {
            delete[] mPtr;
            mPtr = NULL;
        }

    }

    int(*mPtr)[WorkI::CHARGE_TYPES];
};

void
WorkI::CalculateCharges(const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    DeleteCharges(current);

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);

        // Get the charges that apply
        WHID::RateInfoList rates;
        GetRates(rates, current);
        WHID::RatePrx main_rate_prx = GetMainRate(current);
        WHID::RateData main_rate;
        main_rate_prx->GetData(main_rate, current.ctx);
        const int resolution = main_rate.mAlign;

        // main rate decides resolution
        WarTime from_time, to_time;
        from_time.AssignFromAnsiDateTime(mFromTime.c_str());
        from_time.ToLT();
        to_time.AssignFromAnsiDateTime(mToTime.c_str());
        to_time.ToLT();

        // Align
        const int real_elapsed_minutes = 
            (int)((to_time.mTime - from_time.mTime) / 60000L);

        from_time.AlignDown(main_rate.mAlignStart * 60 * 1000,
            main_rate.mAlignThreshold * 1000 * 60);

        // Adjust the to-time relative to the down-alignment.
        to_time.mTime = from_time.mTime + ((war_time_t)real_elapsed_minutes * 60000);

        // Adjust the to-time according to the Align rule.
        to_time.AlignUp(main_rate.mAlign * 60 * 1000,
            main_rate.mAlignThreshold * 1000 * 60);

        // Align to the resolution
        {
            //const int my_span = (int)((to_time.mTime - from_time.mTime) / 60000L);
            //const int units = (my_span / resolution) ? (my_span / resolution) : 1;
            to_time.mTime.mTime /= (resolution * 60000L);
            to_time.mTime.mTime *= (resolution * 60000L);
        }

        const int time_span_in_minutes = (int)
            ((to_time.mTime - from_time.mTime) / 60000L);
        const int time_units = (time_span_in_minutes / resolution) 
            ? (time_span_in_minutes / resolution) : 1;

        if (time_span_in_minutes < 1)
            return; // Nothing to do

        // Cache the rate-data
        std::vector<WHID::RateData> rate_data;
        rate_data.resize(rates.size());
        {
            WHID::RateInfoList::iterator rate;
            int i = 0;
            for(rate = rates.begin(); rate != rates.end(); rate++)
                rate->mRate->GetData(rate_data[i++], current.ctx);
        }

        
        const size_t matrix_size = time_units * (CHARGE_TYPES);
        int (*rate_matrix)[CHARGE_TYPES] = new int[time_units][CHARGE_TYPES];
        auto_delete_array cleanup_matrix(rate_matrix);
        memset(rate_matrix, 0, matrix_size * sizeof(int));
        bool have_fee[sizeof(int) * 8];
        memset(have_fee, 0, sizeof(have_fee));

        const int num_rates = (int)WAR_MIN(rates.size(), (sizeof(int) * 8));

#define MATRIX_SET(mtx_set_time_unit, mtx_set_type, mtx_set_rateix, mtx_bool_value) {\
    WAR_ASSERT(mtx_set_time_unit >= 0); \
    WAR_ASSERT(mtx_set_time_unit < time_units);\
    WAR_ASSERT(mtx_set_type >= 0);\
    WAR_ASSERT(mtx_set_type <= WHID::RTCEXTRAFEE);\
    WAR_ASSERT(mtx_set_rateix >= 0);\
    WAR_ASSERT(mtx_set_rateix < num_rates);\
    if (mtx_bool_value)\
        rate_matrix[mtx_set_time_unit][mtx_set_type] |= (1 << mtx_set_rateix);\
    else\
        rate_matrix[mtx_set_time_unit][mtx_set_type] &= ~(1 << mtx_set_rateix);\
    }

#define MATRIX_CLEAR_TYPE(mtx_set_time_unit, mtx_set_type) {\
    WAR_ASSERT(mtx_set_time_unit >= 0); \
    WAR_ASSERT(mtx_set_time_unit < time_units);\
    WAR_ASSERT(mtx_set_type >= 0);\
    WAR_ASSERT(mtx_set_type <= WHID::RTCEXTRAFEE);\
    rate_matrix[mtx_set_time_unit][mtx_set_type] = 0;\
    }


#define MATRIX_GET(mtx_get_time_unit, mtx_get_type, mtx_get_rateix)\
    ((rate_matrix[mtx_get_time_unit][mtx_get_type] & (1 << mtx_get_rateix)) != 0)

// Get the value in previous (time unit) cell, or false if this is the first
#define MATRIX_PREV(mtx_get_time_unit, mtx_get_type, mtx_get_rateix)\
    ((mtx_get_time_unit > 0) ? \
        MATRIX_GET(mtx_get_time_unit-1, mtx_get_type, mtx_get_rateix)\
        : false)

// Get the value in next (time unit) cell, or false if this is the last
#define MATRIX_NEXT(mtx_get_time_unit, mtx_get_type, mtx_get_rateix)\
    (((mtx_get_time_unit+1) >= time_units) ?\
    false : \
    MATRIX_GET(mtx_get_time_unit+1, mtx_get_type, mtx_get_rateix))


        /*  Build a bit-matrix over what fees that applies to what
            time-units.
        */

        //int curr_rate = -1, prev_rate = -1, try_rate = -1;
        for(int unit = 0; unit < time_units; unit++)
        {
            WarTime start_unit, end_unit;
            start_unit.mTime= from_time.mTime + 
                (unit * time_span_in_minutes * 60000L);
            end_unit.mTime = start_unit.mTime + (time_span_in_minutes * 60000L);
            const int time_span_in_minutes = (int)
                ((start_unit.mTime - from_time.mTime) / 60000L);

            // Select the charge for this unit
            // We can have one rate for each bit in an integer
            int try_rate;
            for(try_rate = 0; try_rate < num_rates; try_rate++)
            {
                WHID::RateData& RATE = rate_data[try_rate];

                // Check active
                if (!RATE.mActive)
                    continue;

                struct tm tm = start_unit.GetTm();

                // Check day constraint
                {
                    bool have_day = false;
                    const char *p = RATE.mAppliesToDayMask.c_str();
                    for(; *p && !have_day; p++)
                    {
                        if ('a' == *p)
                            have_day = true;
                        else if ('m' == *p)
                            have_day = (tm.tm_wday == 1);
                        else if ('t' == *p)
                            have_day = (tm.tm_wday == 2);
                        else if ('w' == *p)
                            have_day = (tm.tm_wday == 3);
                        else if ('T' == *p)
                            have_day = (tm.tm_wday == 4);
                        else if ('f' == *p)
                            have_day = (tm.tm_wday == 5);
                        else if ('s' == *p)
                            have_day = (tm.tm_wday == 6);
                        else if ('S' == *p)
                            have_day = (tm.tm_wday == 0);
                        else if (('p' == *p) || ('X' == *p))
                        {
                            if ((tm.tm_mon == 11) 
                                && ((tm.tm_mday == 24)
                                || (tm.tm_mday == 25)
                                || (tm.tm_mday == 31)))
                                have_day = true;
                        }
                    }

                    if (!have_day)
                        continue;
                }

                // Check clock constraint
                if ((-1 != RATE.mFromClock) && (-1 != RATE.mToClock))
                {
                    if (!((RATE.mFromClock >= tm.tm_hour)
                        && (RATE.mToClock <= tm.tm_hour)))
                        continue;
                }

                // Check continous hours constraint
                if (0 < RATE.mNumContinousHours)
                {
                    if (RATE.mNumContinousHours >= time_span_in_minutes)
                        continue;
                }

                if (RATE.mChargeType == WHID::RCTABSOLUTE)
                {
                    // There are only one of these. We have ro delete
                    // any previous ones
                    MATRIX_CLEAR_TYPE(unit, RATE.mChargeType);
                } 
                else if (RATE.mChargeType == WHID::RTCEXTRAFEE)
                {
                    // This type only apply once
                    if (have_fee[try_rate])
                        continue;

                    have_fee[try_rate] = true;
                }

                
                // Insert the rate in the matrix
                MATRIX_SET(unit, RATE.mChargeType, try_rate, true);
            }
        }

        /*  Go over the matrix and generate charges based on the 
            continous charges. Do one type of the time */

        double normal_charge = 0, full_charge = 0;
        double work_cost = 0, work_profit = 0;

        int start_unit = -1;
        int current_type;
        for(current_type = WHID::RCTABSOLUTE
            ; current_type <= WHID::RTCEXTRAFEE
            ; current_type++)
        {
            for(int current_rate = 0; current_rate < num_rates; current_rate++)
            {
                for(int unit = 0; unit < time_units; unit++)
                {
                    WHID::RateData& RATE = rate_data[current_rate];

                    if (MATRIX_GET(unit, current_type, current_rate))
                    {
                        if (-1 == start_unit)
                            start_unit = unit;

                        if (!MATRIX_NEXT(unit, current_type, current_rate))
                        {
                            // If this is /not/ the last interval,
                            // check if the rule is aligned over
                            // pauses.
                            if ((unit < (time_units -1))
                                && RATE.mMaxTimeBetweenContinousIntervals)
                            {
                                // See if we are within the boundry
                                const int num_units = 
                                    ((RATE.mMaxTimeBetweenContinousIntervals 
                                    / resolution) * resolution);

                                bool do_align = false;

                                for(int u = 0; u < num_units; u++)
                                {
                                    const int try_unit = WAR_MIN(
                                        num_units + u, (time_units -1));
                                    if (MATRIX_GET(try_unit, current_type, current_rate))
                                    {
                                        do_align = true;
                                        unit += (u -1);
                                        break;
                                    }
                                }

                                if (do_align)
                                {
                                    continue; // Jump to for(unit ...)
                                }
                            } //  if (unit < (time_units -1))
                            else
                            {
                                // Finish this time-period and charge.
                                WAR_ASSERT(start_unit >= 0);

                                WarTime charge_from, charge_to;
                                charge_from.mTime = (from_time.mTime 
                                    + (start_unit * resolution * 60000));
                                charge_to.mTime = (from_time.mTime 
                                    + ((unit +1) * resolution * 60000));

                                // Find the normal rate for this time
                                int normal = 0;
                                for(; normal < num_rates; normal++)
                                {
                                    if (MATRIX_GET(unit, WHID::RCTABSOLUTE, normal))
                                        break;
                                }
                                if (normal == num_rates)
                                    normal = 0;

                                Charge(charge_from, charge_to, 
                                    rate_data[current_rate], 
                                    rate_data[normal],
                                    db, 
                                    normal_charge,
                                    full_charge,
                                    work_cost,
                                    work_profit);

                            } //  if (unit < (time_units -1)) ... else
                        } // if !MATRIX_NEXT
                    } // if (MATRIX_GET(
                    else
                    {
                        start_unit = -1; // No start-unit
                    } // if (MATRIX_GET( ... else
                } // for(int unit
            } // for(current_rate
        } // for(current_type 

        

        /*  If the work have been paused, credit a percentage of
            the charges, excluding fees. */
        if (mMinutesPaused > resolution)
        {
            WarDbQuery query(db);
            double num_hours = 0, charge = 0, discount = 0, total_hours = 0;

            const int minutes_paused = (mMinutesPaused / resolution) * resolution;

            num_hours = minutes_paused / 60;
            num_hours += ((double)(minutes_paused % 60)) * (1.0 / 60.0);

            const int total_minutes = (int)((to_time.mTime - from_time.mTime) / 60000L);
            total_hours =  total_minutes / 60;
            total_hours += ((double)(total_minutes % 60)) * (1.0 / 60.0);

            const double percentage = (num_hours / total_hours) * 100;

            charge = (int)((full_charge / 100) * percentage);
            charge *= -1;

            num_hours *= -1;

            double tmp_cost = (int)((work_cost / 100) * percentage);
            tmp_cost *= -1;

            double tmp_profit = (int)((work_profit / 100) * percentage);
            tmp_profit *= -1;
            
            full_charge += charge;
            work_profit += tmp_profit;
            work_cost += tmp_cost;

            WarTime pause_to;
            pause_to.mTime = from_time.mTime + (minutes_paused * 60000L);

            query = "INSERT INTO Charge (UserId, ProjectId, CustomerId, TaskId, WorkId, "
                " RateId, NumHours, Charge, Discount, FromTime, ToTime, Cost, Profit) "
                " VALUES(", mUserId,
                ",", mProjectId,
                ",", mCustomerId,
                ",", mTaskId,
                ",", GetNativeKey(),
                ",", 0,
                ",", num_hours,
                ",", charge,
                ",", discount,
                ",", from_time,
                ",", pause_to,
                ",", tmp_cost,
                ",", tmp_profit,
                ")";

            query.Exec();
        }

        {
            WAR_AUTO_LOCK;
            mCharge = full_charge;
            mCost = work_cost;
            mProfit = work_profit;

            Flush(-1);
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
WorkI::GetRates(::WHID::RateInfoList& rates,
		      const Ice::Current& current) const
{

    VALIDATE_SESSION;

    HaveRatesI::GetRates(rates, current);
    if (rates.empty())
    {
        const_cast<WorkI *>(this)->FixRates(rates, current);
    }
}

void WorkI::Delete(const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin
        && (my_session.mUserId != mUserId)) 
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }
    
    {
        WAR_AUTO_LOCK;
        if (WHID::WSINVOICED == mStatus)
        {
            WAR_ERRLOG;
            err_log << "Cannot delete work-item " << *this
                << ". This item is invoiced and likely referenced by the invoice."
                << war_endl;

            throw WHID::IntegrityErrorException();
        }

        WAR_NOTICELOG;
        notice_log << "Work-item " << *this << " is being deleted."
            << war_endl;

        try
        {

            WarDbConnection db;
            WhidEngine::GetEngine().GetDbPool().GetConnection(db);
            WarDbQuery query(db);

            query = "DELETE FROM Work "
                " WHERE Id = ", GetNativeKey();

            query.Exec();

            WarIceServant::Delete();
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
}

//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

void
WorkI::FixRates(::WHID::RateInfoList& rates,
		      const Ice::Current& current)
{
    WHID::RatePrx rate = GetMainRate(current);
    if (!rate)
    {
        WAR_ERRLOG;
        err_log << "I can't find any suitable rates for "
            << *this
            << ". Please configure the rates."
            << war_endl;

        throw WHID::InternalErrorException();
    }

    WHID::RateInfo r;
    rate->GetInfo(r, current.ctx);
    rates.push_back(r);
    SetRates(rates, current);
}


void WorkI::Charge(const WarTime& from, const WarTime& to, 
        WHID::RateData& rateData, 
        WHID::RateData& normalRateData,
        WarDbConnection& db,
        double& accumulatedNormalCharge,
        double& accumulatedTotalCharge,
        double& accumulatedCost,
        double& accumulatedProfit)
        throw(WarException)
{
    WarDbQuery query(db);

    double num_hours = 0, charge = 0, discount = 0, cost = 0, profit = 0;

    const int num_minutes = (int)((to.mTime - from.mTime) / 60000);
    num_hours = num_minutes / 60;
    num_hours += ((double)(num_minutes % 60)) * (1.0 / 60.0);

    if (rateData.mChargeType == WHID::RTCEXTRAFEE)
        charge = rateData.mChargePerHour;
    else if (rateData.mChargeType == WHID::RTCADDPERCENT)
    {
        charge = ((normalRateData.mChargePerHour 
            * rateData.mChargePerHour) / 100.0)
            * num_hours;
    }
    else
        charge = rateData.mChargePerHour * num_hours;

    charge = (int)charge;
    cost = rateData.mCost * num_hours;
    profit = charge - cost;

    accumulatedCost += cost;
    accumulatedProfit += profit;

    if (rateData.mChargeType == WHID::RCTABSOLUTE)
        accumulatedNormalCharge += charge;

    if (rateData.mChargeType != WHID::RTCEXTRAFEE)
        accumulatedTotalCharge += charge;

    query = "INSERT INTO Charge (UserId, ProjectId, CustomerId, TaskId, WorkId, "
        " RateId, NumHours, Charge, Discount, FromTime, ToTime, Cost, Profit) "
        " VALUES(", mUserId,
        ",", mProjectId,
        ",", mCustomerId,
        ",", mTaskId,
        ",", GetNativeKey(),
        ",", rateData.mId,
        ",", num_hours,
        ",", charge,
        ",", discount,
        ",", from,
        ",", to,
        ",", cost,
        ",", profit,
        ")";

    query.Exec();
}



void WorkI::Resync() throw(WarException)
{
    WarTime my_from, my_to;

    if (mFromTime.empty() || mToTime.empty())
        return; 

    my_from.AssignFromAnsiDateTime(mFromTime.c_str(), false, true);
    my_to.AssignFromAnsiDateTime(mToTime.c_str(), false, true);

    mMinutesUsed = (war_int32_t)
        (((my_to.mTime / 1000) - (my_from.mTime / 1000L)) / 60L);
    mMinutesUsed -= mMinutesPaused;
}

/////////////////////////////// PRIVATE    ///////////////////////////////////

