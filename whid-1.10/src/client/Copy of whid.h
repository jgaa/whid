/////////////////////////////////////////////////////////////////////////////
// Name:        whid.h
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
#define VERSION "0.01"
#define COPYRIGHT "COPYRIGHT (C) 2006 by Jarle (jgaa) Aase"


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "whid.h"
#endif

#include <WarCorelibResources.h>

/*!
 * Includes
 */

#include "WhidEngine.h"

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/image.h"
#include "MainFrame.h"
////@end includes

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

    WarCorelibResources mResources;
    WhidEngine mEngine;
    std::string mLogFile;
    std::string mDir;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(WhidApp)
////@end declare app

#endif
    // _WhidEngine_H_
