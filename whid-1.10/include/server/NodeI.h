
#ifndef NodeI_h
#define NodeI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>
#include <database/WarDbQuery.h>
#include <WarAutoLock.h>

/* PROJECT INCLUDES */

#include <server/HaveRatesI.h>
#include <server/HaveChargesI.h>

/* FORWARD REFERENCES */


#ifdef GetClassName
// Microsoft loves to pollute the root namespace
#  undef GetClassName
#endif

/** \class NodeI
    \brief NodeI
    \ingroup libwhid
*/
class  LIBWHID_API NodeI : 
    virtual public WarIceServant,
    virtual public WHID::Node,
    virtual public HaveRatesI,
    virtual public HaveChargesI
{
public:
    enum NodeColsE {C_NAME, C_NOTES, C_ACTIVESTATUS, C_INVALID};

    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    NodeI();
    
    /**
    * Destructor.
    */
    ~NodeI();
    
    // OPERATORS
    // OPERATIONS

    virtual void Load () throw (WarException);
    virtual void Save () throw (WarException);

    // CALLBACK
    // ICE METHODS

    virtual ::std::string GetName(const Ice::Current&) const;

    virtual void GetData(::WHID::NodeData&,
			 const Ice::Current&) const;

    virtual void SetData(const ::WHID::NodeData&,
			 const Ice::Current&);

    virtual ::std::string Explain(const Ice::Current&) const;

    virtual void GetChildren(bool,
        ::WHID::NodeInfoList&,
        const Ice::Current&) const;

    virtual ::WHID::NodeTypeE GetNodeType(const Ice::Current&);

    virtual bool IsActive(const Ice::Current&);

    virtual void UpdateActiveStatus(const Ice::Current&);


    // ACCESS
    // INQUIRY

    virtual war_ccstr_t GetClassName() const = 0;
    virtual war_ccstr_t GetTableName() const = 0;
    virtual war_ccstr_t GetParentTableName() const = 0;
    virtual war_ccstr_t GetChildTableName() const = 0;
    virtual WHID::NodeTypeE GetNodeType() const = 0;
    virtual war_int64_t GetParentId() const = 0;
    virtual bool IsActive() const = 0;

    

    
protected:
    virtual void GetChildrenClause(WarDbQuery& query) const throw (WarException) = 0;
    /** Override to add joins to the load query */
    virtual void OnLoadAddJoins(WarDbQuery& query) throw (WarException) = 0;
    /** Override to add columns to the SELECT clause of the load query */
    virtual void OnLoadAddToSelect(WarDbQuery& query) throw (WarException) = 0;
    /** Override to add conditions to the WHERE clause */
    virtual void OnLoadAddToWhere(WarDbQuery& query) throw (WarException) = 0;
    /** Override to fetch columns */
    virtual void OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException) = 0;
    /** Override to save **/
    virtual void OnLoadAddToSet(WarDbQuery& query) throw (WarException) {};

    // Internal method Called from UpdateActiveStatus(). No not call manually!!
    virtual void SetActive(const bool active) = 0;

    
    WHID::NodeData mData;

private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::NodePrx& c);
WarLog& operator << (WarLog& o, const NodeI& c);


/****************** END C++ spesific ******************/

#endif  /* NodeI_h_ */

