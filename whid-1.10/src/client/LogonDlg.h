/////////////////////////////////////////////////////////////////////////////
// Name:        LogonDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/07/2007 10:25:03
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOGONDLG_H_
#define _LOGONDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "LogonDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
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
#define SYMBOL_LOGONDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_LOGONDLG_TITLE _("Log on")
#define SYMBOL_LOGONDLG_IDNAME ID_LOGONDLG
#define SYMBOL_LOGONDLG_SIZE wxSize(400, 300)
#define SYMBOL_LOGONDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * LogonDlg class declaration
 */

class LogonDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( LogonDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LogonDlg();
    LogonDlg( wxWindow* parent, wxWindowID id = SYMBOL_LOGONDLG_IDNAME, const wxString& caption = SYMBOL_LOGONDLG_TITLE, const wxPoint& pos = SYMBOL_LOGONDLG_POSITION, const wxSize& size = SYMBOL_LOGONDLG_SIZE, long style = SYMBOL_LOGONDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LOGONDLG_IDNAME, const wxString& caption = SYMBOL_LOGONDLG_TITLE, const wxPoint& pos = SYMBOL_LOGONDLG_POSITION, const wxSize& size = SYMBOL_LOGONDLG_SIZE, long style = SYMBOL_LOGONDLG_STYLE );

    /// Destructor
    ~LogonDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin LogonDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end LogonDlg event handler declarations

////@begin LogonDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LogonDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LogonDlg member variables
    /// Control identifiers
    enum {
        ID_LOGONDLG = 10142,
        ID_TEXTCTRL1 = 10143,
        ID_PASSWD = 10144
    };
////@end LogonDlg member variables

    std::string mLogonName;
    std::string mPasswd;
};

#endif
    // _LOGONDLG_H_
