#include "libwhid.h"
#include "server/HaveRatesI.h"   // class implemented

#include <server/WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>

#include <server/RateI.h>
#include <ice/WarIceException.h>

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::HaveRatesPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const HaveRatesI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

HaveRatesI::HaveRatesI()
{
}// HaveRatesI

HaveRatesI::~HaveRatesI()
{
}// ~HaveRatesI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================

// Default implementation
void HaveRatesI::ValidateSetRatesAccess(const Ice::Current& current)
        throw(Ice::Exception)
{
    VALIDATE_SESSION_ADM;
}

//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

void
HaveRatesI::GetRates(::WHID::RateInfoList& rates,
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

       
        query = "SELECT DISTINCT R.Id, R.Name, R.ChargeType, R.Priority, R.Free, "
            " R.ChargePerHour, R.Active, R.Cost "
            " FROM Rates AS R"
            " JOIN ChargeBinding AS B ON B.ChargeId = R.Id "
            "   AND B.XrefType = ", GetRateXrefType(),
            "   AND B.XrefId = ", GetNativeKey(),
            " ORDER BY ChargeType, Priority, Name ";

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
        err_log << "Caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
HaveRatesI::SetRates(const ::WHID::RateInfoList& rates,
		      const Ice::Current& current)
{
    

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        
        // Delete all the current rates
        query = "DELETE FROM ChargeBinding "
            " WHERE XrefType = ", GetRateXrefType(),
            " AND XrefId = ", GetNativeKey();
        query.Exec();

        // Add the new ones
        WHID::RateInfoList::const_iterator rate;
        for(rate = rates.begin(); rate != rates.end(); rate++)
        {
            query = "INSERT INTO ChargeBinding "
                " (UserId, ChargeId, XrefType, XrefId) "
	       " VALUES (0, ", (const war_int64_t)rate->mRate->GetId(current.ctx),
                ",", GetRateXrefType(),
                ",", GetNativeKey(),
                ")";

            query.Exec();
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


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

