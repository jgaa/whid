/////////////////////////////////////////////////////////////////////////////
// Name:        FilteredList.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     14/12/2006 18:03:48
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FILTEREDLIST_H_
#define _FILTEREDLIST_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "FilteredList.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/toolbar.h"
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FILTEREDLIST 10005
#define SYMBOL_FILTEREDLIST_STYLE 0
#define SYMBOL_FILTEREDLIST_TITLE _("FilteredList")
#define SYMBOL_FILTEREDLIST_IDNAME ID_FILTEREDLIST
#define SYMBOL_FILTEREDLIST_SIZE wxSize(600, 300)
#define SYMBOL_FILTEREDLIST_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * FilteredList class declaration
 */

class FilteredList: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( FilteredList )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FilteredList( );
    FilteredList( wxWindow* parent, wxWindowID id = SYMBOL_FILTEREDLIST_IDNAME, const wxPoint& pos = SYMBOL_FILTEREDLIST_POSITION, const wxSize& size = SYMBOL_FILTEREDLIST_SIZE, long style = SYMBOL_FILTEREDLIST_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FILTEREDLIST_IDNAME, const wxPoint& pos = SYMBOL_FILTEREDLIST_POSITION, const wxSize& size = SYMBOL_FILTEREDLIST_SIZE, long style = SYMBOL_FILTEREDLIST_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin FilteredList event handler declarations

////@end FilteredList event handler declarations

////@begin FilteredList member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end FilteredList member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FilteredList member variables
////@end FilteredList member variables
};

#endif
    // _FILTEREDLIST_H_
