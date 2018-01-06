/////////////////////////////////////////////////////////////////////////////
// Name:        WorkRateManager.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     19/12/2006 00:51:19
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WORKRATEMANAGER_H_
#define _WORKRATEMANAGER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "WorkRateManager.h"
#endif

#include "RateManager.h"

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class WorkRateManager;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WORKRATEMANAGER_STYLE wxTAB_TRAVERSAL
#define SYMBOL_WORKRATEMANAGER_IDNAME ID_RATES
#define SYMBOL_WORKRATEMANAGER_SIZE wxDefaultSize
#define SYMBOL_WORKRATEMANAGER_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * WorkRateManager class declaration
 */

class WorkRateManager: public RateManager
{    
    DECLARE_DYNAMIC_CLASS( WorkRateManager )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WorkRateManager( );
    WorkRateManager( wxWindow* parent, wxWindowID id = SYMBOL_WORKRATEMANAGER_IDNAME, const wxPoint& pos = SYMBOL_WORKRATEMANAGER_POSITION, const wxSize& size = SYMBOL_WORKRATEMANAGER_SIZE, long style = SYMBOL_WORKRATEMANAGER_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WORKRATEMANAGER_IDNAME, const wxPoint& pos = SYMBOL_WORKRATEMANAGER_POSITION, const wxSize& size = SYMBOL_WORKRATEMANAGER_SIZE, long style = SYMBOL_WORKRATEMANAGER_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WorkRateManager event handler declarations

////@end WorkRateManager event handler declarations

////@begin WorkRateManager member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WorkRateManager member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WorkRateManager member variables
    /// Control identifiers
    enum {
        ID_RATES = 10076
    };
////@end WorkRateManager member variables
};

#endif
    // _WORKRATEMANAGER_H_
