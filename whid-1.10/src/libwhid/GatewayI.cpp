#include "libwhid.h"
#include "server/GatewayI.h"   // class implemented

#include <IceUtil/UUID.h>
#include <server/WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>
#include <WarMd5.h>
#include <WarAutoLock.h>
#include "server/UserI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::GatewayPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const GatewayI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

GatewayI::GatewayI()
:mNextSessionId(1)
{
    WAR_ASSERT(NULL == mpThis);
    mpThis = this;
}// GatewayI

GatewayI::~GatewayI()
{
    mpThis = NULL;
}// ~GatewayI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================




//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

::std::string
GatewayI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    {
        msg << GetClassName();
    }

    return msg.str();
}

void
GatewayI::Login(const ::std::string& logonName,
		      const ::std::string& password,
		      ::WHID::Session& ses,
		      const Ice::Current& current)
{
    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t row;

        enum ColsE {C_ID, C_ACTIVE, C_ADMIN, C_PASSWORD, C_NAME};
        query = "SELECT Id, Active, Admin, Password, Name "
            " FROM Users "
            " WHERE LoginName LIKE ", logonName;

        query.Exec(row);
        if (row->IsEof())
        {
            WAR_AUTHLOG;
            auth_log << "User \"" << logonName 
                << "\" tried to log on, but I can't find any users with that name."
                << war_endl;

            throw WHID::AccessDeniedException();
        }

        bool db_active = false;
        row->Get(C_ACTIVE, db_active);
        bool db_admin = false;
        row->Get(C_ADMIN, db_admin);
        std::string db_passwd;
        row->Get(C_PASSWORD, db_passwd);
        if (!db_active)
        {
            WAR_AUTHLOG;
            auth_log << "User \"" << logonName 
                << "\" tried to log on, but this user is disabled."
                << war_endl;

            throw WHID::AccessDeniedException();
        }

        if (db_passwd != password)
        {
            WAR_AUTHLOG;
            auth_log << "User \"" << logonName 
                << "\" tried to log on, but the password is wrong."
                << war_endl;

            throw WHID::AccessDeniedException();
        }

        WAR_AUTHLOG;
        auth_log << "User \"" << logonName 
            << "\" logged on."
            << war_endl;

        war_int64_t db_id = 0;
        row->Get(C_ID, db_id);
        std::string db_name;

        {
            // Clear the password-buffer
            std::string::iterator p;
            for(p = db_passwd.begin(); p != db_passwd.end(); p++)
                *p = ' ';
        }

        row->Get(C_NAME, db_name);
        return CreateSession(db_id, db_name, db_admin, ses);
        
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
GatewayI::Logout(const ::WHID::Session& ses,
		       const Ice::Current& current)
{
    {
        WAR_AUTO_LOCK;
        session_map_t::iterator P = mSessions.find(ses.mSessionKey);
        if (P == mSessions.end())
        {
            return; // Nothing to do
        }

        mSessions.erase(P);
    }

    WAR_NOTICELOG;
    notice_log << "Logging out session #" 
        << ses.mSessionKey
        << war_endl;

}

void
GatewayI::GetUserFromSession(const ::WHID::Session& ses,
				   ::WHID::UserPrx& usr,
				   const Ice::Current& current)
{
    session_t session;
    GetSession(ses, session, current);

    usr = UserI::GetProxy(session.mUserId);
}


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================

bool GatewayI::IsMultiuser()
{
    return IS_MULTIUSER;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

void GatewayI::CreateSession(war_int64_t userId,
                                     const std::string& name,
                                     const bool admin,
                                     ::WHID::Session& ses)
                                     throw(WarException)
{
    session_t session;
    session.mUserId = userId;
    session.mUserName = name;
    session.mRandom = IceUtil::generateUUID();
    session.mAdmin = admin;

    // make a cookie. We don't really need this until we make a more
    // advanved authentication-protocol...
    std::stringstream cookie_stream;
    cookie_stream << userId << '/' << session.mRandom;
    std::string cookie = cookie_stream.str();
    WarMd5 md5;
    md5.GetDigest(cookie, session.mCookie);

    session.mExpire.Reset(DefaultExpire * 1000);

    {
        WAR_AUTO_LOCK;
        ses.mSessionKey = mNextSessionId++;
        ses.mCookie = session.mCookie;
        mSessions.insert(session_map_t::value_type(ses.mSessionKey, session));
    }

    WAR_DEBUGLOG;
    debug_log << "Created session #" 
        << ses.mSessionKey
        << " with cookie \"" 
        << ses.mCookie
        << " for user #"
        << userId
        << " \""
        << name
        << "\"."
        << war_endl;
}

void GatewayI::GetSession(const ::WHID::Session& ses, 
                          GatewayI::session_t& session, 
                          const Ice::Current& cur)
                          throw (WHID::AccessDeniedException)
{
    WAR_AUTO_LOCK;
    session_map_t::const_iterator P = mSessions.find(ses.mSessionKey);
    if (P == mSessions.end())
    {
        WAR_NOTICELOG;
        notice_log << "Could not find session #" 
            << ses.mSessionKey
            << war_endl;
    }
    else
    {
        if (P->second.mCookie != ses.mCookie)
        {
            WAR_SECURITYLOG;
            security_log << "Invalid session cookie for session #"
                << ses.mSessionKey
                << " for Current" 
                << cur
                << war_endl;
        }
        else
        {
            // Session-key is OK
            session = P->second;
            return;
        }
    }

    throw WHID::AccessDeniedException();
}

void GatewayI::GetSession(GatewayI::session_t& session, 
        const Ice::Current& current)
        throw (WHID::AccessDeniedException)
 {
    ::WHID::Session ses;
    Ice::Context::const_iterator P = current.ctx.find("ses_key");
    if (P == current.ctx.end())
    {
        WarLog notice(WARLOG_NOTICE, "Server::GetSession()");
        notice << "No \"ses_key\" variable set by \""
            << current
            << "\"."
            << war_endl;

        throw WHID::AccessDeniedException();
    }

    std::string key = P->second;
    ses.mSessionKey = atoi(key.c_str());

    P = current.ctx.find("ses_cookie");
    if (P == current.ctx.end())
    {
        WarLog notice(WARLOG_NOTICE, "Server::GetSession()");
        notice << "No \"ses_key\" variable set by \""
            << current
            << "\"."
            << war_endl;

        throw WHID::AccessDeniedException();
    }

    ses.mCookie = P->second;

    GetSession(ses, session, current);
 }

/////////////////////////////// PRIVATE    ///////////////////////////////////

GatewayI *GatewayI::mpThis = NULL;

