/////////////////////////////////////////////////////////////////////////////
// Name:        ImageContainer.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     17/12/2006 21:41:15
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#include "libwhid.h"
#include "WhidApp.h"


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ImageContainer.h"
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

#include "ImageContainer.h"

////@begin XPM images
////@end XPM images

/*!
 * Dialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Dialog, wxDialog )

/*!
 * Dialog event table definition
 */

BEGIN_EVENT_TABLE( Dialog, wxDialog )

////@begin Dialog event table entries
////@end Dialog event table entries

END_EVENT_TABLE()

/*!
 * Dialog constructors
 */

Dialog::Dialog( )
{
    Init();
}

Dialog::Dialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * Dialog creator
 */

bool Dialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Dialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Dialog creation
    return true;
}

/*!
 * Member initialisation 
 */

void Dialog::Init()
{
////@begin Dialog member initialisation
////@end Dialog member initialisation
}
/*!
 * Control creation for Dialog
 */

void Dialog::CreateControls()
{    
////@begin Dialog content construction
    Dialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBitmapButton* itemBitmapButton3 = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON1, itemDialog1->GetBitmapResource(wxT("icons/128x128/project.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer2->Add(itemBitmapButton3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBitmapButton* itemBitmapButton4 = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON2, itemDialog1->GetBitmapResource(wxT("icons/128x128/task.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer2->Add(itemBitmapButton4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBitmap* itemStaticBitmap5 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("icons/128x128/work.png")), wxDefaultPosition, wxSize(128, 128), 0 );
    itemBoxSizer2->Add(itemStaticBitmap5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end Dialog content construction

    // Create custom windows not generated automatically here.
////@begin Dialog content initialisation
////@end Dialog content initialisation
}

/*!
 * Should we show tooltips?
 */

bool Dialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Dialog::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin Dialog bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("icons/128x128/project.png"))
    {
        wxBitmap bitmap(_T("icons/128x128/project.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/128x128/task.png"))
    {
        wxBitmap bitmap(_T("icons/128x128/task.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/128x128/work.png"))
    {
        wxBitmap bitmap(_T("icons/128x128/work.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    return wxNullBitmap;
////@end Dialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Dialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Dialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Dialog icon retrieval
}
