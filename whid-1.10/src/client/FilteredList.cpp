/////////////////////////////////////////////////////////////////////////////
// Name:        FilteredList.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     14/12/2006 18:03:48
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#include "libwhid.h"
#include "WhidApp.h"


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "FilteredList.h"
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

#include "FilteredList.h"

////@begin XPM images

////@end XPM images

/*!
 * FilteredList type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FilteredList, wxPanel )

/*!
 * FilteredList event table definition
 */

BEGIN_EVENT_TABLE( FilteredList, wxPanel )

////@begin FilteredList event table entries
////@end FilteredList event table entries

END_EVENT_TABLE()

/*!
 * FilteredList constructors
 */

FilteredList::FilteredList( )
{
    Init();
}

FilteredList::FilteredList( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * FilteredList creator
 */

bool FilteredList::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FilteredList creation
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end FilteredList creation
    return true;
}

/*!
 * Member initialisation 
 */

void FilteredList::Init()
{
////@begin FilteredList member initialisation
////@end FilteredList member initialisation
}
/*!
 * Control creation for FilteredList
 */

void FilteredList::CreateControls()
{    
////@begin FilteredList content construction
    if (!wxXmlResource::Get()->LoadPanel(this, GetParent(), _T("ID_FILTEREDLIST")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
////@end FilteredList content construction

    // Create custom windows not generated automatically here.
////@begin FilteredList content initialisation
////@end FilteredList content initialisation
}

/*!
 * Should we show tooltips?
 */

bool FilteredList::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FilteredList::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin FilteredList bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end FilteredList bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FilteredList::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FilteredList icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FilteredList icon retrieval
}
