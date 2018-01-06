/////////////////////////////////////////////////////////////////////////////
// Name:        MysqlCreateDatabaseDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     31/12/2006 21:30:52
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MysqlCreateDatabaseDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"
#include <WarUtf8.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <database/drivers/WarDbDriverMysql.h>
#include <wxwidgets/WarWxValidator.h>

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

#include "MysqlCreateDatabaseDlg.h"

////@begin XPM images
////@end XPM images

/*!
 * MysqlCreateDatabaseDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MysqlCreateDatabaseDlg, wxDialog )

/*!
 * MysqlCreateDatabaseDlg event table definition
 */

BEGIN_EVENT_TABLE( MysqlCreateDatabaseDlg, wxDialog )

////@begin MysqlCreateDatabaseDlg event table entries
    EVT_BUTTON( ID_CREATE_MYSQL_DATABASE, MysqlCreateDatabaseDlg::OnCreateDatabaseClick )

////@end MysqlCreateDatabaseDlg event table entries

END_EVENT_TABLE()

/*!
 * MysqlCreateDatabaseDlg constructors
 */

MysqlCreateDatabaseDlg::MysqlCreateDatabaseDlg( )
{
    Init();
}

MysqlCreateDatabaseDlg::MysqlCreateDatabaseDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * MysqlCreateDatabaseDlg creator
 */

bool MysqlCreateDatabaseDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MysqlCreateDatabaseDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end MysqlCreateDatabaseDlg creation

    return true;
}

/*!
 * Member initialisation 
 */

void MysqlCreateDatabaseDlg::Init()
{
////@begin MysqlCreateDatabaseDlg member initialisation
    mLoginName = wxT("user");
    mUserName = wxT("Your Name");
    mCharge = 100;
////@end MysqlCreateDatabaseDlg member initialisation
}
/*!
 * Control creation for MysqlCreateDatabaseDlg
 */

void MysqlCreateDatabaseDlg::CreateControls()
{    
////@begin MysqlCreateDatabaseDlg content construction
    MysqlCreateDatabaseDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("MySQL Logon properties"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("In order to create the database, you must provide a\nuser-name and a password to the MySQL database,\nfor a user with access  to create databases and grant\nprivileges. "), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer6 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer4->Add(itemFlexGridSizer6, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Super user name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer6->Add(itemStaticText7, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemDialog1, ID_SUPERUSER_NAME, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer6->Add(itemTextCtrl8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer6->Add(itemStaticText9, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl10 = new wxTextCtrl( itemDialog1, ID_SUPERUSER_PWD, _T(""), wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD );
    itemFlexGridSizer6->Add(itemTextCtrl10, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Default user"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer11, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer11->Add(itemBoxSizer12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("I will add one user to the WHID database. This user will normally\nbe you. Just enter yor name and your normal charge per hour."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer14 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer12->Add(itemFlexGridSizer14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemDialog1, wxID_STATIC, _("Login Name (initials)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemStaticText15, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl16 = new wxTextCtrl( itemDialog1, ID_LOGIN_NAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemTextCtrl16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("Your Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl18 = new wxTextCtrl( itemDialog1, ID_WHID_USER, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer14->Add(itemTextCtrl18, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemDialog1, wxID_STATIC, _("Normal charge per hour"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemStaticText19, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl20 = new wxTextCtrl( itemDialog1, ID_WHID_USER_CHARGE, _T(""), wxDefaultPosition, wxSize(100, -1), 0 );
    itemFlexGridSizer14->Add(itemTextCtrl20, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton22 = new wxButton( itemDialog1, ID_CREATE_MYSQL_DATABASE, _("Create the database"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer21->Add(itemButton22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Set validators
    itemTextCtrl8->SetValidator( WarWxValidator (mSuperUserName) );
    itemTextCtrl10->SetValidator( WarWxValidator (mSuperUserPasswd) );
    itemTextCtrl16->SetValidator( WarWxValidator (mLoginName) );
    itemTextCtrl18->SetValidator( WarWxValidator (mUserName) );
    itemTextCtrl20->SetValidator( wxGenericValidator(& mCharge) );
////@end MysqlCreateDatabaseDlg content construction

    // Create custom windows not generated automatically here.
////@begin MysqlCreateDatabaseDlg content initialisation
////@end MysqlCreateDatabaseDlg content initialisation
}

/*!
 * Should we show tooltips?
 */

bool MysqlCreateDatabaseDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap MysqlCreateDatabaseDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon MysqlCreateDatabaseDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MysqlCreateDatabaseDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MysqlCreateDatabaseDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATE_DATABASE
 */

void MysqlCreateDatabaseDlg::OnCreateDatabaseClick( wxCommandEvent& event )
{
    WarDbConnection db;
    bool warn_grant_host = false;

    TransferDataFromWindow();

    // Connect to the database
    db.Open("mysql", "mysql",
            mHost.c_str(),
            mSuperUserName.GetUtf8().c_str(),
            mSuperUserPasswd.GetUtf8().c_str());

    WarDbDriverMysql *mysql = db.GetDriver().CastToPtr<WarDbDriverMysql>();

    // Create the database
    {
        std::stringstream query;
        query << "CREATE DATABASE `" << mDb << "`";
        mysql->ExecDirect(query.str().c_str());
    }

    // Set access privileges
    try
    {
        std::stringstream query;
        query << "GRANT ALL PRIVILEGES ON `" << mDb << "`.*"
            << " TO `" << mUser << "`@`";

        if ((mHost == "localhost") || (mHost == "127.0.0.1"))
            query << "localhost";
        else
        {
            warn_grant_host = true;
            query << "%";
        }

        std::string escaped_passwd;
        mysql->EscapeString(mPasswd, escaped_passwd);

        query << "` IDENTIFIED BY '" << escaped_passwd << "'";

        mysql->ExecDirect(query.str().c_str());

        mysql->ExecDirect("FLUSH PRIVILEGES");
    }
    catch(WarException)
    {
        WarCollector<wchar_t> msg;
        msg << "Failed to grant privileges to the specified user.\r\n"
            << "You will have to grant the permissions to use the database\r\n"
            << "manually, using the MySQL admin tools.";

        wxMessageBox(msg.GetValue().c_str());
    }

    if (warn_grant_host)
    {
        WarCollector<wchar_t> msg;
        msg << "I have granted the user \""
            << mUser
            << "\"\r\naccess to the database \""
            << mDb
            << "\"\r\n from *any* host. If your network "
            << "is insecure, you should use the MySQL\r\n"
            << "admin tools and tighten the security to\r\n"
            << "only allow access from required hosts.";

        wxMessageBox(msg.GetValue().c_str());
    }

    // Initialize the database
    double charge = (double)mCharge;

    {
        WarDbConnection client_db;
        client_db.Open("mysql", 
            mDb.c_str(),
            mHost.c_str(),
            mUser.c_str(),
            mPasswd.c_str());

        war_syspath_t path = wxGetApp().GetDbDir();

        WhidEngine::GetEngine().CreateAndBootstrap(
            mUserName, mLoginName, charge, client_db);
    }

    EndModal(wxID_OK);
}

