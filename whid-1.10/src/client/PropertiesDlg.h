/////////////////////////////////////////////////////////////////////////////
// Name:        PropertiesDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     31/12/2006 13:59:47
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PROPERTIESDLG_H_
#define _PROPERTIESDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "PropertiesDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/choicebk.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxChoicebook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_PROPERTIESDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROPERTIESDLG_TITLE _("Properties")
#define SYMBOL_PROPERTIESDLG_IDNAME ID_PROPERTIESDLG
#define SYMBOL_PROPERTIESDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROPERTIESDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * PropertiesDlg class declaration
 */

class PropertiesDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( PropertiesDlg )
    DECLARE_EVENT_TABLE()

public:
    enum DbTypeE
    {
            DBTYPE_SQLITE,
            DBTYPE_MYSQL
    };

    /// Constructors
    PropertiesDlg( );
    PropertiesDlg( wxWindow* parent, wxWindowID id = SYMBOL_PROPERTIESDLG_IDNAME, const wxString& caption = SYMBOL_PROPERTIESDLG_TITLE, const wxPoint& pos = SYMBOL_PROPERTIESDLG_POSITION, const wxSize& size = SYMBOL_PROPERTIESDLG_SIZE, long style = SYMBOL_PROPERTIESDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROPERTIESDLG_IDNAME, const wxString& caption = SYMBOL_PROPERTIESDLG_TITLE, const wxPoint& pos = SYMBOL_PROPERTIESDLG_POSITION, const wxSize& size = SYMBOL_PROPERTIESDLG_SIZE, long style = SYMBOL_PROPERTIESDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin PropertiesDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TEST_CONNECTION
    void OnTestConnectionClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TEST_CONNECTION
    void OnTestConnectionUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATE_DATABASE
    void OnCreateDatabaseClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CREATE_DATABASE
    void OnCreateDatabaseUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end PropertiesDlg event handler declarations

////@begin PropertiesDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PropertiesDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PropertiesDlg member variables
    wxChoice* mLanguageCtl;
    wxTextCtrl* mBrowserCtl;
    wxTextCtrl* mIceHostCtl;
    wxTextCtrl* mIcePortCtl;
    wxChoicebook* mDatabaseTypeCtl;
    wxTextCtrl* mSqliteNameCtl;
    wxTextCtrl* mMysqlHostCtl;
    wxTextCtrl* mMysqlDatabaseCtl;
    wxTextCtrl* mMysqlUserCtl;
    wxTextCtrl* mMysqlPasswdCtl;
    wxTextCtrl* mLogFileNameCtl;
    wxCheckBox* mLogEnableCtl;
    wxCheckBox* mLogErrorsAndWarningsCtl;
    wxCheckBox* mLogInfoAndNoticeCtl;
    wxCheckBox* mLogDebugCtl;
    wxCheckBox* mLogDatabaseCtl;
    wxCheckBox* mLogMtCtl;
    wxTextCtrl* mEmailServerCtl;
    wxTextCtrl* mEmailReturnAddressCtl;
    wxTextCtrl* mCramMd5UserNameCtl;
    wxTextCtrl* mCramMd5PaswdCtl;
    /// Control identifiers
    enum {
        ID_PROPERTIESDLG = 10016,
        ID_NOTEBOOK2 = 10017,
        ID_PREFERENCES = 10184,
        ID_USE_LANGUAGE = 10185,
        ID_USE_WEBBROWSER = 10186,
        ID_COMMSETTINGS = 10020,
        ID_ICE_HOST = 10086,
        ID_ICE_PORT = 10094,
        ID_DATABASE_SELECTION = 10030,
        ID_SQLITE_PANEL = 10036,
        ID_SQLITE_NAME = 10078,
        ID_MYSQL_PANEL = 10079,
        ID_MYSQL_HOST = 10080,
        ID_MYSQL_DB_NAME = 10081,
        ID_MYSQL_USER = 10082,
        ID_MYSQL_PASSWD = 10095,
        ID_TEST_CONNECTION = 10084,
        ID_CREATE_DATABASE = 10085,
        ID_LOG = 10083,
        ID_FILE_NAME = 10087,
        ID_LOG_ENABLE = 10088,
        ID_LOG_ERRORS = 10089,
        ID_LOG_INFO = 10090,
        ID_LOG_DEBUG = 10091,
        ID_LOG_DATABASE = 10092,
        ID_LOG_MT = 10093,
        ID_EMAIL_TAB = 10164,
        ID_EMAIL_SERVER_ADDRESS = 10166,
        ID_RETURN_ADDRESS = 10165,
        ID_TEXTCTRL4 = 10171,
        ID_TEXTCTRL5 = 10172
    };
////@end PropertiesDlg member variables
};

#endif
    // _PROPERTIESDLG_H_
