
#ifndef HaveChargesI_h
#define HaveChargesI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */


/* FORWARD REFERENCES */


/** \class HaveChargesI
    \brief HaveChargesI
    \ingroup libwhid
*/
class  LIBWHID_API HaveChargesI : 
    virtual public WHID::HaveCharges,
    virtual public WarIceServant
{
public:
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    HaveChargesI();
    
    /**
    * Destructor.
    */
    ~HaveChargesI();
    
    // OPERATORS
    // OPERATIONS
    // CALLBACK
    // ICE METHODS

    virtual void GetCharges(::WHID::WorkFilterE,
			    const ::WHID::UserPrx&,
			    ::WHID::ChargeDataList&,
			    const Ice::Current&) const;

    virtual void GetInvoices(const ::WHID::UserPrx&,
			     ::WHID::InvoiceInfoList&,
			     const Ice::Current&) const;

    virtual void GetWork(const ::WHID::UserPrx&,
			 ::WHID::WorkFilterE,
			 const ::std::string&,
			 const ::std::string&,
			 ::WHID::WorkInfoList&,
			 const Ice::Current&) const;

    // ACCESS
    // INQUIRY
    virtual war_ccstr_t GetChargeField() const = 0;
    virtual war_ccstr_t GetWorkField()  const { return GetChargeField(); }

protected:    
private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::HaveChargesPrx& c);
WarLog& operator << (WarLog& o, const HaveChargesI& c);


/****************** END C++ spesific ******************/

#endif  /* HaveChargesI_h_ */

