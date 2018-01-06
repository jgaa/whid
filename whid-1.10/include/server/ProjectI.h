
#ifndef ProjectI_h
#define ProjectI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */

#include "server/NodeI.h"


/* FORWARD REFERENCES */


/** \class ProjectI
    \brief ProjectI
    \ingroup libwhid
*/
class  LIBWHID_API ProjectI : 
    virtual public NodeI,
    virtual public WHID::Project
{
public:
    enum NodeColsE 
    { 
        C_CUSTOMERID = NodeI::C_INVALID,
        C_IS_ACTIVE 
    };
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    ProjectI();
    
    /**
    * Destructor.
    */
    ~ProjectI();
    
    // OPERATORS
    // OPERATIONS

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t ProjectId,
        Ice::Identity& id); 

    static WHID::ProjectPrx GetProxy(const war_int64_t ProjectId);


    // CALLBACK
    // ICE METHODS

    // Node overrides
    virtual ::WHID::NodePrx GetParent(const Ice::Current&);
    

    // Own methods
    virtual void GetTasks(bool,
			  ::WHID::TaskList&,
			  const Ice::Current&) const;

    virtual ::WHID::TaskPrx CreateTask(const ::WHID::NodeData&,
				       const Ice::Current&);

    virtual void RecalculatePending(const ::WHID::UserPrx&,
				    const ::std::string&,
				    const ::std::string&,
				    const Ice::Current&);

    virtual void ClearPending(const ::WHID::UserPrx&,
			      const Ice::Current&);

    virtual void GetAllPendingWorkItems(const ::WHID::UserPrx&,
					const ::std::string&,
					const ::std::string&,
					::WHID::WorkList&,
					const Ice::Current&) const;

    // ACCESS
    // INQUIRY

    virtual war_ccstr_t GetClassName() const  { return "Project"; }
    virtual war_ccstr_t GetTableName() const { return "Projects"; }
    virtual war_ccstr_t GetParentTableName() const { return "Customers"; }
    virtual war_ccstr_t GetChargeField() const { return "ProjectId"; }
    virtual war_ccstr_t GetChildTableName() const { return "Tasks"; }
    virtual WHID::NodeTypeE GetNodeType() const { return WHID::NODEPROJECT; }
    virtual war_int64_t GetParentId() const { return 0; }
    virtual war_int32_t GetRateXrefType() const { return 2;}
    virtual bool IsActive() const;
    
protected:
    virtual void GetChildrenClause(WarDbQuery& query) const throw (WarException);
    virtual void OnLoadAddJoins(WarDbQuery& query) throw (WarException);
    virtual void OnLoadAddToSelect(WarDbQuery& query) throw (WarException);
    virtual void OnLoadAddToWhere(WarDbQuery& query) throw (WarException);
    virtual void OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException);
    virtual void OnLoadAddToSet(WarDbQuery& query) throw (WarException);
    virtual void SetActive(const bool active);

    war_int64_t mCustomerId;
    bool mIsActive;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::ProjectPrx& c);
WarLog& operator << (WarLog& o, const ProjectI& c);


/****************** END C++ spesific ******************/

#endif  /* ProjectI_h_ */

