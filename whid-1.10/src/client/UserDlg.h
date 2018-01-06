/////////////////////////////////////////////////////////////////////////////
// Name:        UserDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     23/07/2007 10:35:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _USERDLG_H_
#define _USERDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "UserDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/valgen.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_USERDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_USERDLG_TITLE _("Edit User")
#define SYMBOL_USERDLG_IDNAME ID_USERDLG
#define SYMBOL_USERDLG_SIZE wxSize(400, 300)
#define SYMBOL_USERDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * UserDlg class declaration
 */

class UserDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( UserDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    UserDlg();
    

    /// Creation
    bool Create(WHID::WhidControlPrx ctl, WHID::UserPrx userPrx, wxWindow* parent, wxWindowID id = SYMBOL_USERDLG_IDNAME, const wxString& caption = SYMBOL_USERDLG_TITLE, const wxPoint& pos = SYMBOL_USERDLG_POSITION, const wxSize& size = SYMBOL_USERDLG_SIZE, long style = SYMBOL_USERDLG_STYLE );

    /// Destructor
    ~UserDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin UserDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESET_PASSWORD_BTN
    void OnRESETPASSWORDBTNClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end UserDlg event handler declarations

////@begin UserDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end UserDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin UserDlg member variables
    wxBoxSizer* mPasswdSizer;
    wxTextCtrl* mPasswdCtl;
    wxButton* mPassdResetCtl;
    /// Control identifiers
    enum {
        ID_USERDLG = 10145,
        ID_NOTEBOOK3 = 10146,
        ID_USERPROPERTIES = 10147,
        ID_USER_NAME = 10148,
        ID_LOGIN_NAME = 10149,
        ID_PASSWD_INPUT = 10156,
        ID_RESET_PASSWORD_BTN = 10150,
        ID_ACTIVE = 10151,
        ID_ADMIN = 10152,
        ID_NOTES = 10153,
        ID_CONTACT_INFO = 10162,
        ID_EMAIL = 10163
    };
////@end UserDlg member variables

private:
    WHID::UserPrx mUserPrx;
    WHID::WhidControlPrx mWhidControl;
    WHID::UserData mUserData;
    std::string mPasswd;
};

#endif
    // _USERDLG_H_
