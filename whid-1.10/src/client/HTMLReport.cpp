/////////////////////////////////////////////////////////////////////////////
// Name:        HTMLReport.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     06/08/2007 11:01:15
// RCS-ID:      
// Copyright:   Copyright 2007 Jarle (jgaa) Aase All Rights Reserved
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "HTMLReport.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"

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

#include "HTMLReport.h"

////@begin XPM images
////@end XPM images


/*!
 * HTMLReport type definition
 */

IMPLEMENT_DYNAMIC_CLASS( HTMLReport, wxDialog )


/*!
 * HTMLReport event table definition
 */

BEGIN_EVENT_TABLE( HTMLReport, wxDialog )

////@begin HTMLReport event table entries
    EVT_BUTTON( wxID_OK, HTMLReport::OnOKClick )

////@end HTMLReport event table entries

END_EVENT_TABLE()


/*!
 * HTMLReport constructors
 */

HTMLReport::HTMLReport()
{
    Init();
}

HTMLReport::HTMLReport( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * HTMLReport creator
 */

bool HTMLReport::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin HTMLReport creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end HTMLReport creation
    return true;
}


/*!
 * HTMLReport destructor
 */

HTMLReport::~HTMLReport()
{
////@begin HTMLReport destruction
////@end HTMLReport destruction
}


/*!
 * Member initialisation
 */

void HTMLReport::Init()
{
////@begin HTMLReport member initialisation
    mReport = 0;
    mReportCtl = NULL;
////@end HTMLReport member initialisation
}


/*!
 * Control creation for HTMLReport
 */

void HTMLReport::CreateControls()
{    
////@begin HTMLReport content construction
    HTMLReport* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Choose report"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mReportCtlStrings;
    mReportCtl = new wxListBox( itemDialog1, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, mReportCtlStrings, wxLB_SINGLE );
    itemFlexGridSizer3->Add(mReportCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton8);

    itemStdDialogButtonSizer6->Realize();

////@end HTMLReport content construction

    WarPath<char> invoice2html = wxGetApp().GetXmlDir("whid_invoice2html.xsl");
    WarPath<char> invoice2html_internal = wxGetApp().GetXmlDir("whid_invoice2html_internal.xsl");

    mFiles.push_back(invoice2html.GetPath());
    mFiles.push_back(invoice2html_internal.GetPath());

    mReportCtl->Append(_("Invoice"));
    mReportCtl->Append(_("Internal report with cost and profit"));
    mReportCtl->SetSelection(0);
}


/*!
 * Should we show tooltips?
 */

bool HTMLReport::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap HTMLReport::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon HTMLReport::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin HTMLReport icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end HTMLReport icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void HTMLReport::OnOKClick( wxCommandEvent& event )
{
    if ((mReport = mReportCtl->GetSelection()) < 0)
        mReport = 0;

    EndModal(wxID_OK); 
}

