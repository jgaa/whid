/////////////////////////////////////////////////////////////////////////////
// Name:        LogonDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/07/2007 10:25:03
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#include "libwhid.h"
#include "WhidApp.h"
#include <wxwidgets/WarWxValidator.h>

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "LogonDlg.h"
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

#include "LogonDlg.h"

////@begin XPM images
////@end XPM images


/*!
 * LogonDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LogonDlg, wxDialog )


/*!
 * LogonDlg event table definition
 */

BEGIN_EVENT_TABLE( LogonDlg, wxDialog )

////@begin LogonDlg event table entries
    EVT_BUTTON( wxID_OK, LogonDlg::OnOkClick )

////@end LogonDlg event table entries

END_EVENT_TABLE()


/*!
 * LogonDlg constructors
 */

LogonDlg::LogonDlg()
{
    Init();
}

LogonDlg::LogonDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * LogonDlg creator
 */

bool LogonDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin LogonDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end LogonDlg creation
    return true;
}


/*!
 * LogonDlg destructor
 */

LogonDlg::~LogonDlg()
{
////@begin LogonDlg destruction
////@end LogonDlg destruction
}


/*!
 * Member initialisation
 */

void LogonDlg::Init()
{
////@begin LogonDlg member initialisation
////@end LogonDlg member initialisation
}


/*!
 * Control creation for LogonDlg
 */

void LogonDlg::CreateControls()
{    
////@begin LogonDlg content construction
    LogonDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Login Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemTextCtrl5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText6, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_PASSWD, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
    itemFlexGridSizer3->Add(itemTextCtrl7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer8 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer8->AddButton(itemButton9);

    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer8->AddButton(itemButton10);

    itemStdDialogButtonSizer8->Realize();

    // Set validators
    itemTextCtrl5->SetValidator( WarWxValidator (mLogonName) );
    itemTextCtrl7->SetValidator( WarWxValidator (mPasswd) );
////@end LogonDlg content construction

    // Create custom windows not generated automatically here.
////@begin LogonDlg content initialisation
////@end LogonDlg content initialisation
}


/*!
 * Should we show tooltips?
 */

bool LogonDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap LogonDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon LogonDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LogonDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end LogonDlg icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void LogonDlg::OnOkClick( wxCommandEvent& event )
{
    if ( Validate() && TransferDataFromWindow() ) 
    {
        EndModal(wxID_OK); 
    }
}

