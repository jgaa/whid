/////////////////////////////////////////////////////////////////////////////
// Name:        RateManager.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 21:04:07
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _RATEMANAGER_H_
#define _RATEMANAGER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "RateManager.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_RATEMANAGER_STYLE 0
#define SYMBOL_RATEMANAGER_TITLE _("Rate Manager")
#define SYMBOL_RATEMANAGER_IDNAME ID_RATEMANAGER
#define SYMBOL_RATEMANAGER_SIZE wxSize(400, 300)
#define SYMBOL_RATEMANAGER_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * RateManager class declaration
 */

class RateManager: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( RateManager )
    DECLARE_EVENT_TABLE()

public:
    enum HeaderColumnsE
    {
        HC_NAME, HC_TYPE, HC_RATE
    };

    /// Constructors
    RateManager( );

    bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style );

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_RATEMANAGER_IDNAME, const wxString& caption = SYMBOL_RATEMANAGER_TITLE, const wxPoint& pos = SYMBOL_RATEMANAGER_POSITION, const wxSize& size = SYMBOL_RATEMANAGER_SIZE, long style = SYMBOL_RATEMANAGER_STYLE );
   
    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin RateManager event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_RATES
    void OnAddRatesClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_ADD_RATES
    void OnADDRATESUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REMOVE_RATE
    void OnRemoveRateClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_REMOVE_RATE
    void OnRemoveRateUpdate( wxUpdateUIEvent& event );

////@end RateManager event handler declarations

////@begin RateManager member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end RateManager member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin RateManager member variables
    wxListCtrl* mRateListCtl;
    wxButton* mDeleteButtonCtl;
    /// Control identifiers
    enum {
        ID_RATEMANAGER = 10072,
        ID_RATESLIST = 10073,
        ID_ADD_RATES = 10074,
        ID_REMOVE_RATE = 10075
    };
////@end RateManager member variables

    // Variables used to control the binding
    WHID::RateInfoList *mRates; // Points to the clients mRates
    WHID::WhidControlPrx mWhidControl;

    void ReloadRates();
    bool mAllowAdd;

protected:
    
    void OnRateListActivated(wxListEvent& event);

};

#endif
    // _RATEMANAGER_H_
