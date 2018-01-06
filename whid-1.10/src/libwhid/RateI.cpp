#include "libwhid.h"
#include "server/RateI.h"   // class implemented

#include <server/WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::RatePrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const RateI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

RateI::RateI()
{
}// RateI

RateI::~RateI()
{
}// ~RateI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void RateI::Load () throw (WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t row;

    enum ColsE 
    { 
        C_ID,
        C_NAME, C_CHARGEPERHOUR, C_CHARGETYPE, C_PRIORITY, C_FROMCLOCK, 
        C_TOCLOCK, C_NUMCONTINOUSHOURS, C_MAXTIMEBETWEENCONTINOUSINTERVALS, 
        C_APPLIESTODAYMASK, C_MINTIME, C_ALIGN, C_ALIGNTHRESHOLD, C_FREE,
        C_ACTIVE, C_NOTES, C_ALIGN_START, C_COST
    };
    query = "SELECT "
        " Id, Name, ChargePerHour, ChargeType, Priority, FromClock, "
        " ToClock, NumContinousHours, MaxTimeBetweenContinousIntervals, "
        " AppliesToDayMask, MinTime, Align, AlignThreshold, Free, "
        " Active, Notes, AlignStart, Cost "
        " FROM Rates "
        " WHERE Id = ", GetNativeKey();

    query.Exec(row);
    if (row->IsEof())
        WAR_THROW_NOT_FOUND("Category not found");

    row->Get(C_ID, mData.mId);
    row->Get(C_NAME, mData.mName);
    row->Get(C_CHARGEPERHOUR, mData.mChargePerHour);
    mData.mChargeType = (WHID::RateChargeTypeE)row->GetInt32(C_CHARGETYPE);
    row->Get(C_PRIORITY, mData.mPriority);
    row->Get(C_FROMCLOCK, mData.mFromClock);
    row->Get(C_TOCLOCK, mData.mToClock);
    row->Get(C_NUMCONTINOUSHOURS, mData.mNumContinousHours);
    row->Get(C_MAXTIMEBETWEENCONTINOUSINTERVALS, mData.mMaxTimeBetweenContinousIntervals);
    row->Get(C_APPLIESTODAYMASK, mData.mAppliesToDayMask);
    row->Get(C_MINTIME, mData.mMinTime);
    row->Get(C_ALIGN, mData.mAlign);
    row->Get(C_ALIGN_START, mData.mAlignStart);
    row->Get(C_ALIGNTHRESHOLD, mData.mAlignThreshold);
    row->Get(C_FREE, mData.mFree);
    row->Get(C_ACTIVE, mData.mActive);
    row->Get(C_NOTES, mData.mNotes);
    row->Get(C_COST, mData.mCost);
    
    mDirty = 0;
}

void RateI::Save () throw (WarException)
{
    if (mDirty)
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

#define ADD(name) ", " #name " = ", mData.m ##name

        query = "UPDATE Rates SET Name = ", mData.mName,
            ADD(ChargePerHour),
            ADD(ChargeType),
            ADD(Priority),
            ADD(FromClock),
            ADD(ToClock),
            ADD(NumContinousHours),
            ADD(MaxTimeBetweenContinousIntervals),
            ADD(AppliesToDayMask),
            ADD(MinTime),
            ADD(Align),
            ADD(AlignStart),
            ADD(AlignThreshold),
            ADD(Free),
            ADD(Active),
            ADD(Notes),
            ADD(Cost),
            " WHERE Id = ", GetNativeKey();
#undef ADD

        query.Exec();
    }
}

void RateI::MakeId(const war_int64_t RateId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "Rate", RateId);
}

WHID::RatePrx RateI::GetProxy(const war_int64_t RateId)
{
    Ice::Identity id;
    MakeId(RateId, id);

    return WHID::RatePrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================


::Ice::Long
RateI::GetId(const Ice::Current& current) const
{
    VALIDATE_SESSION;
    return WarAtoi<war_int32_t>(GetNativeKey().c_str());
}


::std::string
RateI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    {
        WAR_AUTO_LOCK;
        msg << "Rate \"" << mData.mName << "\"";
    }

    return msg.str();
}

::std::string
RateI::GetName(const Ice::Current& current) const
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
RateI::GetData(::WHID::RateData& data,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;
    {
        WAR_AUTO_LOCK;
        data = mData;
    }
}

void
RateI::GetInfo(::WHID::RateInfo& info,
		     const Ice::Current& current) const
{
    VALIDATE_SESSION;
    WAR_AUTO_LOCK;
    {
        info.mName = mData.mName;
        info.mChargeType = mData.mChargeType;
        info.mPriority = mData.mPriority;
        info.mFree = mData.mFree;
        info.mCost = mData.mCost;
        info.mChargePerHour = mData.mChargePerHour;
        info.mActive = mData.mActive;        
    }

    info.mRate = RateI::GetProxy(atoi(GetNativeKey().c_str()));
}

void
RateI::SetData(const ::WHID::RateData& data,
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

            query = "SELECT COUNT(*) FROM Rates ",
                " WHERE Name = ", data.mName;

            query.Exec(row);
            if (row->GetInt32(0) > 0)
            {
                WAR_NOTICELOG;
                notice_log << "A Rate \""
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

    {
        WAR_AUTO_LOCK;
        mData = data;
        Flush(-1);
    }
}


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

