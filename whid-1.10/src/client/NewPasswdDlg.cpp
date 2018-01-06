/////////////////////////////////////////////////////////////////////////////
// Name:        NewPasswdDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     23/07/2007 11:36:21
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "NewPasswdDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <wxwidgets/WarWxValidator.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "NewPasswdDlg.h"

////@begin XPM images
////@end XPM images


/*!
 * NewPasswdDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( NewPasswdDlg, wxDialog )


/*!
 * NewPasswdDlg event table definition
 */

BEGIN_EVENT_TABLE( NewPasswdDlg, wxDialog )

////@begin NewPasswdDlg event table entries
////@end NewPasswdDlg event table entries

END_EVENT_TABLE()


/*!
 * NewPasswdDlg constructors
 */

NewPasswdDlg::NewPasswdDlg()
{
    Init();
}

NewPasswdDlg::NewPasswdDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * NewPasswdDlg creator
 */

bool NewPasswdDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin NewPasswdDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end NewPasswdDlg creation
    return true;
}


/*!
 * NewPasswdDlg destructor
 */

NewPasswdDlg::~NewPasswdDlg()
{
////@begin NewPasswdDlg destruction
////@end NewPasswdDlg destruction
}


/*!
 * Member initialisation
 */

void NewPasswdDlg::Init()
{
////@begin NewPasswdDlg member initialisation
////@end NewPasswdDlg member initialisation
}


/*!
 * Control creation for NewPasswdDlg
 */

void NewPasswdDlg::CreateControls()
{    
////@begin NewPasswdDlg content construction
    NewPasswdDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("New password"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_PASSWD, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
    itemFlexGridSizer3->Add(itemTextCtrl5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton8);

    itemStdDialogButtonSizer6->Realize();

    // Set validators
    itemTextCtrl5->SetValidator( WarWxValidator (mPasswd) );
////@end NewPasswdDlg content construction

    // Create custom windows not generated automatically here.
////@begin NewPasswdDlg content initialisation
////@end NewPasswdDlg content initialisation
}


/*!
 * Should we show tooltips?
 */

bool NewPasswdDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap NewPasswdDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon NewPasswdDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin NewPasswdDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end NewPasswdDlg icon retrieval
}
