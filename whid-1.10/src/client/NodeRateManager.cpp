/////////////////////////////////////////////////////////////////////////////
// Name:        NodeRateManager.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     19/12/2006 01:12:36
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "NodeRateManager.h"
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

#include "NodeRateManager.h"

////@begin XPM images
////@end XPM images

/*!
 * NodeRateManager type definition
 */

IMPLEMENT_DYNAMIC_CLASS( NodeRateManager, RateManager )

/*!
 * NodeRateManager event table definition
 */

BEGIN_EVENT_TABLE( NodeRateManager, RateManager )

////@begin NodeRateManager event table entries
////@end NodeRateManager event table entries

END_EVENT_TABLE()

/*!
 * NodeRateManager constructors
 */

NodeRateManager::NodeRateManager( )
{
    Init();
}

NodeRateManager::NodeRateManager( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * NodeRateManager creator
 */

bool NodeRateManager::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
 ///@begin NodeRateManager creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    RateManager::Create(parent, id, pos, size, style);
    CreateControls();
////@end NodeRateManager creation
    return true;
}

/*!
 * Member initialisation 
 */

void NodeRateManager::Init()
{
////@begin NodeRateManager member initialisation
////@end NodeRateManager member initialisation
}
/*!
 * Control creation for NodeRateManager
 */

void NodeRateManager::CreateControls()
{    
////@begin NodeRateManager content construction
////@end NodeRateManager content construction
}

/*!
 * Should we show tooltips?
 */

bool NodeRateManager::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap NodeRateManager::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon NodeRateManager::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin NodeRateManager icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end NodeRateManager icon retrieval
}
