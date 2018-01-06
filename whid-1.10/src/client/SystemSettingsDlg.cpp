/////////////////////////////////////////////////////////////////////////////
// Name:        SystemSettingsDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     05/07/2007 19:44:49
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "SystemSettingsDlg.h"
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

#include "SystemSettingsDlg.h"

////@begin XPM images
////@end XPM images


/*!
 * SystemSettingsDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SystemSettingsDlg, wxDialog )


/*!
 * SystemSettingsDlg event table definition
 */

BEGIN_EVENT_TABLE( SystemSettingsDlg, wxDialog )

////@begin SystemSettingsDlg event table entries
    EVT_BUTTON( wxID_OK, SystemSettingsDlg::OnOKClick )

////@end SystemSettingsDlg event table entries

END_EVENT_TABLE()


/*!
 * SystemSettingsDlg constructors
 */

SystemSettingsDlg::SystemSettingsDlg()
{
    WAR_ASSERT(false); // Don't use this constructor
    Init();
}

SystemSettingsDlg::SystemSettingsDlg( wxWindow* parent, 
                                     WHID::Options & options,
                                     wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
                                     : mOptions(options)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * SystemSettingsDlg creator
 */

bool SystemSettingsDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SystemSettingsDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SystemSettingsDlg creation
    return true;
}


/*!
 * SystemSettingsDlg destructor
 */

SystemSettingsDlg::~SystemSettingsDlg()
{
////@begin SystemSettingsDlg destruction
////@end SystemSettingsDlg destruction
}


/*!
 * Member initialisation
 */

void SystemSettingsDlg::Init()
{
////@begin SystemSettingsDlg member initialisation
////@end SystemSettingsDlg member initialisation
}


/*!
 * Control creation for SystemSettingsDlg
 */

void SystemSettingsDlg::CreateControls()
{    
////@begin SystemSettingsDlg content construction
    SystemSettingsDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxNotebook* itemNotebook3 = new wxNotebook( itemDialog1, ID_SYSTEM_SETTINGS_TAB, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

    wxPanel* itemPanel4 = new wxPanel( itemNotebook3, ID_SYSTEM_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel4->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxFlexGridSizer* itemFlexGridSizer6 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer5->Add(itemFlexGridSizer6, 0, wxALIGN_LEFT, 5);
    itemFlexGridSizer6->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox8 = new wxCheckBox( itemPanel4, ID_MULTIUSER, _("Multiuser mode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox8->SetValue(false);
    if (SystemSettingsDlg::ShowToolTips())
        itemCheckBox8->SetToolTip(_("Enables multiuser-operation"));
    itemFlexGridSizer6->Add(itemCheckBox8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook3->AddPage(itemPanel4, _("Settings"));

    itemBoxSizer2->Add(itemNotebook3, 0, wxGROW, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer9 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer9->AddButton(itemButton10);

    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer9->AddButton(itemButton11);

    itemStdDialogButtonSizer9->Realize();

    // Set validators
    itemCheckBox8->SetValidator( wxGenericValidator(& mOptions.mMultiuser) );
////@end SystemSettingsDlg content construction

    // Create custom windows not generated automatically here.
////@begin SystemSettingsDlg content initialisation
////@end SystemSettingsDlg content initialisation
}


/*!
 * Should we show tooltips?
 */

bool SystemSettingsDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SystemSettingsDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon SystemSettingsDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SystemSettingsDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SystemSettingsDlg icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SystemSettingsDlg::OnOKClick( wxCommandEvent& event )
{
    /*SetReturnCode(wxID_OK);
    Close();*/

    TransferDataFromWindow();

    EndModal(wxID_OK);
}

