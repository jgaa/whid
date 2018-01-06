/////////////////////////////////////////////////////////////////////////////
// Name:        EmailBillDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     28/07/2007 12:03:21
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "EmailBillDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"

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

#include "EmailBillDlg.h"

////@begin XPM images
////@end XPM images


/*!
 * EmailBillDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( EmailBillDlg, wxDialog )


/*!
 * EmailBillDlg event table definition
 */

BEGIN_EVENT_TABLE( EmailBillDlg, wxDialog )

////@begin EmailBillDlg event table entries
////@end EmailBillDlg event table entries

END_EVENT_TABLE()


/*!
 * EmailBillDlg constructors
 */

EmailBillDlg::EmailBillDlg()
{
    Init();
}

EmailBillDlg::EmailBillDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * EmailBillDlg creator
 */

bool EmailBillDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin EmailBillDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end EmailBillDlg creation
    return true;
}


/*!
 * EmailBillDlg destructor
 */

EmailBillDlg::~EmailBillDlg()
{
////@begin EmailBillDlg destruction
////@end EmailBillDlg destruction
}


/*!
 * Member initialisation
 */

void EmailBillDlg::Init()
{
////@begin EmailBillDlg member initialisation
////@end EmailBillDlg member initialisation
}


/*!
 * Control creation for EmailBillDlg
 */

void EmailBillDlg::CreateControls()
{    
////@begin EmailBillDlg content construction
    EmailBillDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Email address"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_EMAIL_RCPT, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemFlexGridSizer3->Add(itemTextCtrl5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Subject"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText6, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_SUBJECT, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemFlexGridSizer3->Add(itemTextCtrl7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Message"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl9 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL3, _T(""), wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE );
    itemFlexGridSizer3->Add(itemTextCtrl9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer10 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer10->AddButton(itemButton11);

    wxButton* itemButton12 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer10->AddButton(itemButton12);

    itemStdDialogButtonSizer10->Realize();

    // Set validators
    itemTextCtrl5->SetValidator( WarWxValidator (mEmail) );
    itemTextCtrl7->SetValidator( WarWxValidator (mSubject) );
    itemTextCtrl9->SetValidator( WarWxValidator (mMessage) );
////@end EmailBillDlg content construction

    // Create custom windows not generated automatically here.
////@begin EmailBillDlg content initialisation
////@end EmailBillDlg content initialisation
}


/*!
 * Should we show tooltips?
 */

bool EmailBillDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap EmailBillDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon EmailBillDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin EmailBillDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end EmailBillDlg icon retrieval
}
