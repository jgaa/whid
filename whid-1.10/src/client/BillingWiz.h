/////////////////////////////////////////////////////////////////////////////
// Name:        BillingWiz.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     01/01/2007 14:44:01
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _BILLINGWIZ_H_
#define _BILLINGWIZ_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "BillingWiz.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/wizard.h"
#include "wx/calctrl.h"
#include "wx/treectrl.h"
#include "wx/listctrl.h"
////@end includes

#include "InvoiceUtil.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class BillWizTimeframePage;
class wxCalendarCtrl;
class BillWizProjectsPage;
class BillWizChargesPage;
class wxTreeCtrl;
class wxListCtrl;
class BillWizInvoicingPage;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_BILLINGWIZ_IDNAME ID_BILINGWIZ
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif


/*!
 * BillingWiz class declaration
 */

class BillingWiz: public wxWizard
{    
    DECLARE_DYNAMIC_CLASS( BillingWiz )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    BillingWiz( );
    BillingWiz( wxWindow* parent, wxWindowID id = SYMBOL_BILLINGWIZ_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_BILLINGWIZ_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin BillingWiz event handler declarations

////@end BillingWiz event handler declarations

////@begin BillingWiz member function declarations

    /// Runs the wizard
    bool Run();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BillingWiz member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin BillingWiz member variables
    /// Control identifiers
    enum {
        ID_BILINGWIZ = 10107
    };
////@end BillingWiz member variables

    wxDateTime mFromdate;
    wxDateTime mToDate;
};

/*!
 * BillWizProjectsPage class declaration
 */

class BillWizProjectsPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( BillWizProjectsPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    BillWizProjectsPage( );

    BillWizProjectsPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin BillWizProjectsPage event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_SELECT_PROJECTS
    void OnSelectProjectsPageChanging( wxWizardEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_ALL
    void OnSelectAllClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_UNSELECT_ALL
    void OnUnselectAllClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TOGGLE
    void OnToggleClick( wxCommandEvent& event );

////@end BillWizProjectsPage event handler declarations

////@begin BillWizProjectsPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BillWizProjectsPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin BillWizProjectsPage member variables
    wxCheckListBox* mProjectsToBillCtl;
    /// Control identifiers
    enum {
        ID_SELECT_PROJECTS = 10108,
        ID_PROJECTS_TO_BILL = 10109,
        ID_SELECT_ALL = 10110,
        ID_UNSELECT_ALL = 10111,
        ID_TOGGLE = 10112
    };
////@end BillWizProjectsPage member variables

    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();

    // The order in mProjectsToBillCtl corresponds to the order
    // in mProjects.
    WHID::ProjectForBillingInfoList mProjects;
};

class CustTreeItemData : public wxTreeItemData
{
public:

    CustTreeItemData(WHID::CustomerPrx cust)
        : mCustomer(cust)
    {
    }

    WHID::CustomerPrx mCustomer;
};


class ProjTreeItemData : public wxTreeItemData
{
public:

    ProjTreeItemData(WHID::ProjectPrx proj)
        : mProject(proj)
    {
    }

    WHID::ProjectPrx mProject;
};

/*!
 * BillWizChargesPage class declaration
 */

class BillWizChargesPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( BillWizChargesPage )
    DECLARE_EVENT_TABLE()

public:
    enum ListColsE
    {
        LC_TASK,
        LC_FROM,
        LC_TO,
        LC_HOURS,
        LC_RATE,
        LC_RATE_PR_HOUR,
        LC_CHARGE,
        LC_DISCOUNT
    };

    typedef std::map<war_int64_t, wxTreeItemId> node_map_t;

    /// Constructors
    BillWizChargesPage( );

    BillWizChargesPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin BillWizChargesPage event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_DO_CALCULATE
    void OnDoCalculatePageChanged( wxWizardEvent& event );

    /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_CHARGES_TREE
    void OnChargesTreeSelChanged( wxTreeEvent& event );

////@end BillWizChargesPage event handler declarations

////@begin BillWizChargesPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BillWizChargesPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin BillWizChargesPage member variables
    wxTreeCtrl* mChargesTreeCtl;
    wxListCtrl* mChargesListCtl;
    /// Control identifiers
    enum {
        ID_DO_CALCULATE = 10113,
        ID_CHARGES_TREE = 10114,
        ID_CHARGES_LIST = 10115
    };
////@end BillWizChargesPage member variables

    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();
};

/*!
 * BillWizInvoicingPage class declaration
 */

class BillWizInvoicingPage: public wxWizardPageSimple,
    public InvoiceUtil
{    
    DECLARE_DYNAMIC_CLASS( BillWizInvoicingPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    BillWizInvoicingPage( );

    BillWizInvoicingPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin BillWizInvoicingPage event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_INVOICING
    void OnInvoicingPageChanged( wxWizardEvent& event );

    /// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_INVOICING
    void OnInvoicingPageChanging( wxWizardEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_ALl_INVOICES
    void OnSELECTALlINVOICESClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_UNSELECT_ALL_INVOICES
    void OnUnselectAllInvoicesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TOGGLE_ALL_INVOICES
    void OnToggleAllInvoicesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPORT_TO_HTML
    void OnExportToHtmlClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPORT_TO_XML
    void OnExportToXmlClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SEND_ON_EMAIL
    void OnSendOnEmailClick( wxCommandEvent& event );

////@end BillWizInvoicingPage event handler declarations

////@begin BillWizInvoicingPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BillWizInvoicingPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin BillWizInvoicingPage member variables
    wxCheckListBox* mInvoicesListCtl;
    /// Control identifiers
    enum {
        ID_INVOICING = 10116,
        ID_INVOICES = 10117,
        ID_SELECT_ALl_INVOICES = 10121,
        ID_UNSELECT_ALL_INVOICES = 10122,
        ID_TOGGLE_ALL_INVOICES = 10123,
        ID_EXPORT_TO_HTML = 10118,
        ID_EXPORT_TO_XML = 10119,
        ID_SEND_ON_EMAIL = 10120
    };
////@end BillWizInvoicingPage member variables

    virtual bool TransferDataToWindow();

    // Corresponds with mInvoicesListCtl
    WHID::InvoiceInfoList mInvoices;

protected:
    void GetSelectedInvoices(WHID::InvoiceList& invoices);
};

/*!
 * BillWizTimeframePage class declaration
 */

class BillWizTimeframePage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( BillWizTimeframePage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    BillWizTimeframePage( );

    BillWizTimeframePage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin BillWizTimeframePage event handler declarations

////@end BillWizTimeframePage event handler declarations

////@begin BillWizTimeframePage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BillWizTimeframePage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin BillWizTimeframePage member variables
    wxCalendarCtrl* mFromDateCtl;
    wxCalendarCtrl* mToDateCtl;
    /// Control identifiers
    enum {
        ID_TIMEFRAME = 10126,
        ID_FROM_DATE_CALENDAR = 10127,
        ID_TO_DATE_CALENDAR = 10128
    };
////@end BillWizTimeframePage member variables

    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();
};

#endif
    // _BILLINGWIZ_H_
