/////////////////////////////////////////////////////////////////////////////
// Name:        FirstUseDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     15/12/2006 18:27:31
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#include "libwhid.h"
#include "WhidApp.h"
#include <wxwidgets/WarWxValidator.h>

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "FirstUseDlg.h"
#endif

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

#include "FirstUseDlg.h"

////@begin XPM images
////@end XPM images

/*!
 * FirstUseDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FirstUseDlg, wxDialog )

/*!
 * FirstUseDlg event table definition
 */

BEGIN_EVENT_TABLE( FirstUseDlg, wxDialog )

////@begin FirstUseDlg event table entries
    EVT_BUTTON( wxID_OK, FirstUseDlg::OnOKClick )

////@end FirstUseDlg event table entries

END_EVENT_TABLE()

/*!
 * FirstUseDlg constructors
 */

FirstUseDlg::FirstUseDlg( )
{
    Init();
}

FirstUseDlg::FirstUseDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * Dialog creator
 */

bool FirstUseDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FirstUseDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end FirstUseDlg creation

    return true;
}

/*!
 * Member initialisation 
 */

void FirstUseDlg::Init()
{
////@begin FirstUseDlg member initialisation
    mLoginName = wxT("user");
    mFullName = wxT("Your Name");
    mCharge = 100;
    mFullNameCtl = NULL;
    mChargeCtl = NULL;
////@end FirstUseDlg member initialisation
}
/*!
 * Control creation for Dialog
 */

void FirstUseDlg::CreateControls()
{    
////@begin FirstUseDlg content construction
    FirstUseDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("I cannot find a database. If this is the first time the program is used by you, that is normal.\nI can create a new database. Just provide the infromation  below and press OK."), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("If you /do/ have a database, press the cancel button to abort."), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer5 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer5, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Login name"), wxDefaultPosition, wxDefaultSize, 0 );
    if (FirstUseDlg::ShowToolTips())
        itemStaticText6->SetToolTip(_("The login-name is normally composed from the users initials"));
    itemFlexGridSizer5->Add(itemStaticText6, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_LOGIN_NAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer5->Add(itemTextCtrl7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Your full name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer5->Add(itemStaticText8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFullNameCtl = new wxTextCtrl( itemDialog1, ID_USER_FUL_NAME, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemFlexGridSizer5->Add(mFullNameCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("What do you normally charge per hour?"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer5->Add(itemStaticText10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mChargeCtl = new wxTextCtrl( itemDialog1, ID_CHARGE_PER_HOUR, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (FirstUseDlg::ShowToolTips())
        mChargeCtl->SetToolTip(_("Just insert the amout (not the currency)"));
    itemFlexGridSizer5->Add(mChargeCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine12 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine12, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer13 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton14 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer13->AddButton(itemButton14);

    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer13->AddButton(itemButton15);

    itemStdDialogButtonSizer13->Realize();

    // Set validators
    itemTextCtrl7->SetValidator( WarWxValidator (mLoginName) );
    mFullNameCtl->SetValidator( WarWxValidator (mFullName) );
    mChargeCtl->SetValidator( wxGenericValidator(& mCharge) );
////@end FirstUseDlg content construction

    // Create custom windows not generated automatically here.
////@begin FirstUseDlg content initialisation
////@end FirstUseDlg content initialisation
}

/*!
 * Should we show tooltips?
 */

bool FirstUseDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FirstUseDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FirstUseDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FirstUseDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FirstUseDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void FirstUseDlg::OnOKClick( wxCommandEvent& event )
{
    wxString tmp = mFullNameCtl->GetValue();
    mFullName = tmp.GetData();
    
    tmp = mChargeCtl->GetValue();
    WarCollector<char> charge_tmp = tmp.GetData();
    mCharge = atoi(charge_tmp.GetValue().c_str());

    EndModal(wxID_OK);
}


