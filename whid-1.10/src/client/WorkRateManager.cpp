/////////////////////////////////////////////////////////////////////////////
// Name:        WorkRateManager.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     19/12/2006 00:51:19
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WorkRateManager.h"
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

#include "WorkRateManager.h"

////@begin XPM images
////@end XPM images

/*!
 * WorkRateManager type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WorkRateManager, RateManager )

/*!
 * WorkRateManager event table definition
 */

BEGIN_EVENT_TABLE( WorkRateManager, RateManager )

////@begin WorkRateManager event table entries
////@end WorkRateManager event table entries

END_EVENT_TABLE()

/*!
 * WorkRateManager constructors
 */

WorkRateManager::WorkRateManager( )
{
    Init();
}

WorkRateManager::WorkRateManager( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * WorkRateManager creator
 */

bool WorkRateManager::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WorkRateManager creation
    RateManager::Create(parent, id, pos, size, style);
    CreateControls();
////@end WorkRateManager creation
    return true;
}

/*!
 * Member initialisation 
 */

void WorkRateManager::Init()
{
////@begin WorkRateManager member initialisation
////@end WorkRateManager member initialisation
}
/*!
 * Control creation for WorkRateManager
 */

void WorkRateManager::CreateControls()
{    
////@begin WorkRateManager content construction
////@end WorkRateManager content construction
}

/*!
 * Should we show tooltips?
 */

bool WorkRateManager::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WorkRateManager::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WorkRateManager::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WorkRateManager icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WorkRateManager icon retrieval
}
