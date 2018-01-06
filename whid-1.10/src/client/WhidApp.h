/////////////////////////////////////////////////////////////////////////////
// Name:        WhidApp.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     15/12/2006 14:08:19
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WhidEngine_H_
#define _WhidEngine_H_

#define PROGRAM "Whid"
#define WHID_VERSION "1.10"
#define COPYRIGHT "COPYRIGHT (C) 2007 - 2009 by Jarle (jgaa) Aase"

#ifndef WHID_DB_DIR
#   define WHID_DB_DIR "database"
#endif

#ifndef WHID_ICON_DIR
#   define WHID_ICON_DIR "icons"
#endif

#ifndef WHID_LNG_DIR
#   define WHID_LNG_DIR "translations"
#endif

#ifndef WHID_XML_DIR
#   define WHID_XML_DIR "xml"
#endif


#if defined(WAR_DEBUG) && !defined(_DEBUG)
#   define _DEBUG
#endif

#if  defined(_DEBUG) && !defined(DEBUG)
#   define DEBUG
#endif


#ifdef DEBUG
#   define DEBUG_CONF_EXT "_debug"
#else
#   define DEBUG_CONF_EXT ""
#endif


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "WhidApp.h"
#endif

#include <WarCorelibResources.h>
#include <xml/WarXmlOptions.h>
#include <WarPath.h>

/*!
 * Includes
 */

#include "server/WhidEngine.h"

////@begin includes
#include "wx/image.h"
#include "MainFrame.h"
////@end includes

#include <wx/app.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * WhidApp class declaration
 */

class WhidApp: public wxApp
{    
    DECLARE_CLASS( WhidApp )
    DECLARE_EVENT_TABLE()

public:
    typedef WarPath<char> path_t;

    struct lng_defs
    {
        const wxChar *mName;
        int mLnaguageNo;
    };

    /// Constructor
    WhidApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin WhidApp event handler declarations

////@end WhidApp event handler declarations

////@begin WhidApp member function declarations

////@end WhidApp member function declarations

////@begin WhidApp member variables
////@end WhidApp member variables

    static wxBitmap* LoadImageFromResource(const wxString& path);
    static wxString GetMinutes(const int minutes, const int align = 0);
    static std::string GetMinutesAsString(const int minutes, const int align = 0);
    static int GetMinutes(const std::string timeStr);
    void SetLocale(int locale = wxLANGUAGE_DEFAULT);
    void LaunchBrowser(wxString url);
    static wxBitmap GetBitmapResource( const wxString& name );

    WarCorelibResources mResources;
    WhidEngine mEngine;
    std::string mLogFile;
    std::string mLogEvents;
    std::string mDir;
    WarXmlOptions mOptions;

    static const lng_defs mLanguages[4];
    
    const path_t& GetDbDir() 
    {
        return mDbDir;
    }
    
    const path_t& GetIconDir() 
    {
        return mIconDir;
    }
    
    const path_t& GetLngDir() 
    {
        return mLngDir;
    }
    
    const path_t GetXmlDir(const char *name = NULL) 
    {
        WarPath<char> path = mXmlDir;
        if (name && *name)
        {
            if (!path.IsEmpty())
                path << WAR_SYSSLASH;
            path << name;
        }
        return path;
    }
    
protected:
    path_t mDbDir;
    path_t mIconDir;
    path_t mLngDir;
    path_t mXmlDir;

private:
    virtual void Usage();

    wxLocale *mpLocale;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(WhidApp)
////@end declare app


#define BROWSER(url) wxGetApp().LaunchBrowser(url)


#endif
    // _WhidEngine_H_
