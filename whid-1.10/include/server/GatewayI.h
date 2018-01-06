
#ifndef GatewayI_h
#define GatewayI_h

/* SYSTEM INCLUDES */

#include <ice/WarIceServant.h>

#include <WarCriticalSection.h>

/* PROJECT INCLUDES */


/* FORWARD REFERENCES */


#define VALIDATE_SESSION \
    GatewayI::session_t my_session;\
    const bool is_multiuser = GatewayI::IsMultiuser();\
    if (is_multiuser) \
        GatewayI::GetGateway().GetSession(my_session, current);\
    const bool is_admin = is_multiuser ? my_session.mAdmin : true; \
    if (is_admin) {;} /* compiler food */ \

#define VALIDATE_SESSION_ADM \
    VALIDATE_SESSION;\
    if (!is_admin)\
    {\
        WAR_NOTICELOG;\
        notice_log << *this << " failed VALIDATE_SESSION_ADM" << war_endl;\
        throw WHID::AccessDeniedException();\
    }

// For UserI class only. Must be same user or admin to get access
#define VALIDATE_SESSION_USER \
    VALIDATE_SESSION;\
    if (!is_admin\
        && (my_session.mUserId != atoi(GetNativeKey().c_str()))) \
    {\
        WAR_NOTICELOG;\
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;\
        throw WHID::AccessDeniedException();\
    }

// For methods with forUser. Must be same user or admin to get access
#define VALIDATE_SESSION_FOR_USER \
    VALIDATE_SESSION;\
    if (!is_admin\
        && (my_session.mUserId != forUser->GetId(current.ctx))) \
    {\
        WAR_NOTICELOG;\
        notice_log << *this << " failed VALIDATE_SESSION_FOR_USER" << war_endl;\
        throw WHID::AccessDeniedException();\
    }



/** \class GatewayI
    \brief GatewayI
    \ingroup libwhid
*/
class  LIBWHID_API GatewayI : 
    public WHID::Gateway
{
public:
    enum ExpireE { DefaultExpire = (60*60) }; // 1 hour

    typedef struct session_def
    {
        war_int64_t mUserId;
        std::string mUserName;
        std::string mCookie;
        std::string mRandom;
        bool mAdmin;
        WarTime mLoginTime;
        WarTime mExpire;
    } session_t;

    typedef std::map<int, session_t> session_map_t;

    // LIFECYCLE
    
    /**
    * Default constructor.
    */
    GatewayI();
    
    /**
    * Destructor.
    */
    ~GatewayI();
    
    // OPERATORS
    // OPERATIONS

    // CALLBACK
    // ICE METHODS

    virtual void Login(const ::std::string&,
		       const ::std::string&,
		       ::WHID::Session&,
		       const Ice::Current&);

    virtual void Logout(const ::WHID::Session&,
			const Ice::Current&);

    virtual ::std::string Explain(const Ice::Current&) const;

    virtual void GetUserFromSession(const ::WHID::Session&,
				    ::WHID::UserPrx&,
				    const Ice::Current&);

    // Get the session. 
    // Throws AccessDeniedException if the session don't exist
    void GetSession(const ::WHID::Session& ses, 
        GatewayI::session_t& session, 
        const Ice::Current& cur)
        throw (WHID::AccessDeniedException);

    // Get session from cur ("cookie" variable.)
    void GetSession(GatewayI::session_t& session, 
        const Ice::Current& cur)
        throw (WHID::AccessDeniedException);

    // ACCESS

    static GatewayI& GetGateway()
    {
        WAR_ASSERT(NULL != mpThis);
        return *mpThis;
    }

    // INQUIRY

    virtual war_ccstr_t GetClassName() const  { return "Gateway"; }

    /** Wrapper to avoid recursice dependencies with WhidEngibe */
    static bool IsMultiuser();

protected:
    void CreateSession(war_int64_t userId,
        const std::string& name,
        const bool admin,
        ::WHID::Session& ses)
        throw(WarException);
    
    
    
private:
    WarCriticalSection mLock;
    session_map_t mSessions;
    war_int32_t mNextSessionId;
    static GatewayI *mpThis;
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

WarLog& operator << (WarLog& o, const WHID::GatewayPrx& c);
WarLog& operator << (WarLog& o, const GatewayI& c);


/****************** END C++ spesific ******************/

#endif  /* GatewayI_h_ */

