/////////////////////////////////////////////////////////////////////////////
// Name:        aboutdlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     01/01/2007 10:54:53
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ABOUTDLG_H_
#define _ABOUTDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "aboutdlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/html/htmlwin.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class WhidHtmlWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_ABOUTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ABOUTDLG_TITLE _("About")
#define SYMBOL_ABOUTDLG_IDNAME ID_ABOUTDLG
#define SYMBOL_ABOUTDLG_SIZE wxSize(400, 300)
#define SYMBOL_ABOUTDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

#include "WhidHtmlWindow.h"

/*!
 * AboutDlg class declaration
 */

class AboutDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AboutDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AboutDlg( );
    AboutDlg( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTDLG_IDNAME, const wxString& caption = SYMBOL_ABOUTDLG_TITLE, const wxPoint& pos = SYMBOL_ABOUTDLG_POSITION, const wxSize& size = SYMBOL_ABOUTDLG_SIZE, long style = SYMBOL_ABOUTDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTDLG_IDNAME, const wxString& caption = SYMBOL_ABOUTDLG_TITLE, const wxPoint& pos = SYMBOL_ABOUTDLG_POSITION, const wxSize& size = SYMBOL_ABOUTDLG_SIZE, long style = SYMBOL_ABOUTDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AboutDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HOMEPAGE
    void OnHOMEPAGEClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DONATE
    void OnDONATEClick( wxCommandEvent& event );

////@end AboutDlg event handler declarations

////@begin AboutDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AboutDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AboutDlg member variables
    WhidHtmlWindow* mHtmlCtl;
    /// Control identifiers
    enum {
        ID_ABOUTDLG = 10102,
        ID_ABOUT_HTML = 10103,
        ID_HOMEPAGE = 10104,
        ID_DONATE = 10105
    };
////@end AboutDlg member variables
};

#endif
    // _ABOUTDLG_H_
