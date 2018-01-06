#include <libwhid.h>
#include "server/WhidEngine.h"   // class implemented
#include <war_util.h>
#include <database/WarDbConnection.h>
#include <database/WarDbStructureXml.h>
#include <database/WarDbQuery.h>
#include <WarShutdownEngine.h>

#include "server/CustomerI.h"
#include "server/ProjectI.h"
#include "server/TaskI.h"
#include "server/WorkI.h"
#include "server/RateI.h"
#include "server/InvoiceI.h"
#include "server/UserI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================

WhidEngine::WhidEngine()
{
    // These values may be overridden from the configuration-file
    mMinDbConnections = 2;
    mMaxDbConnections = 16;
    mDbType = "sqlite";
    mDbDatabase = "whid";
    mDbDatabaseDefFile = "whid_database.xml";
    mDbDir = "";
    WAR_ASSERT(NULL == mThis);
    mThis = this;
    mIsMultiuser = false;

}// WhidEngine

WhidEngine::~WhidEngine()
{
    //mWhidControl = NULL;
    //mLocatorPtr = NULL;
    mThis = NULL;
}// ~WhidEngine


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================

void WhidEngine::GetDbDefPath(WarPath<char>& path)
{
    path = mDbDir;
    
    if (!path.IsEmpty())
       path << WAR_SYSSLASH;
    path << mDbDatabaseDefFile;
}

void WhidEngine::Initialize() throw(WarException)
{
    if (mDbType == "sqlite")
    {
        mDbPath << mDbDatabase;
        if (!mDbPath.HaveExtension())
            mDbPath << ".db";
    }
}

void WhidEngine::CreateAndBootstrap(const WarUtf8 userName, 
                                    const WarUtf8 loginName,
                                    const double charge) 
                                    throw(WarException)
{
    WarDbConnection  db;
    db.Open(mDbType.c_str(), 
        mDbDatabase.c_str(),
        mDbHost.c_str(),
        mDbUser.c_str(),
        mDbPasswd.c_str());

    CreateAndBootstrap(userName, loginName, charge, db);
}

void WhidEngine::CreateAndBootstrap(const WarUtf8& userName, 
                                    const WarUtf8& loginName,
                                    const double& charge,
                                    WarDbConnection& db) 
                                    throw(WarException)
{
    // Get the database-definition
    WarDbStructureXml xml;
    WarDbStructure def;

    WarPath<char> path;
    GetDbDefPath(path);

    xml.LoadDatabaseDefinition(path.GetPath(), def);

    // Create the database
    db.GetDriver()->CreateTables(def);

    // Add the first user
    std::string descr = "Default user added when the database was made";
    WarDbQuery query(db);
    query = "INSERT INTO Users "
        " (LoginName, Name, Notes, Admin) ",
        " VALUES (", loginName.GetUnicode(), ",", userName.GetUnicode(), ",", descr, ", 1)";
    query.Exec();
    const war_int64_t user_id = query.GetLastInsertId();

    // Add his default charge
    query = "INSERT INTO Rates "
        " (Name, ChargePerHour) "
        " VALUES('Normal', ", charge, ")";
    query.Exec();
    const war_int64_t charge_id = query.GetLastInsertId();

    query = "INSERT INTO ChargeBinding (UserId, ChargeId, XrefType, XrefId) "
        " VALUES (", user_id,
        ",", charge_id,
        ", 0, 0)";
    query.Exec();

    // Add admin user, unless the user selected "Admin" as his name.
    if (stricmp("Admin", userName.GetUtf8().c_str()))
    {
        query = "INSERT INTO Users "
            " (LoginName, Name, Admin) ",
            " VALUES ('Admin', 'Admin', 1)";
        query.Exec();
    }
}

void WhidEngine::Open() throw(WarException)
{
    WAR_NOTICELOG;

    //if (mDbType == "mysql")
    {
        notice_log << "Upgrading the database." << war_endl;
        // Upgrade the database
        try
        {
            WarDbConnection  db;
            db.Open(mDbType.c_str(), 
                mDbDatabase.c_str(),
                mDbHost.c_str(),
                mDbUser.c_str(),
                mDbPasswd.c_str());

            WarDbStructureXml xml;
            WarDbStructure def;

	    WarPath<char> path;
            GetDbDefPath(path);

            xml.LoadDatabaseDefinition(path.GetPath(), def);
            db.GetDriver()->UpdateTables(def);
        }
        catch(const WarException& ex)
        {
            WAR_WARNLOG;
            warn_log << "Database-upgrade failed. "
                "I will ignore the problem and try to use the unmodified database. "
                << ex.what()
                << war_endl;
        }
    }

    notice_log << "Opening database \""
        << mDbDatabase
        << "\" of type \""
        << mDbType
        << "\"";
    
    if (!mDbHost.empty())
        notice_log << " on host \""
        << mDbHost
        << "\"";

    notice_log << "." << war_endl;

    mDbPool.Create(mDbType.c_str(),
        mDbDatabase.c_str(),
        mDbHost.c_str(),
        mDbUser.c_str(),
        mDbPasswd.c_str(),
        mMinDbConnections,
        mMaxDbConnections);
}

void WhidEngine::StartEngine(const bool waitForShutdown) 
throw(WarException)
{
    WAR_NOTICELOG;
    Open();

    RunInitialQueries();

    // Initialize Ice
    try
    {
        WarShutdownEngine::GetEngine().AddEvent(OnShutdown, this, "WhidEngine");

        // Start Ice
        notice_log << "Initializing the Ice RPC system." << war_endl;
        WarIceCore::Create(mIceAddress.c_str(), mIceArgs.c_str());
        //WarIceCore::Create(NULL, NULL);

        mLocatorPtr = new WarIceLocator(WAR_ICE_LOCATOR_DEF_CACHED_OBJS,
            WAR_ICE_LOCATOR_DEF_CACHED_SIZE, 1000, 0);

        mLocatorPtr->AddCreator("Customer", WAR_NEW_ICE_GENERIC_CREATOR(CustomerI));
        mLocatorPtr->AddCreator("Project", WAR_NEW_ICE_GENERIC_CREATOR(ProjectI));
        mLocatorPtr->AddCreator("Task", WAR_NEW_ICE_GENERIC_CREATOR(TaskI));
        mLocatorPtr->AddCreator("Work", WAR_NEW_ICE_GENERIC_CREATOR(WorkI));
        mLocatorPtr->AddCreator("Rate", WAR_NEW_ICE_GENERIC_CREATOR(RateI));
        mLocatorPtr->AddCreator("Invoice", WAR_NEW_ICE_GENERIC_CREATOR(InvoiceI));
        mLocatorPtr->AddCreator("User", WAR_NEW_ICE_GENERIC_CREATOR(UserI));
        
        //mLocatorPtr->AddCreator("", WAR_NEW_ICE_GENERIC_CREATOR(I));
        
        mAdapter->addServantLocator(mLocatorPtr.GetWrappedPtr(), "");

        mWhidControl = new WhidControlI;
        mWhidGateway = new GatewayI;
        if (mGatewayName.empty())
            mGatewayName = "WhidGateway";
        if (mControlName.empty())
            mControlName = "WhidControl";
        mAdapter->add(mWhidControl, 
            GetCommunicator().stringToIdentity(
            mControlName.c_str()));
        mAdapter->add(mWhidGateway, 
            GetCommunicator().stringToIdentity(
            mGatewayName.c_str()));

       UpdateMultiuser();

    }
    catch(const Ice::Exception& ex)
    {
        WarLog err_log(WARLOG_ERROR, "Server::Process()");

        err_log << "Failed to initialize Ice: "
            << ex
            << war_endl;

        mSkipShutdown = true; // Avoid bug

        WAR_THROW(WarError(WAR_ICE_SYSTEM_EXCEPTION), NULL);
    }

    try
    {
        // Start the adapter
        mAdapter->activate();

        notice_log << "Ready to process requests on gateway \""
            << mGatewayName
            << "\"." 
            << war_endl;

        if (!waitForShutdown)
            return;

        mIce->waitForShutdown();

        // Wait for other modules to shut down
        notice_log << "Ice services has stopped. "
            << "Waiting for other services to stop." 
            << war_endl;
        
        WarShutdownEngine::GetEngine().WaitForShutdown();

        // Clean up

        // We are now shut down.
        notice_log << "All services are stopped." << war_endl;
    }
    catch(const Ice::Exception& ex)
    {
        WarLog err_log(WARLOG_ERROR, "Server::Process");
        
        err_log << "Caught exception from Ice while running the service: "
            << ex
            << war_endl;
    }
}

void WhidEngine::UpdateMultiuser()
{
    try
    {
        WHID::Options opts;
        Ice::Current current;
        mWhidControl->GetOptions(opts, current);
        mIsMultiuser = opts.mMultiuser;
    }
    catch(...)
    {
        mIsMultiuser = false;
    }
}

void WhidEngine::Backup(const war_syspath_t& path) throw(WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);

    WAR_AUTO_LOCK;
    db.GetDriver()->ExportData(path);
}

void WhidEngine::Restore(const war_syspath_t& path) throw(WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);

    WAR_AUTO_LOCK;
    db.GetDriver()->ImportData(path);
}

//============================= CALLBACK   ===================================

void WhidEngine::OnShutdown(war_cptr_t arg)
{
    mThis->mWhidControl = NULL;
    mThis->mLocatorPtr = NULL;
}

//============================= ACCESS     ===================================

WarDbConnectionPool& WhidEngine::GetDbPool()
{
    return mDbPool;
}

WhidEngine& WhidEngine::GetEngine()
{
    WAR_ASSERT(NULL != mThis);
    return *mThis;
}

WarIceLocator& WhidEngine::GetLocator()
{
    return *mLocatorPtr;
}

WHID::WhidControlPrx WhidEngine::GetWhidControl()
{
     return WHID::WhidControlPrx::uncheckedCast(
        GetAdapter().createProxy(
        GetCommunicator().stringToIdentity(
        mControlName.c_str())));
}

WHID::GatewayPrx WhidEngine::GetWhidGateway()
{
    return WHID::GatewayPrx::uncheckedCast(
        GetAdapter().createProxy(
        GetCommunicator().stringToIdentity(
        mGatewayName.c_str())));
}

//============================= INQUIRY    ===================================

bool WhidEngine::HaveDatabase() const
{
#ifdef WAR_DEBUG
    WAR_NOTICELOG;
    notice_log << "Debugging is on" << war_endl;
#endif
    if (mDbType == "sqlite")
        return war_is_file(mDbPath.GetPath());
    else if (mDbType == "mysql")
    {
        // See if we can connect to the database
        try
        {
            WarDbConnection  db;
            db.Open(mDbType.c_str(), 
                mDbDatabase.c_str(),
                mDbHost.c_str(),
                mDbUser.c_str(),
                mDbPasswd.c_str());

            // See if we have tables
            WarDbQuery query(db);
            query = "SELECT COUNT(*) FROM Users ";
            query.Exec();
        }
        catch(WarException)
        {
            return false;
        }

        return true;
    }
    
    return false;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

void WhidEngine::RunInitialQueries() throw(WarException)
{
    WarDbConnection db;
    GetDbPool().GetConnection(db);
    WarDbQuery query(db);

    // Close all open work-items
    query = "UPDATE Work SET Status = ",
        (const war_int32_t)WHID::WSHOLDBACK,
        " WHERE Status = ";
        
    query += (const war_int32_t)WHID::WSACTIVE;
    query.Exec();

    // Force tasks with invalid status to valid values
    // TODO: Remove this before the BETA period ends!!
    query = "UPDATE Tasks SET ActiveStatus = 1 WHERE ActiveStatus = 0";
    query.Exec();

}

/////////////////////////////// PRIVATE    ///////////////////////////////////

WhidEngine *WhidEngine::mThis = NULL;


