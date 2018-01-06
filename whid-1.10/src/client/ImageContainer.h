/////////////////////////////////////////////////////////////////////////////
// Name:        ImageContainer.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     17/12/2006 21:41:15
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _IMAGECONTAINER_H_
#define _IMAGECONTAINER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ImageContainer.h"
#endif

/*!
 * Includes
 */

////@begin includes
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
#define SYMBOL_DIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_DIALOG_TITLE _("Dialog")
#define SYMBOL_DIALOG_IDNAME ID_DIALOG
#define SYMBOL_DIALOG_SIZE wxSize(400, 300)
#define SYMBOL_DIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * Dialog class declaration
 */

class Dialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( Dialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Dialog( );
    Dialog( wxWindow* parent, wxWindowID id = SYMBOL_DIALOG_IDNAME, const wxString& caption = SYMBOL_DIALOG_TITLE, const wxPoint& pos = SYMBOL_DIALOG_POSITION, const wxSize& size = SYMBOL_DIALOG_SIZE, long style = SYMBOL_DIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DIALOG_IDNAME, const wxString& caption = SYMBOL_DIALOG_TITLE, const wxPoint& pos = SYMBOL_DIALOG_POSITION, const wxSize& size = SYMBOL_DIALOG_SIZE, long style = SYMBOL_DIALOG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Dialog event handler declarations

////@end Dialog event handler declarations

////@begin Dialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Dialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Dialog member variables
    /// Control identifiers
    enum {
        ID_DIALOG = 10037,
        ID_BITMAPBUTTON1 = 10038,
        ID_BITMAPBUTTON2 = 10039
    };
////@end Dialog member variables
};

#endif
    // _IMAGECONTAINER_H_
