
#ifndef HaveRatesI_h
#define HaveRatesI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */


/* FORWARD REFERENCES */


/** \class HaveRatesI
    \brief HaveRatesI
    \ingroup libwhid
*/
class  LIBWHID_API HaveRatesI : 
    virtual public WHID::HaveRates,
    virtual public WarIceServant
{
public:
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    HaveRatesI();
    
    /**
    * Destructor.
    */
    ~HaveRatesI();
    
    // OPERATORS
    // OPERATIONS

    // The derived object must deside
    // The default is VALIDATE_SESSION_ADM
    virtual void ValidateSetRatesAccess(const Ice::Current& current)
        throw(Ice::Exception);

    // CALLBACK
    // ICE METHODS

    virtual void GetRates(::WHID::RateInfoList&,
			  const Ice::Current&) const;

    virtual void SetRates(const ::WHID::RateInfoList&,
			  const Ice::Current&);

    // ACCESS
    // INQUIRY
    virtual war_int32_t GetRateXrefType() const = 0;

protected:
   
    
private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::HaveRatesPrx& c);
WarLog& operator << (WarLog& o, const HaveRatesI& c);


/****************** END C++ spesific ******************/

#endif  /* HaveRatesI_h_ */

