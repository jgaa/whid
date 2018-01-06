/////////////////////////////////////////////////////////////////////////////
// Name:        NodeRateManager.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     19/12/2006 01:12:36
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NODERATEMANAGER_H_
#define _NODERATEMANAGER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "NodeRateManager.h"
#endif


/*!
 * Includes
 */

#include "RateManager.h"

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class NodeRateManager;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_NODERATEMANAGER_STYLE wxTAB_TRAVERSAL
#define SYMBOL_NODERATEMANAGER_IDNAME ID_NODE_RATES
#define SYMBOL_NODERATEMANAGER_SIZE wxDefaultSize
#define SYMBOL_NODERATEMANAGER_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * NodeRateManager class declaration
 */

class NodeRateManager: public RateManager
{    
    DECLARE_DYNAMIC_CLASS( NodeRateManager )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    NodeRateManager( );
    NodeRateManager( wxWindow* parent, wxWindowID id = SYMBOL_NODERATEMANAGER_IDNAME, const wxPoint& pos = SYMBOL_NODERATEMANAGER_POSITION, const wxSize& size = SYMBOL_NODERATEMANAGER_SIZE, long style = SYMBOL_NODERATEMANAGER_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_NODERATEMANAGER_IDNAME, const wxPoint& pos = SYMBOL_NODERATEMANAGER_POSITION, const wxSize& size = SYMBOL_NODERATEMANAGER_SIZE, long style = SYMBOL_NODERATEMANAGER_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin NodeRateManager event handler declarations

////@end NodeRateManager event handler declarations

////@begin NodeRateManager member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end NodeRateManager member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin NodeRateManager member variables
    /// Control identifiers
    enum {
        ID_NODE_RATES = 10029
    };
////@end NodeRateManager member variables
};

#endif
    // _NODERATEMANAGER_H_
