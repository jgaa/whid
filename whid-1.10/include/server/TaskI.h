
#ifndef TaskI_h
#define TaskI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */

#include "server/NodeI.h"


/* FORWARD REFERENCES */


/** \class TaskI
    \brief TaskI
    \ingroup libwhid
*/
class  LIBWHID_API TaskI : 
    virtual public NodeI,
    public WHID::Task
{
public:
    enum NodeColsE { C_CUSTOMERID = NodeI::C_INVALID, C_PROJECTID };

    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    TaskI();
    
    /**
    * Destructor.
    */
    ~TaskI();
    
    // OPERATORS
    // OPERATIONS

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t TaskId,
        Ice::Identity& id); 

    static WHID::TaskPrx GetProxy(const war_int64_t TaskId);


    // CALLBACK
    // ICE METHODS

    // Node overrides
    virtual ::WHID::NodePrx GetParent(const Ice::Current&);

    virtual void GetChildren(::WHID::NodeInfoList&,
        const Ice::Current&) const;

    // Own methods

    virtual ::WHID::WorkPrx CreateWork(const ::WHID::UserPrx&,
				       const ::WHID::WorkData&,
				       const Ice::Current&);

    virtual ::WHID::WorkPrx StartTimer(const ::WHID::UserPrx&,
				       const Ice::Current&);

    virtual void GetDefaultRates(const ::WHID::UserPrx&,
				 ::WHID::RateInfoList&,
				 const Ice::Current&) const;

    // ACCESS
    // INQUIRY

    virtual war_ccstr_t GetClassName() const  { return "Task"; }
    virtual war_ccstr_t GetTableName() const { return "Tasks"; }
    virtual war_ccstr_t GetParentTableName() const { return "Projects"; }
    virtual war_ccstr_t GetChargeField() const { return "TaskId"; }
    virtual war_ccstr_t GetChildTableName() const { return ""; }
    virtual WHID::NodeTypeE GetNodeType() const { return WHID::NODETASK; }
    virtual war_int64_t GetParentId() const { return 0; }
    virtual war_int32_t GetRateXrefType() const { return 3;}
    virtual bool IsActive() const;
    
protected:
    virtual void GetChildrenClause(WarDbQuery& query) const throw (WarException);
    virtual void OnLoadAddJoins(WarDbQuery& query) throw (WarException);
    virtual void OnLoadAddToSelect(WarDbQuery& query) throw (WarException);
    virtual void OnLoadAddToWhere(WarDbQuery& query) throw (WarException);
    virtual void OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException);
    virtual void SetActive(const bool active);

    war_int64_t mCustomerId;
    war_int64_t mProjectId;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::TaskPrx& c);
WarLog& operator << (WarLog& o, const TaskI& c);


/****************** END C++ spesific ******************/

#endif  /* TaskI_h_ */

