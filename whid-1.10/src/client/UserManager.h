/////////////////////////////////////////////////////////////////////////////
// Name:        UserManager.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     06/07/2007 08:49:25
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "UserManager.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_USERMANAGER_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_USERMANAGER_TITLE _("User Manager")
#define SYMBOL_USERMANAGER_IDNAME ID_USERMANAGER
#define SYMBOL_USERMANAGER_SIZE wxSize(400, 300)
#define SYMBOL_USERMANAGER_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * UserManager class declaration
 */

class UserManager: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( UserManager )
    DECLARE_EVENT_TABLE()

public:
    enum HeaderColumnsE
    {
        HC_NAME, HC_LOGIN_NAME, HC_ACTIVE, HC_ADMIN
    };

    /// Constructors
    UserManager();
    UserManager( wxWindow* parent, wxWindowID id = SYMBOL_USERMANAGER_IDNAME, const wxString& caption = SYMBOL_USERMANAGER_TITLE, const wxPoint& pos = SYMBOL_USERMANAGER_POSITION, const wxSize& size = SYMBOL_USERMANAGER_SIZE, long style = SYMBOL_USERMANAGER_STYLE );

    /// Creation
    bool Create(WHID::WhidControlPrx prx, wxWindow* parent, wxWindowID id = SYMBOL_USERMANAGER_IDNAME, const wxString& caption = SYMBOL_USERMANAGER_TITLE, const wxPoint& pos = SYMBOL_USERMANAGER_POSITION, const wxSize& size = SYMBOL_USERMANAGER_SIZE, long style = SYMBOL_USERMANAGER_STYLE );

    /// Destructor
    ~UserManager();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();
    
    void EditUser();

////@begin UserManager event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_USERMANAGER
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_USER_LIST
    void OnUSERLISTSelected( wxListEvent& event );

    /// wxEVT_LEFT_DCLICK event handler for ID_USER_LIST
    void OnLeftDClick( wxMouseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_USER
    void OnADDUSERClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_ADD_USER
    void OnADDUSERUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_USER
    void OnDELETEUSERClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_DELETE_USER
    void OnDELETEUSERUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EDIT_USER
    void OnEditUserClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EDIT_USER
    void OnEditUserUpdate( wxUpdateUIEvent& event );

////@end UserManager event handler declarations

////@begin UserManager member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end UserManager member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin UserManager member variables
    wxListCtrl* mUserListCtl;
    /// Control identifiers
    enum {
        ID_USERMANAGER = 10135,
        ID_USER_LIST = 10138,
        ID_ADD_USER = 10139,
        ID_DELETE_USER = 10140,
        ID_EDIT_USER = 10187
    };
////@end UserManager member variables

private:
    void ReloadUsers();
    WHID::WhidControlPrx mWhidControl;
    WHID::UserInfoList mUsers;
};

#endif
    // _USERMANAGER_H_
