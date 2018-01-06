/////////////////////////////////////////////////////////////////////////////
// Name:        PropertiesDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     31/12/2006 13:59:47
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PropertiesDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"
#include <WarUtf8.h>


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

#include "PropertiesDlg.h"
#include "MysqlCreateDatabaseDlg.h"

////@begin XPM images
////@end XPM images

/*!
 * PropertiesDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( PropertiesDlg, wxDialog )

/*!
 * PropertiesDlg event table definition
 */

BEGIN_EVENT_TABLE( PropertiesDlg, wxDialog )

////@begin PropertiesDlg event table entries
    EVT_BUTTON( ID_TEST_CONNECTION, PropertiesDlg::OnTestConnectionClick )
    EVT_UPDATE_UI( ID_TEST_CONNECTION, PropertiesDlg::OnTestConnectionUpdate )

    EVT_BUTTON( ID_CREATE_DATABASE, PropertiesDlg::OnCreateDatabaseClick )
    EVT_UPDATE_UI( ID_CREATE_DATABASE, PropertiesDlg::OnCreateDatabaseUpdate )

    EVT_BUTTON( wxID_OK, PropertiesDlg::OnOKClick )

////@end PropertiesDlg event table entries

END_EVENT_TABLE()

/*!
 * PropertiesDlg constructors
 */

PropertiesDlg::PropertiesDlg( )
{
    Init();
}

PropertiesDlg::PropertiesDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * PropertiesDlg creator
 */

bool PropertiesDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin PropertiesDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end PropertiesDlg creation

    // Initialize the controls
    
#define INIT(path, control) { \
        std::wstring value; \
        wxGetApp().mOptions.GetOption(path, value); \
        control->SetValue(value.c_str());\
    }

#define INITBOOL(path, control) { \
        bool value; \
        wxGetApp().mOptions.GetOption(path, value, true); \
        control->SetValue(value);\
    }

#define INITINT(path, control) { \
        int value; wxString str; \
        wxGetApp().mOptions.GetOption(path, value, 0); \
        str = wxString::Format(_("%d"), value); \
        control->SetValue(str);\
    }

    WarCollector<wchar_t> def;
    def = wxGetApp().mLogFile;
    INIT("/log/file-name", mLogFileNameCtl);
    INITBOOL("/log/events/debug_mt", mLogMtCtl);
    INITBOOL("/log/events/error", mLogErrorsAndWarningsCtl);
    INITBOOL("/log/events/info", mLogInfoAndNoticeCtl);
    INITBOOL("/log/events/debug", mLogDebugCtl);
    INITBOOL("/log/events/debug_database", mLogDatabaseCtl);
    INITBOOL("/log/enable", mLogEnableCtl);

    INIT("/ice/address", mIceHostCtl);
    INITINT("/ice/port", mIcePortCtl);
    //INIT("/ice/gateway", );

    INIT("/db/sqlite/database", mSqliteNameCtl);

    INIT("/db/mysql/host", mMysqlHostCtl);
    INIT("/db/mysql/database", mMysqlDatabaseCtl);
    INIT("/db/mysql/user", mMysqlUserCtl);
    INIT("/db/mysql/passwd", mMysqlPasswdCtl);

    std::string db_type;
    wxGetApp().mOptions.GetOption("/db/type", db_type);
    if (db_type == "mysql")
        mDatabaseTypeCtl->SetSelection(DBTYPE_MYSQL);
    else
        mDatabaseTypeCtl->SetSelection(DBTYPE_SQLITE);

    INIT("/email/server/address", mEmailServerCtl);
    INIT("/email/server/returnaddress", mEmailReturnAddressCtl);

    INIT("/email/auth/cram.md5/name", mCramMd5UserNameCtl);
    INIT("/email/auth/cram.md5/passwd", mCramMd5PaswdCtl);

    // Get languages
    {
        war_int32_t current_lng = 0;
        wxGetApp().mOptions.GetOption("/preferences/language", 
            current_lng, wxLANGUAGE_DEFAULT);

        for(const WhidApp::lng_defs *p = WhidApp::mLanguages; NULL != p->mName; p++)
        {
            int item = mLanguageCtl->Append(::wxGetTranslation(p->mName), (void *)p);
            if (current_lng == p->mLnaguageNo)
                mLanguageCtl->SetSelection(item);
        }
    }

    INIT("/preferences/browser", mBrowserCtl);

#undef INIT
#undef INITBOOL
#undef INITINT

    return true;
}

/*!
 * Member initialisation 
 */

void PropertiesDlg::Init()
{
////@begin PropertiesDlg member initialisation
    mLanguageCtl = NULL;
    mBrowserCtl = NULL;
    mIceHostCtl = NULL;
    mIcePortCtl = NULL;
    mDatabaseTypeCtl = NULL;
    mSqliteNameCtl = NULL;
    mMysqlHostCtl = NULL;
    mMysqlDatabaseCtl = NULL;
    mMysqlUserCtl = NULL;
    mMysqlPasswdCtl = NULL;
    mLogFileNameCtl = NULL;
    mLogEnableCtl = NULL;
    mLogErrorsAndWarningsCtl = NULL;
    mLogInfoAndNoticeCtl = NULL;
    mLogDebugCtl = NULL;
    mLogDatabaseCtl = NULL;
    mLogMtCtl = NULL;
    mEmailServerCtl = NULL;
    mEmailReturnAddressCtl = NULL;
    mCramMd5UserNameCtl = NULL;
    mCramMd5PaswdCtl = NULL;
////@end PropertiesDlg member initialisation
}
/*!
 * Control creation for PropertiesDlg
 */

void PropertiesDlg::CreateControls()
{    
////@begin PropertiesDlg content construction
    // Generated by DialogBlocks, ti. 03. feb. 2009 kl. 10.56 +0200 (unregistered)

    PropertiesDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxNotebook* itemNotebook3 = new wxNotebook( itemDialog1, ID_NOTEBOOK2, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

    wxPanel* itemPanel4 = new wxPanel( itemNotebook3, ID_PREFERENCES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Language"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxHORIZONTAL);
    itemBoxSizer5->Add(itemStaticBoxSizer6, 0, wxGROW|wxALL, 5);
    wxArrayString mLanguageCtlStrings;
    mLanguageCtl = new wxChoice( itemPanel4, ID_USE_LANGUAGE, wxDefaultPosition, wxSize(200, -1), mLanguageCtlStrings, 0 );
    itemStaticBoxSizer6->Add(mLanguageCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer8Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Default helper applications"));
    wxStaticBoxSizer* itemStaticBoxSizer8 = new wxStaticBoxSizer(itemStaticBoxSizer8Static, wxHORIZONTAL);
    itemBoxSizer5->Add(itemStaticBoxSizer8, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer9 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer8->Add(itemFlexGridSizer9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel4, wxID_STATIC, _("Web browser"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBrowserCtl = new wxTextCtrl( itemPanel4, ID_USE_WEBBROWSER, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemFlexGridSizer9->Add(mBrowserCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook3->AddPage(itemPanel4, _("Preferences"));

    wxPanel* itemPanel12 = new wxPanel( itemNotebook3, ID_COMMSETTINGS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    itemPanel12->SetSizer(itemBoxSizer13);

    wxStaticBox* itemStaticBoxSizer14Static = new wxStaticBox(itemPanel12, wxID_ANY, _("Ice settings"));
    wxStaticBoxSizer* itemStaticBoxSizer14 = new wxStaticBoxSizer(itemStaticBoxSizer14Static, wxVERTICAL);
    itemBoxSizer13->Add(itemStaticBoxSizer14, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer15 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer14->Add(itemFlexGridSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel12, wxID_STATIC, _("Ice Host"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer15->Add(itemStaticText16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mIceHostCtl = new wxTextCtrl( itemPanel12, ID_ICE_HOST, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer15->Add(mIceHostCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel12, wxID_STATIC, _("Ice Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer15->Add(itemStaticText18, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mIcePortCtl = new wxTextCtrl( itemPanel12, ID_ICE_PORT, _T(""), wxDefaultPosition, wxSize(60, -1), 0 );
    itemFlexGridSizer15->Add(mIcePortCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook3->AddPage(itemPanel12, _("Communication"));

    mDatabaseTypeCtl = new wxChoicebook( itemNotebook3, ID_DATABASE_SELECTION, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );

    wxPanel* itemPanel21 = new wxPanel( mDatabaseTypeCtl, ID_SQLITE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel21->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer22 = new wxFlexGridSizer(2, 2, 0, 0);
    itemPanel21->SetSizer(itemFlexGridSizer22);

    wxStaticText* itemStaticText23 = new wxStaticText( itemPanel21, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer22->Add(itemStaticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSqliteNameCtl = new wxTextCtrl( itemPanel21, ID_SQLITE_NAME, _T(""), wxDefaultPosition, wxSize(350, -1), 0 );
    itemFlexGridSizer22->Add(mSqliteNameCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mDatabaseTypeCtl->AddPage(itemPanel21, _("SQLite (default, embedded database)"));

    wxPanel* itemPanel25 = new wxPanel( mDatabaseTypeCtl, ID_MYSQL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel25->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxVERTICAL);
    itemPanel25->SetSizer(itemBoxSizer26);

    wxStaticBox* itemStaticBoxSizer27Static = new wxStaticBox(itemPanel25, wxID_ANY, _("MySQL Connection properties"));
    wxStaticBoxSizer* itemStaticBoxSizer27 = new wxStaticBoxSizer(itemStaticBoxSizer27Static, wxHORIZONTAL);
    itemBoxSizer26->Add(itemStaticBoxSizer27, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer28 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer27->Add(itemFlexGridSizer28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel25, wxID_STATIC, _("Database Host"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText29, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMysqlHostCtl = new wxTextCtrl( itemPanel25, ID_MYSQL_HOST, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    if (PropertiesDlg::ShowToolTips())
        mMysqlHostCtl->SetToolTip(_("Hostname or IP-number for the MySQL Server"));
    itemFlexGridSizer28->Add(mMysqlHostCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel25, wxID_STATIC, _("Database Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMysqlDatabaseCtl = new wxTextCtrl( itemPanel25, ID_MYSQL_DB_NAME, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer28->Add(mMysqlDatabaseCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel25, wxID_STATIC, _("User Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMysqlUserCtl = new wxTextCtrl( itemPanel25, ID_MYSQL_USER, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer28->Add(mMysqlUserCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel25, wxID_STATIC, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText35, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMysqlPasswdCtl = new wxTextCtrl( itemPanel25, ID_MYSQL_PASSWD, _T(""), wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD );
    itemFlexGridSizer28->Add(mMysqlPasswdCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer26->Add(itemBoxSizer37, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton38 = new wxButton( itemPanel25, ID_TEST_CONNECTION, _("Test connection"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer37->Add(itemButton38, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton39 = new wxButton( itemPanel25, ID_CREATE_DATABASE, _("Create database"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer37->Add(itemButton39, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mDatabaseTypeCtl->AddPage(itemPanel25, _("MySQL (5.0 or newer) Server Connection"));

    itemNotebook3->AddPage(mDatabaseTypeCtl, _("Database"));

    wxPanel* itemPanel40 = new wxPanel( itemNotebook3, ID_LOG, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel40->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxVERTICAL);
    itemPanel40->SetSizer(itemBoxSizer41);

    wxFlexGridSizer* itemFlexGridSizer42 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer41->Add(itemFlexGridSizer42, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText43 = new wxStaticText( itemPanel40, wxID_STATIC, _("File Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer42->Add(itemStaticText43, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLogFileNameCtl = new wxTextCtrl( itemPanel40, ID_FILE_NAME, _T(""), wxDefaultPosition, wxSize(350, -1), 0 );
    itemFlexGridSizer42->Add(mLogFileNameCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer42->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLogEnableCtl = new wxCheckBox( itemPanel40, ID_LOG_ENABLE, _("Enable logging to file"), wxDefaultPosition, wxDefaultSize, 0 );
    mLogEnableCtl->SetValue(false);
    itemFlexGridSizer42->Add(mLogEnableCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(itemPanel40, wxID_ANY, _("Log Events"));
    wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxVERTICAL);
    itemBoxSizer41->Add(itemStaticBoxSizer47, 0, wxGROW|wxALL, 5);
    mLogErrorsAndWarningsCtl = new wxCheckBox( itemPanel40, ID_LOG_ERRORS, _("Errors and Warnings"), wxDefaultPosition, wxDefaultSize, 0 );
    mLogErrorsAndWarningsCtl->SetValue(false);
    itemStaticBoxSizer47->Add(mLogErrorsAndWarningsCtl, 0, wxALIGN_LEFT|wxALL, 5);

    mLogInfoAndNoticeCtl = new wxCheckBox( itemPanel40, ID_LOG_INFO, _("Informational log messages"), wxDefaultPosition, wxDefaultSize, 0 );
    mLogInfoAndNoticeCtl->SetValue(false);
    itemStaticBoxSizer47->Add(mLogInfoAndNoticeCtl, 0, wxALIGN_LEFT|wxALL, 5);

    mLogDebugCtl = new wxCheckBox( itemPanel40, ID_LOG_DEBUG, _("Debug messages"), wxDefaultPosition, wxDefaultSize, 0 );
    mLogDebugCtl->SetValue(false);
    itemStaticBoxSizer47->Add(mLogDebugCtl, 0, wxALIGN_LEFT|wxALL, 5);

    mLogDatabaseCtl = new wxCheckBox( itemPanel40, ID_LOG_DATABASE, _("Very verbose debug messages for database"), wxDefaultPosition, wxDefaultSize, 0 );
    mLogDatabaseCtl->SetValue(false);
    itemStaticBoxSizer47->Add(mLogDatabaseCtl, 0, wxALIGN_LEFT|wxALL, 5);

    mLogMtCtl = new wxCheckBox( itemPanel40, ID_LOG_MT, _("Very verbose debug messages for multithreading"), wxDefaultPosition, wxDefaultSize, 0 );
    mLogMtCtl->SetValue(false);
    itemStaticBoxSizer47->Add(mLogMtCtl, 0, wxALIGN_LEFT|wxALL, 5);

    itemNotebook3->AddPage(itemPanel40, _("Log"));

    wxPanel* itemPanel53 = new wxPanel( itemNotebook3, ID_EMAIL_TAB, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxVERTICAL);
    itemPanel53->SetSizer(itemBoxSizer54);

    wxStaticBox* itemStaticBoxSizer55Static = new wxStaticBox(itemPanel53, wxID_ANY, _("Server properties"));
    wxStaticBoxSizer* itemStaticBoxSizer55 = new wxStaticBoxSizer(itemStaticBoxSizer55Static, wxHORIZONTAL);
    itemBoxSizer54->Add(itemStaticBoxSizer55, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer56 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer55->Add(itemFlexGridSizer56, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText57 = new wxStaticText( itemPanel53, wxID_STATIC, _("Server address"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer56->Add(itemStaticText57, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mEmailServerCtl = new wxTextCtrl( itemPanel53, ID_EMAIL_SERVER_ADDRESS, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer56->Add(mEmailServerCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText59 = new wxStaticText( itemPanel53, wxID_STATIC, _("Return-address"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer56->Add(itemStaticText59, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mEmailReturnAddressCtl = new wxTextCtrl( itemPanel53, ID_RETURN_ADDRESS, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer56->Add(mEmailReturnAddressCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer61Static = new wxStaticBox(itemPanel53, wxID_ANY, _("Optional credentials on the mailserver (ESMTP CRAM-MD5)"));
    wxStaticBoxSizer* itemStaticBoxSizer61 = new wxStaticBoxSizer(itemStaticBoxSizer61Static, wxHORIZONTAL);
    itemBoxSizer54->Add(itemStaticBoxSizer61, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer62 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer61->Add(itemFlexGridSizer62, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText63 = new wxStaticText( itemPanel53, wxID_STATIC, _("User Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer62->Add(itemStaticText63, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mCramMd5UserNameCtl = new wxTextCtrl( itemPanel53, ID_TEXTCTRL4, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer62->Add(mCramMd5UserNameCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText65 = new wxStaticText( itemPanel53, wxID_STATIC, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer62->Add(itemStaticText65, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mCramMd5PaswdCtl = new wxTextCtrl( itemPanel53, ID_TEXTCTRL5, _T(""), wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD );
    itemFlexGridSizer62->Add(mCramMd5PaswdCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook3->AddPage(itemPanel53, _("Email"));

    itemBoxSizer2->Add(itemNotebook3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer67 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer67, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton68 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer67->AddButton(itemButton68);

    wxButton* itemButton69 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer67->AddButton(itemButton69);

    itemStdDialogButtonSizer67->Realize();

////@end PropertiesDlg content construction

    // Create custom windows not generated automatically here.
////@begin PropertiesDlg content initialisation
////@end PropertiesDlg content initialisation
}

/*!
 * Should we show tooltips?
 */

bool PropertiesDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap PropertiesDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon PropertiesDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin PropertiesDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end PropertiesDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TEST_CONNECTION
 */

void PropertiesDlg::OnTestConnectionClick( wxCommandEvent& event )
{
    WarDbConnection db;

    WarUtf8 database, user, passwd, host;

    database = mMysqlDatabaseCtl->GetValue().GetData();
    user = mMysqlUserCtl->GetValue().GetData();
    passwd = mMysqlPasswdCtl->GetValue().GetData();
    host = mMysqlHostCtl->GetValue().GetData();

    try
    {
        db.Open("mysql", database.GetUtf8().c_str(),
            host.GetUtf8().c_str(),
            user.GetUtf8().c_str(),
            passwd.GetUtf8().c_str());
    }
    catch(WarException)
    {
        return;
    }

    wxMessageBox(_("The connection to the database-server was successful"),
        _("MySQL Connection"), wxICON_INFORMATION | wxOK);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_TEST_CONNECTION
 */

void PropertiesDlg::OnTestConnectionUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!mMysqlHostCtl->GetValue().IsEmpty()
        && !mMysqlDatabaseCtl->GetValue().IsEmpty()
        && !mMysqlUserCtl->GetValue().IsEmpty());
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATE_DATABASE
 */

void PropertiesDlg::OnCreateDatabaseClick( wxCommandEvent& event )
{
    WarDbConnection db;

    WarUtf8 database, user, passwd, host;

    database = mMysqlDatabaseCtl->GetValue().GetData();
    user = mMysqlUserCtl->GetValue().GetData();
    passwd = mMysqlPasswdCtl->GetValue().GetData();
    host = mMysqlHostCtl->GetValue().GetData();

    MysqlCreateDatabaseDlg dlg;
    dlg.mDb = database.GetUtf8();
    dlg.mHost = host.GetUtf8();
    dlg.mUser = user.GetUtf8();
    dlg.mPasswd = passwd.GetUtf8();
    dlg.Create(this);

    try
    {
        if (dlg.ShowModal() != wxID_OK)
            return;
    }
    catch(WarException)
    {
        return;
    }

    wxMessageBox(_("The database was successfully created and initialized."),
        _("Database Creation"), wxICON_INFORMATION | wxOK);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_CREATE_DATABASE
 */

void PropertiesDlg::OnCreateDatabaseUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!mMysqlHostCtl->GetValue().IsEmpty()
        && !mMysqlDatabaseCtl->GetValue().IsEmpty()
        && !mMysqlUserCtl->GetValue().IsEmpty());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void PropertiesDlg::OnOKClick( wxCommandEvent& event )
{
    // Save

    #define INIT(path, control) { \
        std::wstring value = control->GetValue().GetData();\
        wxGetApp().mOptions.SetOption(path, value); \
    }

#define INITBOOL(path, control) { \
        bool value = control->GetValue();\
        wxGetApp().mOptions.SetOption(path, value); \
    }

#define INITINT(path, control) { \
        int value; WarUtf8 utf8 = control->GetValue().GetData();\
        value = atoi(utf8.GetUtf8().c_str());\
        wxGetApp().mOptions.SetOption(path, value); \
    }

    INIT("/log/file-name", mLogFileNameCtl);
    INITBOOL("/log/events/debug_mt", mLogMtCtl);
    INITBOOL("/log/events/error", mLogErrorsAndWarningsCtl);
    INITBOOL("/log/events/info", mLogInfoAndNoticeCtl);
    INITBOOL("/log/events/debug", mLogDebugCtl);
    INITBOOL("/log/events/debug_database", mLogDatabaseCtl);
    INITBOOL("/log/enable", mLogEnableCtl);

    INIT("/ice/address", mIceHostCtl);
    INITINT("/ice/port", mIcePortCtl);
    //INIT("/ice/gateway", );

    INIT("/db/sqlite/database", mSqliteNameCtl);

    INIT("/db/mysql/host", mMysqlHostCtl);
    INIT("/db/mysql/database", mMysqlDatabaseCtl);
    INIT("/db/mysql/user", mMysqlUserCtl);
    INIT("/db/mysql/passwd", mMysqlPasswdCtl);

    // Language
    {
        const int item = mLanguageCtl->GetSelection();
        if (0 <= item)
        {
            const WhidApp::lng_defs *p = (const WhidApp::lng_defs*)
                mLanguageCtl->GetClientData(item);
            wxGetApp().mOptions.SetOption("/preferences/language", p->mLnaguageNo);
        }
    }

    INIT("/preferences/browser", mBrowserCtl);

    int db_type = mDatabaseTypeCtl->GetSelection();
    std::string db_type_name;
    switch(db_type)
    {
    case DBTYPE_MYSQL:
        db_type_name = "mysql";
        break;
    default:
        db_type_name = "sqlite";
        break;
    }

    wxGetApp().mOptions.SetOption("/db/type", db_type_name);

    INIT("/email/server/address", mEmailServerCtl);
    INIT("/email/server/returnaddress", mEmailReturnAddressCtl);

    INIT("/email/auth/cram.md5/name", mCramMd5UserNameCtl);
    INIT("/email/auth/cram.md5/passwd", mCramMd5PaswdCtl);

#undef INIT
#undef INITBOOL
#undef INITINT

    wxGetApp().mOptions.Save();

    wxMessageBox(_("The changes will take effect the next time Whid is started."));
    EndModal(wxID_OK);
}


