
#ifndef WhidControlI_h
#define WhidControlI_h

/* SYSTEM INCLUDES */

#include <WarTime.h>

/* PROJECT INCLUDES */


/* FORWARD REFERENCES */


/** \class WhidControlI
    \brief WhidControlI
    \ingroup libwhid
*/
class  LIBWHID_API WhidControlI : 
    public WHID::WhidControl
{
public:
    typedef std::vector<std::string> option_names_t;
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    WhidControlI();
    
    /**
    * Destructor.
    */
    ~WhidControlI();
    
    // OPERATORS
    // OPERATIONS
    // CALLBACK
    // ICE METHODS

    // Inherited from Node

    virtual ::std::string Explain(const Ice::Current&) const;

    // Own

    virtual ::WHID::CustomerPrx CreateCustomer(const ::WHID::NodeData&,
					       const Ice::Current&);

    virtual void GetCustomers(::WHID::CustFilterE,
			      const ::std::string&,
			      bool,
			      ::WHID::NodeInfoList&,
			      const Ice::Current&) const;

    virtual void GetRates(bool,
			  ::WHID::RateInfoList&,
			  const Ice::Current&) const;

    virtual ::WHID::RatePrx AddRate(const ::WHID::RateData&,
				    const Ice::Current&);

    virtual void GetOptions(::WHID::Options&,
			    const Ice::Current&) const;

    virtual void SetOptions(const ::WHID::Options&,
			    const Ice::Current&);

    virtual void GetUser(::Ice::Long,
			 ::WHID::UserPrx&,
			 const Ice::Current&) const;

    virtual void GetUserList(::WHID::UserInfoList&,
			     const Ice::Current&) const;

    virtual void AddUser(const ::WHID::UserData&,
			 ::WHID::UserPrx&,
			 const Ice::Current&);


    // ACCESS
    // INQUIRY    
protected:
   

    WarCriticalSection mLock;
    
    option_names_t mOptionNames;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::WhidControlPrx& c);
WarLog& operator << (WarLog& o, const WhidControlI& c);


/****************** END C++ spesific ******************/

#endif  /* WhidControlI_h_ */

