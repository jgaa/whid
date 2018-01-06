/////////////////////////////////////////////////////////////////////////////
// Name:        NodeDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     16/12/2006 13:26:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NODEDLG_H_
#define _NODEDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "NodeDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/valgen.h"
////@end includes
#include <wx/statbox.h>
#include <wx/choice.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
class NodeRateManager;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_NODEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_NODEDLG_TITLE _("Node")
#define SYMBOL_NODEDLG_IDNAME ID_NODE_DLG
#define SYMBOL_NODEDLG_SIZE wxSize(400, 300)
#define SYMBOL_NODEDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * NodeDlg class declaration
 */

class NodeDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( NodeDlg )
    DECLARE_EVENT_TABLE()

public:
    enum HeaderColumnsE
    {
        HC_NAME, HC_TYPE, HC_RATE
    };

    enum TabPagesE
    {
        TP_PROP, TP_RATE, TP_CUSTDATA 
    };


    /// Constructors
    NodeDlg( );

    /// Creation
    bool Create(WHID::NodePrx node,
        WHID::WhidControlPrx whidControl,
        wxWindow* parent, wxWindowID id = SYMBOL_NODEDLG_IDNAME, const wxString& caption = SYMBOL_NODEDLG_TITLE, const wxPoint& pos = SYMBOL_NODEDLG_POSITION, const wxSize& size = SYMBOL_NODEDLG_SIZE, long style = SYMBOL_NODEDLG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin NodeDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_OK
    void OnOKUpdate( wxUpdateUIEvent& event );

////@end NodeDlg event handler declarations

////@begin NodeDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end NodeDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin NodeDlg member variables
    wxStaticBitmap* mTopLeftImageCtl;
    wxNotebook* mTabCtl;
    wxTextCtrl* mNameCtl;
    wxStaticBox* mActiveStateLabelCtl;
    wxChoice* mNodeActiveStatusCtl;
    wxTextCtrl* mNotesCtl;
    NodeRateManager* mRatesCtl;
    wxPanel* mCustomerDataCtl;
    /// Control identifiers
    enum {
        ID_NODE_DLG = 10025,
        ID_NOTEBOOK1 = 10031,
        ID_NODE_PROPERTIES = 10032,
        ID_NAME = 10026,
        ID_NODE_ACTIVE_STATUS = 10028,
        ID_NOTES = 10027,
        ID_CUSTOMER_DATA = 10160,
        ID_EMAIL = 10161
    };
////@end NodeDlg member variables

    WHID::NodeData mData;
    WHID::CustomerData mCustomerData;
    WHID::NodePrx mNode;
    WHID::WhidControlPrx mWhidControl;
    WHID::RateInfoList mRates;
    int mAutoStatus; 

protected:
    // Called to get the right stuff enabled and the correct image
    virtual void OnCustomizeToLevel() {} ;
    virtual void SetData();
    //virtual void SaveData();
};

#endif
    // _NODEDLG_H_
