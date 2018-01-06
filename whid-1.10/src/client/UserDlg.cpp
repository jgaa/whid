/////////////////////////////////////////////////////////////////////////////
// Name:        UserDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     23/07/2007 10:35:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "UserDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"
#include "MainFrame.h"
#include <ice/WarIceException.h>
#include <wxwidgets/WarWxValidator.h>
#include "NewPasswdDlg.h"

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

#include "UserDlg.h"

////@begin XPM images
////@end XPM images


/*!
 * UserDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( UserDlg, wxDialog )


/*!
 * UserDlg event table definition
 */

BEGIN_EVENT_TABLE( UserDlg, wxDialog )

////@begin UserDlg event table entries
    EVT_BUTTON( ID_RESET_PASSWORD_BTN, UserDlg::OnRESETPASSWORDBTNClick )

    EVT_BUTTON( wxID_OK, UserDlg::OnOKClick )

////@end UserDlg event table entries

END_EVENT_TABLE()


/*!
 * UserDlg constructors
 */

UserDlg::UserDlg()
{
    Init();
}

//UserDlg::UserDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
//{
//    WAR_ASSERT(false);
//    
//}


/*!
 * UserDlg creator
 */

bool UserDlg::Create(WHID::WhidControlPrx ctl, 
                     WHID::UserPrx userPrx, 
                     wxWindow* parent, 
                     wxWindowID id, 
                     const wxString& caption, 
                     const wxPoint& pos, 
                     const wxSize& size, 
                     long style )
{
    mUserPrx = userPrx;
    mWhidControl = ctl;
    if (mUserPrx)
    {
        try
        {
            mUserPrx->GetData(mUserData, CTX);
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Caught exception: " << ex << war_endl;
            return false;
        }
    }
////@begin UserDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end UserDlg creation

    

    TransferDataToWindow();

    return true;
}


/*!
 * UserDlg destructor
 */

UserDlg::~UserDlg()
{
////@begin UserDlg destruction
////@end UserDlg destruction
}


/*!
 * Member initialisation
 */

void UserDlg::Init()
{
////@begin UserDlg member initialisation
    mPasswdSizer = NULL;
    mPasswdCtl = NULL;
    mPassdResetCtl = NULL;
////@end UserDlg member initialisation
}


/*!
 * Control creation for UserDlg
 */

void UserDlg::CreateControls()
{    
////@begin UserDlg content construction
    UserDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBitmap* itemStaticBitmap3 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("icons/128x128/user.png")), wxDefaultPosition, wxSize(128, 128), 0 );
    itemBoxSizer2->Add(itemStaticBitmap3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL, 5);

    wxNotebook* itemNotebook5 = new wxNotebook( itemDialog1, ID_NOTEBOOK3, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

    wxPanel* itemPanel6 = new wxPanel( itemNotebook5, ID_USERPROPERTIES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel6->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
    itemPanel6->SetSizer(itemFlexGridSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel6, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl9 = new wxTextCtrl( itemPanel6, ID_USER_NAME, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemFlexGridSizer7->Add(itemTextCtrl9, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel6, wxID_STATIC, _("Login Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl11 = new wxTextCtrl( itemPanel6, ID_LOGIN_NAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemTextCtrl11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel6, wxID_STATIC, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText12, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPasswdSizer = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer7->Add(mPasswdSizer, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    mPasswdCtl = new wxTextCtrl( itemPanel6, ID_PASSWD_INPUT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
    mPasswdSizer->Add(mPasswdCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPassdResetCtl = new wxButton( itemPanel6, ID_RESET_PASSWORD_BTN, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
    mPasswdSizer->Add(mPassdResetCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer7->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox17 = new wxCheckBox( itemPanel6, ID_ACTIVE, _("Active"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox17->SetValue(false);
    itemFlexGridSizer7->Add(itemCheckBox17, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer7->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox19 = new wxCheckBox( itemPanel6, ID_ADMIN, _("Administrator privileges"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox19->SetValue(false);
    itemFlexGridSizer7->Add(itemCheckBox19, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel6, wxID_STATIC, _("Notes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText20, 0, wxALIGN_RIGHT|wxALIGN_TOP|wxALL, 5);

    wxTextCtrl* itemTextCtrl21 = new wxTextCtrl( itemPanel6, ID_NOTES, _T(""), wxDefaultPosition, wxSize(250, 100), wxTE_MULTILINE );
    itemFlexGridSizer7->Add(itemTextCtrl21, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook5->AddPage(itemPanel6, _("Properties"));

    wxPanel* itemPanel22 = new wxPanel( itemNotebook5, ID_CONTACT_INFO, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer23 = new wxFlexGridSizer(2, 2, 0, 0);
    itemPanel22->SetSizer(itemFlexGridSizer23);

    wxStaticText* itemStaticText24 = new wxStaticText( itemPanel22, wxID_STATIC, _("Email"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer23->Add(itemStaticText24, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl25 = new wxTextCtrl( itemPanel22, ID_EMAIL, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer23->Add(itemTextCtrl25, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook5->AddPage(itemPanel22, _("Contact Info"));

    itemBoxSizer4->Add(itemNotebook5, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer26 = new wxStdDialogButtonSizer;

    itemBoxSizer4->Add(itemStdDialogButtonSizer26, 0, wxALIGN_LEFT|wxALL, 5);
    wxButton* itemButton27 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer26->AddButton(itemButton27);

    wxButton* itemButton28 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer26->AddButton(itemButton28);

    itemStdDialogButtonSizer26->Realize();

    // Set validators
    itemTextCtrl9->SetValidator( WarWxValidator (mUserData.mName) );
    itemTextCtrl11->SetValidator( WarWxValidator (mUserData.mLoginName) );
    mPasswdCtl->SetValidator( WarWxValidator (mPasswd) );
    itemCheckBox17->SetValidator( wxGenericValidator(& mUserData.mActive) );
    itemCheckBox19->SetValidator( wxGenericValidator(& mUserData.mAdmin) );
    itemTextCtrl21->SetValidator( WarWxValidator (mUserData.mNotes) );
    itemTextCtrl25->SetValidator( WarWxValidator (mUserData.mEmail) );
////@end UserDlg content construction

    // Create custom windows not generated automatically here.
////@begin UserDlg content initialisation
////@end UserDlg content initialisation

    if (mUserPrx)
    {
        mPasswdCtl->Enable(false);
        mPassdResetCtl->Enable(true);
    }
    else
    {
        mPasswdCtl->Enable(true);
        mPassdResetCtl->Enable(false);
    }

    //mPasswdSizer->RecalcSizes();
}


/*!
 * Should we show tooltips?
 */

bool UserDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap UserDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon UserDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin UserDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end UserDlg icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESET_PASSWORD_BTN
 */

void UserDlg::OnRESETPASSWORDBTNClick( wxCommandEvent& event )
{
    // Show password dialog
    NewPasswdDlg dlg(this);
    if (dlg.ShowModal() == wxID_OK)
    {

        // Update

        mPasswd = dlg.mPasswd;

        try
        {
            mUserPrx->SetPassword(mPasswd, CTX);
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Caught exception: " << ex << war_endl;
        }
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void UserDlg::OnOKClick( wxCommandEvent& event )
{
    if (!TransferDataFromWindow())
        return;

    try
    {
        if (mUserPrx)
        {
            // Update user
            mUserPrx->SetData(mUserData, CTX);
        }
        else
        {
            // New user
            mWhidControl->AddUser(mUserData, mUserPrx, CTX);
            mUserPrx->SetPassword(mPasswd, CTX);
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught exception: " << ex << war_endl;
        return;
    }

    EndModal(wxID_OK);
}


