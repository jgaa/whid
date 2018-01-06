
#ifndef WhidEngine_h
#define WhidEngine_h

/* SYSTEM INCLUDES */

#include <WarPath.h>
#include <WarCriticalSection.h>
#include <database/WarDbConnectionPool.h>
#include <WarUtf8.h>
#include <ice/WarIceCore.h>
#include <ice/WarIceLocator.h>
#include <ice/WarIceException.h>

/* PROJECT INCLUDES */

#include "server/WhidControlI.h"
#include "server/GatewayI.h"

/* FORWARD REFERENCES */


/** \class WhidEngine
    \brief WhidEngine
    \ingroup libwhid
*/
class  LIBWHID_API WhidEngine :
    public WarIceCore
{
public:
    typedef WarPath<char> db_path_t;
    typedef IceUtil::Handle<WhidControlI> whid_control_t;
    typedef IceUtil::Handle<GatewayI> whid_gateway_t;

    // LIFECYCLE
    
    /**
    * Default constructor.
    */
     WhidEngine();
    
    /**
    * Destructor.
    */
    ~WhidEngine();
    
    // OPERATORS

    void GetDbDefPath(WarPath<char>& path);
    void Initialize() throw(WarException);
    void CreateAndBootstrap(const WarUtf8 userName, 
        const WarUtf8 loginName,
        const double charge) 
        throw(WarException);

    void CreateAndBootstrap(
        const WarUtf8& userName, 
        const WarUtf8& loginName,
        const double& charge,
        WarDbConnection& db)
        throw(WarException);

    void Open() throw(WarException);

    /** Start the Ice subsystem and initialize objects */
    void StartEngine(const bool waitForShutdown) 
       throw(WarException);

    void Backup(const war_syspath_t& path) throw(WarException);
    void Restore(const war_syspath_t& path) throw(WarException);

    // OPERATIONS
    // CALLBACK

    static void OnShutdown(war_cptr_t arg);

    // ACCESS

    WarDbConnectionPool& GetDbPool();
    
    static WhidEngine& GetEngine();

    WarIceLocator& GetLocator();

    WHID::WhidControlPrx GetWhidControl();

    WHID::GatewayPrx GetWhidGateway();

    void UpdateMultiuser();

    // INQUIRY

    bool HaveDatabase() const;

    bool IsMultiuser() const
    {
        return mIsMultiuser;
    }

    // Config-settings. Loaded from the applications .conf file
    std::string mDbType;
    std::string mDbHost;
    std::string mDbDatabase;
    std::string mDbUser;
    std::string mDbPasswd;
    std::string mIceAddress;
    std::string mIceArgs;
    std::string mGatewayName;
    std::string mControlName;
    std::string mDbDir;
    
    int mMinDbConnections;
    int mMaxDbConnections;

    std::string mDbDatabaseDefFile;
    
protected:
    void RunInitialQueries() throw(WarException);

private:
    war_locator_ptr_t mLocatorPtr;
    db_path_t mDbPath;
    WarDbConnectionPool mDbPool;
    WarCriticalSection mLock;
    static WhidEngine *mThis;
    whid_control_t mWhidControl;
    whid_gateway_t mWhidGateway;
    bool mIsMultiuser;
    
};

/* INLINE METHODS */

/* EXTERNAL REFERENCES */

/* TYPEDEFS */

/* OPERATORS */

#define IS_MULTIUSER WhidEngine::GetEngine().IsMultiuser()

/****************** END C++ spesific ******************/

#endif  /* WhidEngine_h_ */

