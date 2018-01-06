/////////////////////////////////////////////////////////////////////////////
// Name:        MysqlCreateDatabaseDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     31/12/2006 21:30:52
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYSQLCREATEDATABASEDLG_H_
#define _MYSQLCREATEDATABASEDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "MysqlCreateDatabaseDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/valgen.h"
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
#define SYMBOL_MYSQLCREATEDATABASEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MYSQLCREATEDATABASEDLG_TITLE _("Create MySQL Database")
#define SYMBOL_MYSQLCREATEDATABASEDLG_IDNAME ID_MYSQLCREATEDATABASEDLG
#define SYMBOL_MYSQLCREATEDATABASEDLG_SIZE wxSize(400, 300)
#define SYMBOL_MYSQLCREATEDATABASEDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * MysqlCreateDatabaseDlg class declaration
 */

class MysqlCreateDatabaseDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( MysqlCreateDatabaseDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MysqlCreateDatabaseDlg( );
    MysqlCreateDatabaseDlg( wxWindow* parent, wxWindowID id = SYMBOL_MYSQLCREATEDATABASEDLG_IDNAME, const wxString& caption = SYMBOL_MYSQLCREATEDATABASEDLG_TITLE, const wxPoint& pos = SYMBOL_MYSQLCREATEDATABASEDLG_POSITION, const wxSize& size = SYMBOL_MYSQLCREATEDATABASEDLG_SIZE, long style = SYMBOL_MYSQLCREATEDATABASEDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MYSQLCREATEDATABASEDLG_IDNAME, const wxString& caption = SYMBOL_MYSQLCREATEDATABASEDLG_TITLE, const wxPoint& pos = SYMBOL_MYSQLCREATEDATABASEDLG_POSITION, const wxSize& size = SYMBOL_MYSQLCREATEDATABASEDLG_SIZE, long style = SYMBOL_MYSQLCREATEDATABASEDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MysqlCreateDatabaseDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATE_MYSQL_DATABASE
    void OnCreateDatabaseClick( wxCommandEvent& event );

////@end MysqlCreateDatabaseDlg event handler declarations

////@begin MysqlCreateDatabaseDlg member function declarations

    WarUtf8 GetSuperUserName() const { return mSuperUserName ; }
    void SetSuperUserName(WarUtf8 value) { mSuperUserName = value ; }

    WarUtf8 GetSuperUserPasswd() const { return mSuperUserPasswd ; }
    void SetSuperUserPasswd(WarUtf8 value) { mSuperUserPasswd = value ; }

    WarUtf8 GetLoginName() const { return mLoginName ; }
    void SetLoginName(WarUtf8 value) { mLoginName = value ; }

    WarUtf8 GetUserName() const { return mUserName ; }
    void SetUserName(WarUtf8 value) { mUserName = value ; }

    int GetCharge() const { return mCharge ; }
    void SetCharge(int value) { mCharge = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MysqlCreateDatabaseDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MysqlCreateDatabaseDlg member variables
    WarUtf8 mSuperUserName;
    WarUtf8 mSuperUserPasswd;
    WarUtf8 mLoginName;
    WarUtf8 mUserName;
    int mCharge;
    /// Control identifiers
    enum {
        ID_MYSQLCREATEDATABASEDLG = 10096,
        ID_SUPERUSER_NAME = 10097,
        ID_SUPERUSER_PWD = 10098,
        ID_LOGIN_NAME = 10137,
        ID_WHID_USER = 10100,
        ID_WHID_USER_CHARGE = 10101,
        ID_CREATE_MYSQL_DATABASE = 10099
    };
////@end MysqlCreateDatabaseDlg member variables

    // From the properies dialog
    std::string mDb, mHost, mUser, mPasswd;
};

#endif
    // _MYSQLCREATEDATABASEDLG_H_
