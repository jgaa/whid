
#ifndef CustomerI_h
#define CustomerI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */

#include "server/NodeI.h"

/* FORWARD REFERENCES */


/** \class CustomerI
    \brief CustomerI
    \ingroup libwhid
*/
class  LIBWHID_API CustomerI : 
    virtual public NodeI,
    public WHID::Customer
{
public:
    enum NodeColsE { C_IS_ACTIVE = NodeI::C_INVALID, C_EMAIL };
        
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    CustomerI();
    
    /**
    * Destructor.
    */
    ~CustomerI();
    
    // OPERATORS
    // OPERATIONS

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t CustomerId,
        Ice::Identity& id); 

    static WHID::CustomerPrx GetProxy(const war_int64_t CustomerId);


    // CALLBACK
    // ICE METHODS

    // Node overrides
    virtual ::WHID::NodePrx GetParent(const Ice::Current&);

    // Own methods
    virtual void GetProjects(bool,
			     ::WHID::ProjectList&,
			     const Ice::Current&) const;

    virtual void GetCustomerData(::WHID::CustomerData&,
				 const Ice::Current&) const;

    virtual void SetCustomerData(const ::WHID::CustomerData&,
				 const Ice::Current&);

    virtual ::WHID::ProjectPrx CreateProject(const ::WHID::NodeData&,
					     const Ice::Current&);

    virtual ::WHID::InvoicePrx MakeInvoice(const ::WHID::ProjectList&,
					   const ::WHID::UserPrx&,
					   const Ice::Current&);
    

    // ACCESS
    // INQUIRY

    virtual war_ccstr_t GetClassName() const  { return "Customer"; }
    virtual war_ccstr_t GetTableName() const { return "Customers"; }
    virtual war_ccstr_t GetParentTableName() const { return ""; }
    virtual war_ccstr_t GetChildTableName() const { return "Projects"; }
    virtual war_ccstr_t GetChargeField() const { return "CustomerId"; }
    virtual WHID::NodeTypeE GetNodeType() const { return WHID::NODECUSTOMER; }
    virtual war_int64_t GetParentId() const { return 0; }
    virtual war_int32_t GetRateXrefType() const { return 1;}
    virtual bool IsActive() const;

protected:
    virtual void GetChildrenClause(WarDbQuery& query) const throw (WarException);
    virtual void OnLoadAddJoins(WarDbQuery& query) throw (WarException);
    virtual void OnLoadAddToSelect(WarDbQuery& query) throw (WarException);
    virtual void OnLoadAddToWhere(WarDbQuery& query) throw (WarException);
    virtual void OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException);
    virtual void OnLoadAddToSet(WarDbQuery& query) throw (WarException);
    virtual void SetActive(const bool active);

    WHID::CustomerData mCustomerData;
    bool mIsActive;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::CustomerPrx& c);
WarLog& operator << (WarLog& o, const CustomerI& c);


/****************** END C++ spesific ******************/

#endif  /* CustomerI_h_ */

