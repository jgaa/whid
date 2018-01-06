
#ifndef InvoiceI_h
#define InvoiceI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */

#include "server/HaveChargesI.h"

/* FORWARD REFERENCES */


/** \class InvoiceI
    \brief InvoiceI
    \ingroup libwhid
*/
class  LIBWHID_API InvoiceI : 
    virtual public WarIceServant,
    virtual public WHID::Invoice,
    virtual public HaveChargesI
{
public:
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    InvoiceI();
    
    /**
    * Destructor.
    */
    ~InvoiceI();
    
    // OPERATORS
    // OPERATIONS

    virtual void Load () throw (WarException);
    virtual void Save () throw (WarException);

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t InvoiceId,
        Ice::Identity& id); 

    static WHID::InvoicePrx GetProxy(const war_int64_t InvoiceId);


    // CALLBACK
    // ICE METHODS

    // Inherited
    virtual ::std::string Explain(const Ice::Current&) const;

    // Own
    virtual void Commit(const Ice::Current&);

    virtual void GetInfo(::WHID::InvoiceInfo&,
			 const Ice::Current&) const;

    virtual void GetAsXml(::std::string&,
			  ::Ice::Double&,
			  ::Ice::Double&,
			  ::Ice::Double&,
			  ::Ice::Double&,
			  const Ice::Current&);

    // ACCESS
    // INQUIRY
    virtual war_ccstr_t GetChargeField() const { return "InvoiceId"; }

protected:


    war_int64_t mUserId;
    war_int64_t mCustomerId;
    double mCharge;
    double mCost;
    double mProfit;
    double mDiscount;
    std::string mInvoiceDate;
    double mHours;
    bool mFrozen;
    
private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::InvoicePrx& c);
WarLog& operator << (WarLog& o, const InvoiceI& c);


/****************** END C++ spesific ******************/

#endif  /* InvoiceI_h_ */

