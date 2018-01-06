
#ifndef WorkI_h
#define WorkI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */

#include <server/HaveRatesI.h>
#include <server/HaveChargesI.h>

/* FORWARD REFERENCES */
class WarDbConnection;

/** \class WorkI
    \brief WorkI
    \ingroup libwhid
*/
class  LIBWHID_API WorkI : 
    virtual public WarIceServant,
    virtual public WHID::Work,
    virtual public HaveRatesI,
    virtual public HaveChargesI
{
public:
    enum ChargeTypsE
    {
        // Array-size for all charge-types (WHID::RateChargeTypeE)
        CHARGE_TYPES = WHID::RTCEXTRAFEE +1
    };

    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    WorkI();
    
    /**
    * Destructor.
    */
    ~WorkI();
    
    // OPERATORS
    // OPERATIONS

    virtual void Load () throw (WarException);
    virtual void Save () throw (WarException);

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t WorkId,
        Ice::Identity& id); 

    static WHID::WorkPrx GetProxy(const war_int64_t WorkId);

    virtual void ValidateSetRatesAccess(const Ice::Current& current)
        throw(Ice::Exception);


    // CALLBACK
    // ICE METHODS
    
    virtual ::WHID::TaskPrx GetParent(const Ice::Current&) const;

    virtual void SetData(const ::WHID::WorkData&,
			  const Ice::Current&);

    virtual void GetData(::WHID::WorkData&,
			 const Ice::Current&) const;

    virtual ::std::string Explain(const Ice::Current&) const;

    virtual ::WHID::WorkStatusE GetStatus(const Ice::Current&) const;

    virtual void SetStatus(::WHID::WorkStatusE,
			   const Ice::Current&);

    virtual void Delete(const Ice::Current&);

    virtual void CloseTimer(const Ice::Current&);

    virtual ::WHID::RatePrx GetMainRate(const Ice::Current&) const;

    virtual void AddPause(::Ice::Int,
			  const Ice::Current&);

    virtual void SetEndtimeToNow(const Ice::Current&);

    virtual void DeleteCharges(const Ice::Current&);

    virtual void CalculateCharges(const Ice::Current&);

    virtual void GetInfo(::WHID::WorkInfo&,
			 const Ice::Current&) const;

    // Overrides of parent classes
     virtual void GetRates(::WHID::RateInfoList&,
			  const Ice::Current&) const;

    // ACCESS
    // INQUIRY    
    virtual war_int32_t GetRateXrefType() const { return 4;}

protected:
    virtual war_ccstr_t GetChargeField() const { return "WorkId"; }

    // Make sure there are at least one rate assigned to the item.
    void FixRates(::WHID::RateInfoList&,
        const Ice::Current&);
    void Resync() throw(WarException);
    void Charge(const WarTime& from, const WarTime& to, 
        WHID::RateData& rateData, 
        WHID::RateData& normalRateData,
        WarDbConnection& db,
        double& accumulatedNormalCharge,
        double& accumulatedTotalCharge,
        double& accumulatedCost,
        double& accumulatedProfit)
        throw(WarException);

    war_int64_t mCustomerId;
    war_int64_t mProjectId;
    war_int64_t mTaskId;
    war_int64_t mInvoiceId;
    war_int64_t mUserId;
    std::string mNotes;
    std::string mFromTime;
    std::string mToTime;
    war_int32_t mMinutesPaused;
    war_int32_t mMinutesUsed;
    double mCharge;
    double mCost;
    double mProfit;
    WHID::WorkStatusE mStatus;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::WorkPrx& c);
WarLog& operator << (WarLog& o, const WorkI& c);


/****************** END C++ spesific ******************/

#endif  /* WorkI_h_ */

