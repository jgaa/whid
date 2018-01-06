/////////////////////////////////////////////////////////////////////////////
// Name:        FirstUseDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     15/12/2006 18:27:31
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FIRSTUSEDLG_H_
#define _FIRSTUSEDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "FirstUseDlg.h"
#endif

/*!
 * Includes
 */

#include <WarUtf8.h>

////@begin includes
#include "wx/valgen.h"
#include "wx/statline.h"
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
#define SYMBOL_FIRSTUSEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FIRSTUSEDLG_TITLE _("No database found")
#define SYMBOL_FIRSTUSEDLG_IDNAME ID_DIALOG
#define SYMBOL_FIRSTUSEDLG_SIZE wxSize(400, 300)
#define SYMBOL_FIRSTUSEDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * FirstUseDlg class declaration
 */

class FirstUseDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( FirstUseDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FirstUseDlg( );
    FirstUseDlg( wxWindow* parent, wxWindowID id = SYMBOL_FIRSTUSEDLG_IDNAME, const wxString& caption = SYMBOL_FIRSTUSEDLG_TITLE, const wxPoint& pos = SYMBOL_FIRSTUSEDLG_POSITION, const wxSize& size = SYMBOL_FIRSTUSEDLG_SIZE, long style = SYMBOL_FIRSTUSEDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FIRSTUSEDLG_IDNAME, const wxString& caption = SYMBOL_FIRSTUSEDLG_TITLE, const wxPoint& pos = SYMBOL_FIRSTUSEDLG_POSITION, const wxSize& size = SYMBOL_FIRSTUSEDLG_SIZE, long style = SYMBOL_FIRSTUSEDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin FirstUseDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end FirstUseDlg event handler declarations

////@begin FirstUseDlg member function declarations

    WarUtf8 GetLoginName() const { return mLoginName ; }
    void SetLoginName(WarUtf8 value) { mLoginName = value ; }

    WarUtf8 GetFullName() const { return mFullName ; }
    void SetFullName(WarUtf8 value) { mFullName = value ; }

    int GetCharge() const { return mCharge ; }
    void SetCharge(int value) { mCharge = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end FirstUseDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FirstUseDlg member variables
    wxTextCtrl* mFullNameCtl;
    wxTextCtrl* mChargeCtl;
private:
    WarUtf8 mLoginName;
    WarUtf8 mFullName;
    int mCharge;
    /// Control identifiers
    enum {
        ID_DIALOG = 10010,
        ID_LOGIN_NAME = 10136,
        ID_USER_FUL_NAME = 10023,
        ID_CHARGE_PER_HOUR = 10024
    };
////@end FirstUseDlg member variables
};

#endif
    // _FIRSTUSEDLG_H_
