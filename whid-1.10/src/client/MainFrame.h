/////////////////////////////////////////////////////////////////////////////
// Name:        MainFrame.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     15/12/2006 15:48:28
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "MainFrame.h"
#endif



/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/splitter.h"
#include "wx/tglbtn.h"
#include "wx/treectrl.h"
#include "wx/notebook.h"
#include "wx/html/htmlwin.h"
#include "wx/listctrl.h"
#include "wx/statusbr.h"
////@end includes

#include <wx/imaglist.h>
#include <wx/timer.h>
#include <wx/choice.h>
#include <wx/wxchar.h>



#include <WarPath.h>
#include <WarTime.h>
#include "server/UserI.h"
#include "InvoiceUtil.h"

/*!
 * Forward declarations
 */

class wxTreeItemId;

#include "app_resources.h"

////@begin forward declarations
class wxToggleButton;
class wxTreeCtrl;
class wxNotebook;
class wxHtmlWindow;
class wxListCtrl;
class wxStatusBar;
////@end forward declarations

class TreeNodeCustomer;
class TreeNodeProject;
class TreeNodeTask;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_MAINFRAME_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MAINFRAME_TITLE _("What have I Done!")
#define SYMBOL_MAINFRAME_IDNAME ID_MAINFRAME
#define SYMBOL_MAINFRAME_SIZE wxSize(800, 610)
#define SYMBOL_MAINFRAME_POSITION wxDefaultPosition
////@end control identifiers


//#define ID_WORK_LIST 10021
//#define ID_NOTEBOOK 10012
//#define ID_PROJECTS 10013

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

#define CTX MainFrame::GetCtx()

#define USER MainFrame::GetUser()

/*!
 * MainFrame class declaration
 */

// Tomer every 5 seconds
#define TIMER_INTERVAL 5
#define ID_MFTIMER ID_MAINFRAME

class MainFrame: public wxFrame, public InvoiceUtil
{    
    DECLARE_CLASS( MainFrame )
    DECLARE_EVENT_TABLE()

public:
    struct stacked_info
    {
        WarTime mPaused;
        WarTime mCreated;
        WHID::WorkPrx mWork;
    };
    typedef std::vector<stacked_info> stack_t;

    enum StatusBarFieldsE
    {
        SB_USER,
        SB_ADMIN,
        SB_INFO
    };

    enum TabPagesE
    {
        TAB_INFO, TAB_WORKLIST, TAB_INVOICES
    };

    enum WorkListColumnsE
    {
        WLC_ORIGIN,
        WLC_FROM,
        WLC_TO,
        WLC_PAUSED,
        WLC_ELAPSED,
        WLC_CHARGE,
        WLC_NOTES
    };

    enum InvoicesListColumnsE
    {
        ILC_DATE,
        ILC_ID,
        ILC_CUST,
        ILC_USER,
        ILC_HOURS,
        ILC_CHARGE,
        ILC_PROFIT,
    };

    enum StackListColumnsE
    {
        SLC_FROM,
        SLC_TO,
        SLC_PAUSED,
        SLC_ELAPSED,
        SLC_NOTES
    };

    /// Constructors
    MainFrame( );
    MainFrame( wxWindow* parent, wxWindowID id = SYMBOL_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_MAINFRAME_POSITION, const wxSize& size = SYMBOL_MAINFRAME_SIZE, long style = SYMBOL_MAINFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_MAINFRAME_POSITION, const wxSize& size = SYMBOL_MAINFRAME_SIZE, long style = SYMBOL_MAINFRAME_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MainFrame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_MAINFRAME
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_CONTEXT_MENU event handler for ID_MAINFRAME
    void OnContextMenu( wxContextMenuEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
    void OnEXITClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_PROPERTIES
    void OnPROPERTIESClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MANAGE_USERS
    void OnManageUsersClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MANAGE_USERS
    void OnManageUsersUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MANAGE_RATES
    void OnManageRatesClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MANAGE_RATES
    void OnManageRatesUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_GLOBAL_SETTINGS
    void OnGlobalSettingsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_BACKUP_DATABASE
    void OnBACKUPDATABASEClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_BACKUP_DATABASE
    void OnBACKUPDATABASEUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_RESTORE_DATABASE
    void OnRESTOREDATABASEClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_RESTORE_DATABASE
    void OnRESTOREDATABASEUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_CUSTOMER
    void OnEditCustomerClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EDIT_CUSTOMER
    void OnEditCustomerUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_NEW_CUSTOMER
    void OnNewCustomerUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_PROJECT
    void OnEditProjectClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EDIT_PROJECT
    void OnEditProjectUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_NEW_PROJECT
    void OnNewProjectUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_ID_NEW_TASK
    void OnIdNewTaskUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_TASK
    void OnEditTaskClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EDIT_TASK
    void OnEditTaskUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_NEW_TASK
    void OnNewTaskUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_NEW_WORK
    void OnNewWorkUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_WORK
    void OnEditWorkClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EDIT_WORK
    void OnEditWorkUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_DELETE_WORK
    void OnDeleteWorkClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_DELETE_WORK
    void OnDeleteWorkUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_INVOICES_TO_XML
    void OnEXPORTINVOICESTOXMLClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EXPORT_INVOICES_TO_XML
    void OnEXPORTINVOICESTOXMLUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_INVOICES_TO_HTML
    void OnEXPORTINVOICESTOHTMLClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EXPORT_INVOICES_TO_HTML
    void OnEXPORTINVOICESTOHTMLUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_TO_EMAIL
    void OnEXPORTTOEMAILClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_EXPORT_TO_EMAIL
    void OnEXPORTTOEMAILUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
    void OnABOUTClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_CUSTOMER
    void OnNewCustomerClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_PROJECT
    void OnNewProjectClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_TASK
    void OnNewTaskClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_WORK
    void OnNewWorkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_BILLING
    void OnBillingClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_BILLING
    void OnBillingUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_REFRESH
    void OnREFRESHClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_WORK_HOURS_STATUS_FILTER
    void OnWorkHoursStatusFilterSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_RESET_FILTERS
    void OnResetFiltersClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON1
    void OnTOGGLEBUTTON1Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_NOTE_FILTER_TEXT
    void OnNOTEFILTERTEXTUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_TREECTRL
    void OnProjectsSelChanged( wxTreeEvent& event );

    /// wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_TREECTRL
    void OnProjectsItemActivated( wxTreeEvent& event );

    /// wxEVT_COMMAND_TREE_ITEM_EXPANDING event handler for ID_TREECTRL
    void OnProjectsItemExpanding( wxTreeEvent& event );

    /// wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK
    void OnNotebookPageChanged( wxNotebookEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_WORK_LIST
    void OnWorkListSelected( wxListEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_WORK_LIST
    void OnWORKLISTDeselected( wxListEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_WORK_LIST
    void OnWorkListItemActivated( wxListEvent& event );

    /// wxEVT_LEFT_DCLICK event handler for ID_INVOICES_LIST
    void OnInvoicesListLeftDClick( wxMouseEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_STACK
    void OnStackItemActivated( wxListEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECK_IN_OUT
    void OnCheckInOutClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHECK_IN_OUT
    void OnCheckInOutUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PAUSE
    void OnPauseClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_PAUSE
    void OnPauseUpdate( wxUpdateUIEvent& event );

////@end MainFrame event handler declarations

////@begin MainFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MainFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MainFrame member variables
    wxChoice* mWorkHoursStatusFilterCtl;
    wxToggleButton* mOnlyActiveCtl;
    wxTextCtrl* mNodeFilterTextCtl;
    wxTreeCtrl* mTreeCtl;
    wxNotebook* mTabCtl;
    wxHtmlWindow* mSummaryCtl;
    wxListCtrl* mWorkListCtl;
    wxListCtrl* mInvoicesListCtl;
    wxListCtrl* mStackListCtl;
    wxToggleButton* mCheckInOutButtonCtl;
    wxToggleButton* mPauseButtonCtl;
    wxStatusBar* mStatusBarCtl;
    /// Control identifiers
    enum {
        ID_MAINFRAME = 10000,
        ID_MANAGE_USERS = 10141,
        ID_MANAGE_RATES = 10071,
        ID_GLOBAL_SETTINGS = 10134,
        ID_BACKUP_DATABASE = 10173,
        ID_RESTORE_DATABASE = 10174,
        ID_EDIT_CUSTOMER = 10033,
        ID_NEW_CUSTOMER = 10001,
        ID_EDIT_PROJECT = 10034,
        ID_NEW_PROJECT = 10011,
        ID_ID_NEW_TASK = 10040,
        ID_EDIT_TASK = 10042,
        ID_NEW_TASK = 10014,
        ID_NEW_WORK = 10050,
        ID_EDIT_WORK = 10124,
        ID_DELETE_WORK = 10125,
        ID_EXPORT_INVOICES_TO_XML = 10158,
        ID_EXPORT_INVOICES_TO_HTML = 10159,
        ID_EXPORT_TO_EMAIL = 10183,
        ID_TOOLBAR = 10002,
        ID_BILLING = 10106,
        ID_WORK_HOURS_STATUS_FILTER = 10077,
        ID_RESET_FILTERS = 10018,
        ID_SPLITTER_TOP_STACK = 10003,
        ID_SPLITTER_PROJECTS_STACK = 10004,
        ID_SPLITTER_PROJECTS_SUMMARY = 10005,
        ID_PANEL = 10010,
        ID_TOGGLEBUTTON1 = 10013,
        ID_NOTE_FILTER_TEXT = 10180,
        ID_TREECTRL = 10017,
        ID_NOTEBOOK = 10012,
        ID_HTMLWINDOW = 10016,
        ID_WORK_LIST = 10021,
        ID_INVOICES_LIST = 10157,
        ID_SPLITTER_STACK_CONTROL = 10006,
        ID_STACK = 10007,
        ID_PANEL1 = 10008,
        ID_CHECK_IN_OUT = 10022,
        ID_PAUSE = 10009,
        ID_STATUSBAR = 10015
    };
////@end MainFrame member variables

    void ReloadNodes();
    void ReloadWorklist(const bool statsOnly = false);
    void UpdateSummary();

    // Reeturn the selcetd customer or NULL
    TreeNodeCustomer *GetSelectedCustomer(wxTreeItemId *treeItem = NULL);
    TreeNodeProject *GetSelectedProject(wxTreeItemId *treeItem = NULL);
    TreeNodeTask *GetSelectedTask(wxTreeItemId *treeItem = NULL);

    // Returns -1 if nothing is selected, else the first selected
    // item index is returned.
    int GetSelectedWorkItem();

    template <class T> void AddChild(wxTreeItemId& parent, 
        WHID::NodePrx node,
        const bool haveChildren)
    {
        T *n = new T;
        n->mNode = node;

        WHID::NodeData data;
        node->GetData(data, CTX);

        WarUtf8 name = data.mName;
        wxTreeItemId tree_node =  mTreeCtl->AppendItem(parent, 
            name.GetUnicode().c_str(), 
            data.mActiveStatus, -1, n);
        if (tree_node.IsOk())
        {
            mTreeCtl->SetItemHasChildren(tree_node, haveChildren);
            mTreeCtl->EnsureVisible(tree_node);
        }
    }

    template <class T> void  AddChildren(wxTreeItemId& parent,
        WHID::NodeInfoList& children,
        const bool haveChildren,
        const bool onlyActive)
    {
        WHID::NodeInfoList::iterator child;

        mTreeCtl->DeleteChildren(parent);

        for(child = children.begin()
            ; child != children.end()
            ; child++)
        {
            T *n = new T;
            n->mNode = child->mNode;

            WarUtf8 name = child->mName;
            wxTreeItemId tree_node =  mTreeCtl->AppendItem(parent, 
                name.GetUnicode().c_str(), 
                (int)child->mType, -1, n);
            if (tree_node.IsOk())
            {
                if (!n->mNode->IsActive(CTX))
                {
                    mTreeCtl->SetItemTextColour(tree_node,
                        wxTheColourDatabase->Find(wxT("blue")));
                }
                mTreeCtl->SetItemHasChildren(tree_node, haveChildren);
            }
        }
    };

    template <class T> T *GetSelected(wxTreeItemId *treeItem = NULL)
    {
        if (IsCrippled())
            return NULL; 

        wxTreeItemId node = mTreeCtl->GetSelection();
        for(; node.IsOk() && mTreeCtl->GetItemParent(node).IsOk()
            ; node = mTreeCtl->GetItemParent(node))
        {
            T *data = dynamic_cast<T *>
                (mTreeCtl->GetItemData(node));

            if (NULL != data)
            {
                if (treeItem)
                    *treeItem = node;
                return data;
            }
        }

        return NULL;
    }

    bool HaveDatabase() const;

    static bool IsCrippled()
    {
        return mpThis->mIsCrippled;
    }

    void SetCrippled(const bool crippled)
    {
        mIsCrippled = crippled;
    }

    static bool IsMultiuser() 
    {
        return mpThis->mOptions.mMultiuser;
    }

    // Returns trrue if trhe current user is admin
    static bool IsAdmin()
    {
        return mpThis->mIsAdmin || !IsMultiuser() || IsCrippled();
    }

    void LoadOptions();

    bool Login();

    static Ice::Context& GetCtx()
    {
        WAR_ASSERT(NULL != mpCtx);
        return *mpCtx;
    }

    // Get the current user-handle for the user-session (CTX)
    // In single-user mode, return the user with ID 1
    static WHID::UserPrx GetUser()
    {
        if (mpThis->IsMultiuser())
        {
            WAR_ASSERT(mpThis->mUserPrx);
            return mpThis->mUserPrx;
        }
        
        return UserI::GetProxy(1);
    }

private:
    void RefreshStack();
    void UnselectStack();
    int GetSelectedStackItem();
    void OnTimer(wxTimerEvent& event);
    void StackResetTimer(stacked_info& info);
    void EditWork(const int itemIndex);
    void ReloadInvoices();
    void ReloadCurrentList();
    void ExportSelectedInvoicesToXml();
    void ExportSelectedInvoicesToHtml();
    void GetSelectedInvoices(WHID::InvoiceList& invoices);
    void UpdateWorkListItem(WHID::WorkPrx work);
    int SetDataInWorklist(int nitem, int row, const WHID::WorkInfo wi,
        const bool isStats = false);
   
    wxTimer mTimer;

    WHID::WorkInfoList mWorkListItems;
    WHID::InvoiceInfoList mInvoicesListItems;
    stack_t mWorkStack;
    bool mIsPaused;
    bool mIsCrippled;
    bool mIsAdmin;
    WHID::Options mOptions;
    static Ice::Context *mpCtx;
    WHID::Session mSession;
    WHID::UserPrx mUserPrx;
    static MainFrame *mpThis;
};


#define IS_ADMIN MainFrame::IsAdmin()

#endif
    // _MAINFRAME_H_
