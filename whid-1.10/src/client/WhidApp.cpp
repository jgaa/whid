// /////////////////////////////////////////////////////////////////////////////
// Name:        whid.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     15/12/2006 14:08:19
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WhidApp.h"
#endif

#include "libwhid.h"
#include <war_util.h>
#include <WarConfigFile.h>
#include <WarLogHandlerFile.h>
#include <database/WarDbQuery.h>
#include <WarLogEngine.h>
#include <WarUtf8.h>
#include <WarShutdownEngine.h>
#include <WarSmtpClient.h>
#include <war_getopt.h>

#include "MainFrame.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

#include "WhidApp.h"
#include <wxwidgets/WarWxLogProxy.h>
#include "FirstUseDlg.h"

////@begin implement app
IMPLEMENT_APP( WhidApp )
////@end implement app

/*!
 * WhidApp type definition
 */

IMPLEMENT_CLASS( WhidApp, wxApp )

/*!
 * WhidApp event table definition
 */

BEGIN_EVENT_TABLE( WhidApp, wxApp )

////@begin WhidApp event table entries
////@end WhidApp event table entries

END_EVENT_TABLE()


const WhidApp::lng_defs WhidApp::mLanguages[4] = 
{
    { wxT("System Default"),  wxLANGUAGE_DEFAULT },
    { wxT("English"), wxLANGUAGE_ENGLISH },
    { wxT("Norwegian"), wxLANGUAGE_NORWEGIAN_BOKMAL },
    { NULL, 0 }
};

bool debug_flag = false;
std::string debug_events;

/*!
 * Constructor for WhidApp
 
 */

WhidApp::WhidApp()
: mResources(WarCorelibResources::WITH_THREAD_ENGINE
             |WarCorelibResources ::WITH_SHUTDOWN_ENGINE
             |WarCorelibResources ::WITH_TIMER_ENGINE),
             mpLocale(NULL)
{
    
    /*!
    * Member initialisation 
    */
    
    mLngDir = WHID_LNG_DIR;
    mDbDir = WHID_DB_DIR;
    mIconDir = WHID_ICON_DIR;
    mXmlDir = WHID_XML_DIR;
}


void WhidApp::Usage()
{
    WarPath<char> prg = wxApp::argv[0];
    WarPath<char> filename = prg.GetFilename();

    std::cout << "Usage: " << filename.GetPath() << " [options] " << std::endl
        << "Options:" << std::endl
        << "   -h --help             Show this help" << std::endl
        << "   -v --version          Print version" << std::endl
        << "   -d --debug            Show debug messages" << std::endl
        << "      --icon-dir=dir     Specify the icon directory" << std::endl
        << "      --db-dir=dir       Specify the database definitin directory" << std::endl
        << "      --lng-dir=dir      Specify the language directory" << std::endl
        << "      --xml-dir=dir      Specify the XML directory" << std::endl
#ifdef DEBUG
        << "      --devel            Use directories relative to src/client" << std::endl
#endif
        << "";
}

bool WhidApp::OnInit()
{        
    tzset();    

    try
    {
        mResources.Initialize();
        
        // Parse command-line
        
        enum CmdsE
        {
            CMD_DEVEL = 256,
            CMD_DIR_ICONS,
            CMD_DIR_DB,
            CMD_DIR_LNG,
            CMD_DIR_XML
        };
        
        static const struct option options[] =
        {
                {"devel", no_argument, NULL, CMD_DEVEL},
                {"debug", optional_argument, NULL, 'd'},
                {"version", no_argument, NULL, 'v'},
                {"help", no_argument, NULL, 'h'},
                {"icon-dir", required_argument, NULL, CMD_DIR_ICONS},
                {"db-dir", required_argument, NULL, CMD_DIR_DB,},
                {"lng-dir", required_argument, NULL, CMD_DIR_LNG},
                {"xml-dir", required_argument, NULL, CMD_DIR_XML},
                { NULL, 0, NULL, 0}
        };

        int my_argc = wxApp::argc;
        char ** my_argv = NULL;
        {
            my_argv = new char*[my_argc];
            for(int i = 0; i < my_argc; i++)
            {
                WarCollector<char> conv = wxApp::argv[i];
                size_t len = conv.GetValue().length();
                char *p = new char[len +1];
                strcpy(p, conv.GetValue().c_str());
                my_argv[i] = p;
            }
        }

        int ch;
        while((ch = getopt_long(my_argc, my_argv, "hvc:d::", options, NULL)) >= 0)
        {
            switch(ch)
            {
            case 'v':
                std::cout << PROGRAM << " " << WHID_VERSION << std::endl;
                return false;

            case CMD_DIR_ICONS:
                mIconDir = optarg;
                break;
                
            case CMD_DIR_DB:
                mDbDir = optarg;
                break;
            
            case CMD_DIR_LNG:
                mLngDir = optarg;
                break;
                
             case CMD_DIR_XML:
                mXmlDir = optarg;
                break;
                
            case CMD_DEVEL:
                mIconDir = "";
                mLngDir = "../../translations";
                mDbDir = "../../database";
                mXmlDir = "../../xml";
                break;

            case 'h':
                Usage();
                return false;

            case 'd':
                debug_flag = true;
                if (optarg)
                {
                    if (!debug_events.empty())
                        debug_events += ',';
                        debug_events += optarg;
                }
                break;
            case 0:
                break;

            default:
                Usage();
                return false;
            }
        }

        mOptions.OpenUserSettings(
            "whid-user-settings.xml",
            "whid" DEBUG_CONF_EXT);

        mOptions.SetAutoAdd();

        // Get settings
        WarPath<char> default_log_file, default_sqlite_file;
        
        default_log_file << WAR_HOME_APP_DATA_AND_SETTINGS 
            << "whid" DEBUG_CONF_EXT
            << WAR_SYSSLASH
            << "whid.log";

        default_sqlite_file << WAR_HOME_APP_DATA_AND_SETTINGS 
            << "whid" DEBUG_CONF_EXT
            << WAR_SYSSLASH
            << "whid.db";

        mOptions.GetOption("/log/file-name", mLogFile, 
            default_log_file.GetPath());

        bool log_err, log_info, log_debug, log_debug_database, 
            log_debug_mt, enable_log;
        mOptions.GetOption("/log/events/error", log_err, true);
        mOptions.GetOption("/log/events/info", log_info, true);
        mOptions.GetOption("/log/events/debug", log_debug, false);
        mOptions.GetOption("/log/events/debug_database", log_debug_database, false);
        mOptions.GetOption("/log/events/debug_mt", log_debug_mt, false);
        mOptions.GetOption("/log/enable", enable_log, false);

        if (enable_log)
        {
#define ADD(name) if (!mLogEvents.empty()) mLogEvents += ';'; mLogEvents += #name

            if (log_err || debug_flag)
            {
                ADD(ERROR);
                ADD(WARNINGS);
            }

            if (log_info || debug_flag)
            {
                ADD(INFO);
                ADD(NOTICE);
            }

            if (log_debug || debug_flag)
            {
                ADD(DEBUG);
            }

            if (log_debug_database)
            {
                ADD(DATABASE);
            }

            if (log_debug_mt)
            {
                ADD(THREADS);
            }
#undef ADD
        }

        mOptions.GetOption("/db/type", mEngine.mDbType, "sqlite");

        std::string mysql_db_name, sqlite_db_name;

        mOptions.GetOption("/db/mysql/host", mEngine.mDbHost, "localhost");
        mOptions.GetOption("/db/mysql/database", mysql_db_name, "whid");
        mOptions.GetOption("/db/mysql/user", mEngine.mDbUser, "whid");
        mOptions.GetOption("/db/mysql/passwd", mEngine.mDbPasswd, "secret");

        mOptions.GetOption("/db/min-connections", mEngine.mMinDbConnections, 2);
        mOptions.GetOption("/db/max-connections", mEngine.mMaxDbConnections, 4);

        // Sqlite
        mOptions.GetOption("/db/sqlite/database", sqlite_db_name, 
            default_sqlite_file.GetPath());


        if (mEngine.mDbType == "mysql")
            mEngine.mDbDatabase = mysql_db_name;
        else
            mEngine.mDbDatabase = sqlite_db_name;


        mOptions.GetOption("/db/def/file", mEngine.mDbDatabaseDefFile, "whid_database.xml");

        int ice_port = 30333;
        mOptions.GetOption("/ice/address", mEngine.mIceAddress, "localhost");
        mOptions.GetOption("/ice/port", ice_port, ice_port);
        mOptions.GetOption("/ice/gateway", mEngine.mGatewayName, "WhidGateway");
        mOptions.GetOption("/ice/control", mEngine.mControlName, "WhidControl");

        {
            std::stringstream ice;
            ice << mEngine.mIceAddress << ':' << ice_port;
            mEngine.mIceAddress = ice.str();
        }

        mOptions.Save();
    }
    catch(WarException& ex)
    {
        WarCollector<wxChar> msg;
        msg << "Failed to initialize the war libraries: "
            << ex.what();
        wxMessageBox(msg.GetValue().c_str(), _("ERROR"), wxICON_ERROR);
        return false;
    }

    try
    {

        if (WarLogEngine::GetEngine().HaveHandler("file"))
            WarLogEngine::GetEngine().KillHandler("file");
            
        if (!mLogFile.empty() && !mLogEvents.empty())
        {
            WarLogEngine::GetEngine().AddHandler(
                new WarLogHandlerFile(mLogFile.c_str(), "file", mLogEvents.c_str()));
        }

#if defined(DEBUG) && defined(WIN32)
        if (WAR_IS_LOGEVENT_IN_USE(WARLOG_DEBUG) || debug_flag)
        {
            ::AllocConsole();
            intptr_t stdout_handle = (intptr_t)GetStdHandle(STD_OUTPUT_HANDLE);
            int hcrt = _open_osfhandle(stdout_handle, _O_TEXT | _O_WRONLY);
            FILE *fp = NULL;
            fp = _fdopen(hcrt, "w");
            stdout->_file = fileno(fp);
            setvbuf(stdout, NULL, _IONBF, 0);
            std::cout.rdbuf(new std::filebuf(stdout));
        }
#endif

        if (debug_flag &&  WarLogEngine::GetEngine().HaveHandler("cout"))
        {
            WarLogEngine::GetEngine().EnableEvents("cout", 
                "ERROR;WARNINGS;INFO;SYSTEM;DEBUG");
        }
    }
    catch(WarException& ex)
    {
      WarCollector<wxChar> msg;
        msg << "Failed to initialize logging: "
            << ex.what();
            
        //wxString wx_msg = WAR_TEXT_NATIVE(msg.str().c_str());
        wxMessageBox(msg.GetValue().c_str());

        return false;
    }

    // Redirect xwWidgets log-events to the WarLog framework
    wxLog::SetActiveTarget(new WarWxLogProxy);

    // Set locale
    {
        int locale = 0;
        mOptions.GetOption("/preferences/language", locale, wxLANGUAGE_DEFAULT);
        SetLocale(locale);
    }



    // // Test mail
    //try
    //{
    //    WarSmtpClient mail;
    //    war_smtp_client_auth_list_t auth_list;

    //    WarSmtpClientAuthCramMd5 *cram_auth = 
    //        new WarSmtpClientAuthCramMd5("mx@jgaa.com", "NOQBKEVI");
    //    auth_list.push_back(cram_auth);

    //    mail.Create("mime.ldp.no", auth_list, "jgaa.com");
    //}
    //catch(const WarException& ex)
    //{
    //    WAR_ERRLOG;
    //    err_log << "Gakk!" << war_endl;
    //}
    //

    /*if (!war_is_file("whid_resources.zip"))
    {
        wxMessageBox(_("Cant find the resource-file \"whid_resources.zip\"."));
        goto init_failed;
    }*/

////@begin WhidApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	MainFrame* mainWindow = new MainFrame( NULL );
	mainWindow->Show(true);
////@end WhidApp initialisation

    
    try
    {
        mEngine.mDbDir = mDbDir.GetPath();
        mEngine.Initialize();
        if (!mEngine.HaveDatabase())
        {
            FirstUseDlg dlg(NULL);
            if (dlg.ShowModal() == wxID_OK)
            {
                mEngine.CreateAndBootstrap(
                    dlg.GetFullName(), 
                    dlg.GetLoginName(),
                    (double)dlg.GetCharge());
            }
            else
            {
                return true; // Enable main window in cripped mode
            }
        }

        mEngine.StartEngine(false);
    }
    catch(WarException)
    {
        WAR_ERRLOG;
        err_log << "Initialization of the database failed. Giving up."
            << war_endl;
    
        goto init_failed;
    }

    mainWindow->SetCrippled(false);

    try
    {
        mainWindow->LoadOptions();
    }
    catch(WarException)
    {
        goto init_failed;
    }
    
    if (!mainWindow->Login())
        goto init_failed;
    
    mainWindow->ReloadNodes();


    return true;


init_failed:
    // OnExit() is nnot called, so we must release stuff here
    WarShutdownEngine::GetEngine().Shutdown();
    return false;
}

void WhidApp::SetLocale(int locale)
{
    if (NULL != mpLocale)
        delete mpLocale;
    
    mpLocale = new wxLocale;
    mpLocale->Init(locale);
    
    WarPath<wxChar> path = mLngDir;
    mpLocale->AddCatalogLookupPathPrefix(path.GetPath());
    mpLocale->AddCatalog(wxT("whid"));
}

/*!
 * Cleanup for WhidApp
 */
int WhidApp::OnExit()
{     
    WarShutdownEngine::GetEngine().Shutdown();
////@begin WhidApp cleanup
	return wxApp::OnExit();
////@end WhidApp cleanup
}

wxBitmap WhidApp::GetBitmapResource( const wxString& name )
{
    wxBitmap *p = WhidApp::LoadImageFromResource(name);
    if (NULL != p)
    {
        wxBitmap rval(*p);
        delete p;
        return rval;
    }
    return wxNullBitmap;
}

wxBitmap* WhidApp::LoadImageFromResource(const wxString& path)
{
    WAR_DEBUGLOG;
    debug_log << "Loading \"" << path.GetData() << "\"." << war_endl;
    
    // Both icon-path and the path may include the "icons" token. So
    // we strip it off the icon-path if it exist there,
    WarPath<wxChar> my_path = wxGetApp().GetIconDir();
    if (!my_path.IsEmpty())
    {
        WarPath<wxChar>::path_vect_t elements;
        my_path.GetElements(elements);
        if (!elements.empty())
        {
            WarPath<wxChar> icons = "icons";
            if (elements[elements.size() -1] == icons)
                my_path.ClipAfterLevel(elements.size() -1);
        }
        
        if (!my_path.IsEmpty())
            my_path << WAR_SYSSLASH;
	}
    my_path << path;

    wxBitmap* bmp = NULL;
    wxFileSystem fs;
    wxFSFile *fsf = fs.OpenFile(my_path.GetPath());
    if (fsf != NULL)
    {
       wxInputStream* stream = fsf->GetStream();
       if (stream != NULL)
       {
          bmp = new wxBitmap(*(new wxImage(*stream, wxBITMAP_TYPE_PNG)));
       }
       // don't delete stream;
    }
    else
    {
        debug_log << "Failed to open file \"" << my_path << "\"." << war_endl;
    }
    delete fsf;

    return bmp;
} 

int WhidApp::GetMinutes(const std::string timeStr)
{
    int minutes = 0, hours = 0;
    sscanf(timeStr.c_str(), "%d:%d", &hours, &minutes);

    return (hours * 60) + minutes;
}

std::string WhidApp::GetMinutesAsString(const int minutes, const int align)
{
    int my_minutes = minutes;

    if (align)
    {
        my_minutes = (my_minutes / align) * align;
        if (minutes % align)
            my_minutes += align; // Round up
    }


    char buf[64];

    if (my_minutes % 60)
        sprintf(buf, "%02d:%02d", my_minutes / 60, my_minutes % 60);
    else
        sprintf(buf, "%02d", my_minutes / 60);

    return buf;
}

wxString WhidApp::GetMinutes(const int minutes, const int align)
{
    wxString rval;
    WarUtf8 my_val = GetMinutesAsString(minutes, align);
    
    return my_val.GetUnicode().c_str();
}


void WhidApp::LaunchBrowser(wxString url)
{
    std::string browser;
    mOptions.GetOption("/preferences/browser", browser);

    if (!browser.empty())
    {
        wxString cmd;
        WarCollector<wxChar> conv = browser;

        cmd = conv.GetValue().c_str();
        if (cmd.Replace(wxT("%u"), url) < 1)
        {
            cmd += wxT(" ");
            cmd += url;
        }

        ::wxExecute(cmd);
    }
    else
    {
        ::wxLaunchDefaultBrowser(url);
    }
}
