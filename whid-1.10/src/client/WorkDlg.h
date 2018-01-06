/////////////////////////////////////////////////////////////////////////////
// Name:        WorkDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 03:53:59
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
#include "wx/notebook.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"
////@end includes

#include "RateManager.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxDatePickerCtrl;
class WorkRateManager;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WORKDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WORKDLG_TITLE _("WorkDlg")
#define SYMBOL_WORKDLG_IDNAME ID_RATES
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
    
    /// Creation
    bool Create(WHID::WorkPrx prx, WHID::WhidControlPrx whidControl, wxWindow* parent, wxWindowID id = SYMBOL_WORKDLG_IDNAME, const wxString& caption = SYMBOL_WORKDLG_TITLE, const wxPoint& pos = SYMBOL_WORKDLG_POSITION, const wxSize& size = SYMBOL_WORKDLG_SIZE, long style = SYMBOL_WORKDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WorkDlg event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_WORK_PROPERTIES
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_DATE_CHANGED event handler for ID_FROM_DATE
    void OnFromDateDateChanged( wxDateEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_OK
    void OnOKUpdate( wxUpdateUIEvent& event );

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
    wxStaticBitmap* mTopLeftImageCtl;
    wxDatePickerCtrl* mFromDateCtl;
    wxTextCtrl* mFromTimeCtl;
    wxDatePickerCtrl* mToDateCtl;
    wxTextCtrl* mToTimeCtl;
    wxTextCtrl* mPausedCtl;
    wxRadioBox* mStatusCtl;
    wxTextCtrl* mNotesCtl;
    WorkRateManager* mRatesCtl;
    wxButton* mOkButtonCtl;
    /// Control identifiers
    enum {
        ID_RATES = 10076,
        ID_WORKTAB = 10041,
        ID_WORK_PROPERTIES = 10043,
        ID_FROM_DATE = 10044,
        ID_FROM_TIME = 10045,
        ID_TO_DATE = 10046,
        ID_TO_TIME = 10035,
        ID_PAUSED_TIME = 10047,
        ID_STATUS = 10048,
        ID_WORK_NOTES = 10049
    };
////@end WorkDlg member variables


    WHID::WorkData mData;
    WHID::WorkStatusE mStatus;

    WHID::WhidControlPrx mWhidControl;
    WHID::RateInfoList mRates;

    WHID::WorkPrx mWork;
    static wxBitmap *mImage;
    static wxDateTime mLastSetDate; // Save/set last set date
    bool mTrackLastSetDate;
};



#endif
    // _WORKDLG_H_
