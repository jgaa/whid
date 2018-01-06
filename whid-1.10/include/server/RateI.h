
#ifndef RateI_h
#define RateI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */


/* FORWARD REFERENCES */


/** \class RateI
    \brief RateI
    \ingroup libwhid
*/
class  LIBWHID_API RateI : 
    public WarIceServant,
    public WHID::Rate
{
public:
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    RateI();
    
    /**
    * Destructor.
    */
    ~RateI();
    
    // OPERATORS
    // OPERATIONS

    virtual void Load () throw (WarException);
    virtual void Save () throw (WarException);

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t RateId,
        Ice::Identity& id); 

    static WHID::RatePrx GetProxy(const war_int64_t RateId);


    // CALLBACK
    // ICE METHODS

    virtual ::std::string GetName(const Ice::Current&) const;

    virtual void GetData(::WHID::RateData&,
			 const Ice::Current&) const;

    virtual void GetInfo(::WHID::RateInfo&,
			 const Ice::Current&) const;

    virtual void SetData(const ::WHID::RateData&,
			 const Ice::Current&);

    virtual ::std::string Explain(const Ice::Current&) const;

    virtual ::Ice::Long GetId(const Ice::Current&) const;

    // ACCESS
    // INQUIRY

protected:
    WHID::RateData mData;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::RatePrx& c);
WarLog& operator << (WarLog& o, const RateI& c);


/****************** END C++ spesific ******************/

#endif  /* RateI_h_ */

