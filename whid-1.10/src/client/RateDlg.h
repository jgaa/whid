/////////////////////////////////////////////////////////////////////////////
// Name:        RateDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 16:39:42
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _RATEDLG_H_
#define _RATEDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "RateDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_RATEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_RATEDLG_TITLE _("Rate")
#define SYMBOL_RATEDLG_IDNAME ID_RATEDLG
#define SYMBOL_RATEDLG_SIZE wxSize(400, 300)
#define SYMBOL_RATEDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * RateDlg class declaration
 */

class RateDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( RateDlg )
    DECLARE_EVENT_TABLE()

public:
    enum DaysInWeekItemsE
    {
        DIW_ALL,
        DIW_MOM,
        DIW_TUE,
        DIW_WED,
        DIW_THUE,
        DIW_FRI,
        DIW_SAT,
        DIW_SUN,
        DIW_PUBH,
        DIW_SPUBH
    };

    /// Constructors
    RateDlg( );

    /// Creation
    bool Create(WHID::RatePrx prx,
        WHID::WhidControlPrx ctl,
        wxWindow* parent, wxWindowID id = SYMBOL_RATEDLG_IDNAME, const wxString& caption = SYMBOL_RATEDLG_TITLE, const wxPoint& pos = SYMBOL_RATEDLG_POSITION, const wxSize& size = SYMBOL_RATEDLG_SIZE, long style = SYMBOL_RATEDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin RateDlg event handler declarations

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FROM_TO_HRS_ENABLED
    void OnFromToHrsEnabledClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_OK
    void OnOKUpdate( wxUpdateUIEvent& event );

////@end RateDlg event handler declarations

////@begin RateDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end RateDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin RateDlg member variables
    wxTextCtrl* mNameCtl;
    wxSpinCtrl* mPriorityCtl;
    wxChoice* mTypeCtl;
    wxTextCtrl* mChargeCtl;
    wxTextCtrl* mCostCtl;
    wxCheckBox* mEnabledCtl;
    wxCheckBox* mFromToHoursEnabledCtl;
    wxTextCtrl* mFromClockCtl;
    wxTextCtrl* mToClockCtl;
    wxCheckBox* mEnableContinousTimeCtl;
    wxSpinCtrl* mNumContinousHoursCtl;
    wxSpinCtrl* mMaxTimeBetweenContinousIntervalsCtl;
    wxCheckListBox* mDaysOfWeekCtl;
    wxSpinCtrl* mMinTimeCtl;
    wxSpinCtrl* mAlignCtl;
    wxSpinCtrl* mAlignThresholdCtl;
    wxSpinCtrl* mAlignStartCtl;
    wxTextCtrl* mNotesCtl;
    /// Control identifiers
    enum {
        ID_RATEDLG = 10051,
        ID_RATETAB = 10052,
        ID_PROPERTIES_PANEL = 10053,
        ID_NAMECTL = 10054,
        ID_PRIORIRY_CTL = 10055,
        ID_TYPE_CTL = 10056,
        ID_CHARGE_CTL = 10062,
        ID_RATE_COST = 10019,
        ID_ENABLED_CTL = 10063,
        ID_APPLIES_PANEL = 10057,
        ID_FROM_TO_HRS_ENABLED = 10060,
        ID_FROM_HOUR = 10059,
        ID_TO_HOUR = 10175,
        ID_ENABLE_CONTINOUS_TIME = 10179,
        ID_SPINCTRL_FROM_HOURS = 10061,
        ID_SPINCTRL_MAX_DELAYS = 10177,
        ID_DAYS_OF_WEEK_CTL = 10058,
        ID_ADJUSTMENTS_PANEL = 10064,
        ID_SPINCTRL_MIN_TIME = 10176,
        ID_SPINCTRL_ALIGN_NEAREST = 10066,
        ID_SPINCTRL_TREASHOLD = 10178,
        ID_SPINCTRL_OFFSET = 10129,
        ID_NOTES = 10065
    };
////@end RateDlg member variables

protected:
    void LoadData();
    void SaveData();

    

    WHID::RatePrx mPrx;
    WHID::WhidControlPrx mWhidControl;
    WHID::RateData mData;
};

#endif
    // _RATEDLG_H_
