
#ifndef XXI_h
#define XXI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */


/* FORWARD REFERENCES */


/** \class XXI
    \brief XXI
    \ingroup libwhid
*/
class  LIBWHID_API XXI : 
    public WarIceServant,
    public WHID::XX
{
public:
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    XXI();
    
    /**
    * Destructor.
    */
    ~XXI();
    
    // OPERATORS
    // OPERATIONS

    virtual void Load () throw (WarException);
    virtual void Save () throw (WarException);

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t XXId,
        Ice::Identity& id); 

    static WHID::XXPrx GetProxy(const war_int64_t XXId);


    // CALLBACK
    // ICE METHODS
    // ACCESS
    // INQUIRY

protected:
    
private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::XXPrx& c);
WarLog& operator << (WarLog& o, const XXI& c);


/****************** END C++ spesific ******************/

#endif  /* XXI_h_ */

