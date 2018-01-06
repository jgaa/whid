
#ifndef UserI_h
#define UserI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

/* PROJECT INCLUDES */

#include <server/HaveChargesI.h>

/* FORWARD REFERENCES */


/** \class UserI
    \brief UserI
    \ingroup libwhid
*/
class  LIBWHID_API UserI : 
    virtual public WarIceServant,
    virtual public WHID::User,
    virtual public HaveChargesI
{
public:
    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    UserI();
    
    /**
    * Destructor.
    */
    ~UserI();
    
    // OPERATORS
    // OPERATIONS

    virtual void Load () throw (WarException);
    virtual void Save () throw (WarException);

    /** Create a Ice::Id for the object */
    static void MakeId(const war_int64_t UserId,
        Ice::Identity& id); 

    static WHID::UserPrx GetProxy(const war_int64_t UserId);


    // CALLBACK
    // ICE METHODS
    
    virtual ::Ice::Long GetId(const Ice::Current&) const;

    virtual ::std::string GetName(const Ice::Current&) const;

    virtual void GetData(::WHID::UserData&,
			 const Ice::Current&) const;

    virtual void GetInfo(::WHID::UserInfo&,
			 const Ice::Current&) const;

    virtual void SetData(const ::WHID::UserData&,
			 const Ice::Current&);

    virtual void SetPassword(const ::std::string&,
			     const Ice::Current&);

    virtual ::std::string Explain(const Ice::Current&) const;

    virtual void GetProjectSummary(::WHID::ProjectSummaryModeE,
				   const ::std::string&,
				   ::WHID::ProjectSummaryList&,
				   const Ice::Current&) const;

    virtual void GetProjectsForBilling(const ::std::string&,
				       const ::std::string&,
				       ::WHID::ProjectForBillingInfoList&,
				       const Ice::Current&) const;

    virtual void GetProjectsReadyForInvoicing(::WHID::ProjectWithChargeForBillingInfoList&,
					      const Ice::Current&) const;

    virtual void ClearAllChargesNotInvoiced(const Ice::Current&);

    virtual void MakeInvoices(::WHID::InvoiceInfoList&,
			      const Ice::Current&);

    virtual void GetInvoicesAsXml(const ::WHID::InvoiceList&,
				  ::std::string&,
				  const Ice::Current&) const;

    /*virtual void GetInvoices(::WHID::InvoiceInfoList&,
			     const Ice::Current&) const;*/

    // ACCESS
    // INQUIRY

    // PROTECTED
protected:
    void GetProjectSummaryWeek(const WarTime& fromDate ,
        const WarTime& toDate,
        ::WHID::ProjectSummaryList& result,
        const Ice::Current& current) const;

    void GetWeekDates(const std::string& anyDate, 
        WarTime& fromDate, 
        WarTime& toDate) const
        throw(WarException, Ice::Exception);

    virtual war_ccstr_t GetChargeField() const { return "UserId"; };

protected:
    std::string mName;
    std::string mLoginName;
    std::string mPassword;
    std::string mNotes;
    std::string mEmail;
    bool mActive;
    bool mAdmin;
    
private:
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::UserPrx& c);
WarLog& operator << (WarLog& o, const UserI& c);


/****************** END C++ spesific ******************/

#endif  /* UserI_h_ */

