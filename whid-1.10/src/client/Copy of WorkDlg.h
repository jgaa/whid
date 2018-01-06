/////////////////////////////////////////////////////////////////////////////
// Name:        WorkDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 00:57:01
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WORKDLG_H_
#define _WORKDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "WorkDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/notebook.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxDatePickerCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WORKDLG 10036
#define SYMBOL_WORKDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WORKDLG_TITLE _("WorkDlg")
#define SYMBOL_WORKDLG_IDNAME ID_WORKDLG
#define SYMBOL_WORKDLG_SIZE wxSize(400, 300)
#define SYMBOL_WORKDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * WorkDlg class declaration
 */

class WorkDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WorkDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WorkDlg( );
    WorkDlg(WHID::WorkPrx prx, wxWindow* parent, wxWindowID id = SYMBOL_WORKDLG_IDNAME, const wxString& caption = SYMBOL_WORKDLG_TITLE, const wxPoint& pos = SYMBOL_WORKDLG_POSITION, const wxSize& size = SYMBOL_WORKDLG_SIZE, long style = SYMBOL_WORKDLG_STYLE );

    /// Creation
    bool Create(WHID::WorkPrx prx, wxWindow* parent, wxWindowID id = SYMBOL_WORKDLG_IDNAME, const wxString& caption = SYMBOL_WORKDLG_TITLE, const wxPoint& pos = SYMBOL_WORKDLG_POSITION, const wxSize& size = SYMBOL_WORKDLG_SIZE, long style = SYMBOL_WORKDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WorkDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end WorkDlg event handler declarations

////@begin WorkDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WorkDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WorkDlg member variables
    wxDatePickerCtrl* mFromDateCtl;
    wxTextCtrl* mFromTimeCtl;
    wxDatePickerCtrl* mToDateCtl;
    wxTextCtrl* mToTimeCtl;
    wxTextCtrl* mPausedCtl;
    wxRadioBox* mStatusCtl;
    wxTextCtrl* mNotesCtl;
////@end WorkDlg member variables

    WHID::WorkData mData;
    WHID::WorkStatusE mStatus;

    WHID::WorkPrx mWork;
};

#endif
    // _WORKDLG_H_
