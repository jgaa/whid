/////////////////////////////////////////////////////////////////////////////
// Name:        ChooseRateDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 18:16:20
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CHOOSERATEDLG_H_
#define _CHOOSERATEDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ChooseRateDlg.h"
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
#define SYMBOL_CHOOSERATEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CHOOSERATEDLG_TITLE _("Rates")
#define SYMBOL_CHOOSERATEDLG_IDNAME ID_CHOOSERATEDLG
#define SYMBOL_CHOOSERATEDLG_SIZE wxSize(400, 300)
#define SYMBOL_CHOOSERATEDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * ChooseRateDlg class declaration
 */

class ChooseRateDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ChooseRateDlg )
    DECLARE_EVENT_TABLE()

public:
    enum HeaderColumnsE
    {
        HC_NAME, HC_TYPE, HC_RATE
    };

    /// Constructors
    ChooseRateDlg( );
    ChooseRateDlg( wxWindow* parent, wxWindowID id = SYMBOL_CHOOSERATEDLG_IDNAME, const wxString& caption = SYMBOL_CHOOSERATEDLG_TITLE, const wxPoint& pos = SYMBOL_CHOOSERATEDLG_POSITION, const wxSize& size = SYMBOL_CHOOSERATEDLG_SIZE, long style = SYMBOL_CHOOSERATEDLG_STYLE );

    /// Creation
    bool Create(WHID::WhidControlPrx prx, wxWindow* parent, wxWindowID id = SYMBOL_CHOOSERATEDLG_IDNAME, const wxString& caption = SYMBOL_CHOOSERATEDLG_TITLE, const wxPoint& pos = SYMBOL_CHOOSERATEDLG_POSITION, const wxSize& size = SYMBOL_CHOOSERATEDLG_SIZE, long style = SYMBOL_CHOOSERATEDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ChooseRateDlg event handler declarations

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HIDE_INACTIVE
    void OnHideInactiveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_NEW
    void OnAddNewClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_ADD_NEW
    void OnADDNEWUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end ChooseRateDlg event handler declarations

////@begin ChooseRateDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ChooseRateDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ChooseRateDlg member variables
    wxListCtrl* mRateListCtl;
    wxCheckBox* mHideInactiveCtl;
    /// Control identifiers
    enum {
        ID_CHOOSERATEDLG = 10067,
        ID_RATELIST = 10068,
        ID_HIDE_INACTIVE = 10069,
        ID_ADD_NEW = 10070
    };
////@end ChooseRateDlg member variables

    // Set if the user selected a rate before oressing OK
    WHID::RateInfo mSelectedRate;

protected:
    void ReloadList();
    void OnRateListActivated(wxListEvent& event);

    WHID::WhidControlPrx mWhidControl;
    WHID::RateInfoList mRates;
    
};

#endif
    // _CHOOSERATEDLG_H_
