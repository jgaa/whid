/////////////////////////////////////////////////////////////////////////////
// Name:        SystemSettingsDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     05/07/2007 19:44:49
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMSETTINGSDLG_H_
#define _SYSTEMSETTINGSDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "SystemSettingsDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/valgen.h"
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
#define SYMBOL_SYSTEMSETTINGSDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_SYSTEMSETTINGSDLG_TITLE _("System Settings")
#define SYMBOL_SYSTEMSETTINGSDLG_IDNAME ID_SYSTEMSETTINGSDLG
#define SYMBOL_SYSTEMSETTINGSDLG_SIZE wxSize(400, 300)
#define SYMBOL_SYSTEMSETTINGSDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SystemSettingsDlg class declaration
 */

class SystemSettingsDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SystemSettingsDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SystemSettingsDlg();
    SystemSettingsDlg( wxWindow* parent, 
        WHID::Options & mOptions,
        wxWindowID id = SYMBOL_SYSTEMSETTINGSDLG_IDNAME, const wxString& caption = SYMBOL_SYSTEMSETTINGSDLG_TITLE, const wxPoint& pos = SYMBOL_SYSTEMSETTINGSDLG_POSITION, const wxSize& size = SYMBOL_SYSTEMSETTINGSDLG_SIZE, long style = SYMBOL_SYSTEMSETTINGSDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SYSTEMSETTINGSDLG_IDNAME, const wxString& caption = SYMBOL_SYSTEMSETTINGSDLG_TITLE, const wxPoint& pos = SYMBOL_SYSTEMSETTINGSDLG_POSITION, const wxSize& size = SYMBOL_SYSTEMSETTINGSDLG_SIZE, long style = SYMBOL_SYSTEMSETTINGSDLG_STYLE );

    /// Destructor
    ~SystemSettingsDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SystemSettingsDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end SystemSettingsDlg event handler declarations

////@begin SystemSettingsDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SystemSettingsDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SystemSettingsDlg member variables
    /// Control identifiers
    enum {
        ID_SYSTEMSETTINGSDLG = 10130,
        ID_SYSTEM_SETTINGS_TAB = 10131,
        ID_SYSTEM_PANEL = 10132,
        ID_MULTIUSER = 10133
    };
////@end SystemSettingsDlg member variables

    WHID::Options mOptions;
};

#endif
    // _SYSTEMSETTINGSDLG_H_
