/////////////////////////////////////////////////////////////////////////////
// Name:        MainFrame.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     15/12/2006 15:48:28
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MainFrame.h"
#endif

#include "libwhid.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/gdicmn.h>

#ifdef WIN32
#   include <shellapi.h>
#endif

#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>


////@begin includes
////@end includes

#include "WhidApp.h"
#include "MainFrame.h"
#include "CustomerDlg.h"
#include "TreeNodeCustomer.h"
#include "ProjectDlg.h"
#include "TreeNodeProject.h"
#include "TreeNodeTask.h"
#include "TaskDlg.h"
#include "WorkDlg.h"
#include "ChooseRateDlg.h"
#include "PropertiesDlg.h"
#include "AboutDlg.h"
#include "BillingWiz.h"
#include "SystemSettingsDlg.h"
#include "UserManager.h"
#include "LogonDlg.h"

#include <WarLogEngine.h>
#include <WarLog.h>
#include <wxwidgets/WarWxLogHandler.h>
#include <WarFileWrapper.h>
#include <WarTmpFileName.h>

#include <wx/busyinfo.h>

////@begin XPM images
////@end XPM images

#include "icons/16x16/work0.xpm"
#include "icons/16x16/work1.xpm"
#include "icons/16x16/work2.xpm"
#include "icons/16x16/work3.xpm"
#include "icons/16x16/work4.xpm"
#include "icons/16x16/work5.xpm"
#include "icons/16x16/work6.xpm"

#include "icons/16x16/customer.xpm"
#include "icons/16x16/project.xpm"
#include "icons/16x16/task.xpm"

/*!
 * MainFrame type definition
 */

IMPLEMENT_CLASS( MainFrame, wxFrame )

/*!
 * MainFrame event table definition
 */

BEGIN_EVENT_TABLE( MainFrame, wxFrame )

////@begin MainFrame event table entries
    EVT_CLOSE( MainFrame::OnCloseWindow )
    EVT_CONTEXT_MENU( MainFrame::OnContextMenu )

    EVT_MENU( wxID_EXIT, MainFrame::OnEXITClick )

    EVT_MENU( wxID_PROPERTIES, MainFrame::OnPROPERTIESClick )

    EVT_MENU( ID_MANAGE_USERS, MainFrame::OnManageUsersClick )
    EVT_UPDATE_UI( ID_MANAGE_USERS, MainFrame::OnManageUsersUpdate )

    EVT_MENU( ID_MANAGE_RATES, MainFrame::OnManageRatesClick )
    EVT_UPDATE_UI( ID_MANAGE_RATES, MainFrame::OnManageRatesUpdate )

    EVT_MENU( ID_GLOBAL_SETTINGS, MainFrame::OnGlobalSettingsClick )

    EVT_MENU( ID_BACKUP_DATABASE, MainFrame::OnBACKUPDATABASEClick )
    EVT_UPDATE_UI( ID_BACKUP_DATABASE, MainFrame::OnBACKUPDATABASEUpdate )

    EVT_MENU( ID_RESTORE_DATABASE, MainFrame::OnRESTOREDATABASEClick )
    EVT_UPDATE_UI( ID_RESTORE_DATABASE, MainFrame::OnRESTOREDATABASEUpdate )

    EVT_MENU( ID_EDIT_CUSTOMER, MainFrame::OnEditCustomerClick )
    EVT_UPDATE_UI( ID_EDIT_CUSTOMER, MainFrame::OnEditCustomerUpdate )

    EVT_UPDATE_UI( ID_NEW_CUSTOMER, MainFrame::OnNewCustomerUpdate )

    EVT_MENU( ID_EDIT_PROJECT, MainFrame::OnEditProjectClick )
    EVT_UPDATE_UI( ID_EDIT_PROJECT, MainFrame::OnEditProjectUpdate )

    EVT_UPDATE_UI( ID_NEW_PROJECT, MainFrame::OnNewProjectUpdate )

    EVT_UPDATE_UI( ID_ID_NEW_TASK, MainFrame::OnIdNewTaskUpdate )

    EVT_MENU( ID_EDIT_TASK, MainFrame::OnEditTaskClick )
    EVT_UPDATE_UI( ID_EDIT_TASK, MainFrame::OnEditTaskUpdate )

    EVT_UPDATE_UI( ID_NEW_TASK, MainFrame::OnNewTaskUpdate )

    EVT_UPDATE_UI( ID_NEW_WORK, MainFrame::OnNewWorkUpdate )

    EVT_MENU( ID_EDIT_WORK, MainFrame::OnEditWorkClick )
    EVT_UPDATE_UI( ID_EDIT_WORK, MainFrame::OnEditWorkUpdate )

    EVT_MENU( ID_DELETE_WORK, MainFrame::OnDeleteWorkClick )
    EVT_UPDATE_UI( ID_DELETE_WORK, MainFrame::OnDeleteWorkUpdate )

    EVT_MENU( ID_EXPORT_INVOICES_TO_XML, MainFrame::OnEXPORTINVOICESTOXMLClick )
    EVT_UPDATE_UI( ID_EXPORT_INVOICES_TO_XML, MainFrame::OnEXPORTINVOICESTOXMLUpdate )

    EVT_MENU( ID_EXPORT_INVOICES_TO_HTML, MainFrame::OnEXPORTINVOICESTOHTMLClick )
    EVT_UPDATE_UI( ID_EXPORT_INVOICES_TO_HTML, MainFrame::OnEXPORTINVOICESTOHTMLUpdate )

    EVT_MENU( ID_EXPORT_TO_EMAIL, MainFrame::OnEXPORTTOEMAILClick )
    EVT_UPDATE_UI( ID_EXPORT_TO_EMAIL, MainFrame::OnEXPORTTOEMAILUpdate )

    EVT_MENU( wxID_ABOUT, MainFrame::OnABOUTClick )

    EVT_MENU( ID_NEW_CUSTOMER, MainFrame::OnNewCustomerClick )

    EVT_MENU( ID_NEW_PROJECT, MainFrame::OnNewProjectClick )
    EVT_UPDATE_UI( ID_NEW_PROJECT, MainFrame::OnNewProjectUpdate )

    EVT_MENU( ID_NEW_TASK, MainFrame::OnNewTaskClick )
    EVT_UPDATE_UI( ID_NEW_TASK, MainFrame::OnNewTaskUpdate )

    EVT_MENU( ID_NEW_WORK, MainFrame::OnNewWorkClick )
    EVT_UPDATE_UI( ID_NEW_WORK, MainFrame::OnNewWorkUpdate )

    EVT_MENU( ID_BILLING, MainFrame::OnBillingClick )
    EVT_UPDATE_UI( ID_BILLING, MainFrame::OnBillingUpdate )

    EVT_MENU( wxID_REFRESH, MainFrame::OnREFRESHClick )

    EVT_CHOICE( ID_WORK_HOURS_STATUS_FILTER, MainFrame::OnWorkHoursStatusFilterSelected )

    EVT_MENU( ID_RESET_FILTERS, MainFrame::OnResetFiltersClick )

    EVT_TOGGLEBUTTON( ID_TOGGLEBUTTON1, MainFrame::OnTOGGLEBUTTON1Click )

    EVT_TEXT( ID_NOTE_FILTER_TEXT, MainFrame::OnNOTEFILTERTEXTUpdated )

    EVT_TREE_SEL_CHANGED( ID_TREECTRL, MainFrame::OnProjectsSelChanged )
    EVT_TREE_ITEM_ACTIVATED( ID_TREECTRL, MainFrame::OnProjectsItemActivated )
    EVT_TREE_ITEM_EXPANDING( ID_TREECTRL, MainFrame::OnProjectsItemExpanding )

    EVT_NOTEBOOK_PAGE_CHANGED( ID_NOTEBOOK, MainFrame::OnNotebookPageChanged )

    EVT_LIST_ITEM_SELECTED( ID_WORK_LIST, MainFrame::OnWorkListSelected )
    EVT_LIST_ITEM_DESELECTED( ID_WORK_LIST, MainFrame::OnWORKLISTDeselected )
    EVT_LIST_ITEM_ACTIVATED( ID_WORK_LIST, MainFrame::OnWorkListItemActivated )

    EVT_LIST_ITEM_ACTIVATED( ID_STACK, MainFrame::OnStackItemActivated )

    EVT_TOGGLEBUTTON( ID_CHECK_IN_OUT, MainFrame::OnCheckInOutClick )
    EVT_UPDATE_UI( ID_CHECK_IN_OUT, MainFrame::OnCheckInOutUpdate )

    EVT_TOGGLEBUTTON( ID_PAUSE, MainFrame::OnPauseClick )
    EVT_UPDATE_UI( ID_PAUSE, MainFrame::OnPauseUpdate )

////@end MainFrame event table entries

  EVT_TIMER(ID_MFTIMER, MainFrame::OnTimer) 

END_EVENT_TABLE()


Ice::Context *MainFrame::mpCtx = NULL;

MainFrame *MainFrame::mpThis = NULL;

/*!
 * MainFrame constructors
 */

MainFrame::MainFrame( )
{
    WAR_ASSERT(NULL == mpThis);
    Init();

    // Default options in case we can't access the database
    mOptions.mMultiuser = false;
}

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * MainFrame creator
 */

bool MainFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    const wchar_t *my_caption = caption.GetData();
    ++my_caption;
    --my_caption;
    WarCollector<wxChar> test_str = my_caption;
    WAR_DEBUGLOG;
    debug_log << "my_caption is " << my_caption << war_endl;
////@begin MainFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();

    if (FindWindow(ID_SPLITTER_TOP_STACK))
        ((wxSplitterWindow*) FindWindow(ID_SPLITTER_TOP_STACK))->SetSashPosition(390);
    if (FindWindow(ID_SPLITTER_PROJECTS_STACK))
        ((wxSplitterWindow*) FindWindow(ID_SPLITTER_PROJECTS_STACK))->SetSashPosition(200);
    if (FindWindow(ID_SPLITTER_PROJECTS_SUMMARY))
        ((wxSplitterWindow*) FindWindow(ID_SPLITTER_PROJECTS_SUMMARY))->SetSashPosition(300);
    if (FindWindow(ID_SPLITTER_STACK_CONTROL))
        ((wxSplitterWindow*) FindWindow(ID_SPLITTER_STACK_CONTROL))->SetSashPosition(600);
////@end MainFrame creation


    WarWxLogHandler *log_wnd = new WarWxLogHandler("popuplog", "ERROR");
    WarLogEngine::GetEngine().AddHandler(log_wnd);

    WarLog info(WARLOG_INFO, "MainFrame::Create()");
    info << PROGRAM << " version " << WHID_VERSION << " starting up." << war_endl;
    info << COPYRIGHT << war_endl;

    mWorkHoursStatusFilterCtl->SetSelection(WHID::WFOPEN);

    // Initialize the server-tree
    {
        wxBitmap icon;
        wxImageList *image_list = new wxImageList(16,16);
        image_list->Add(wxIcon(customer_xpm));
        image_list->Add(wxIcon(project_xpm));
        image_list->Add(wxIcon(task_xpm));
       
        mTreeCtl->SetImageList(new wxImageList(*image_list));
    }

    // Add icons (same order as enums in IconsE
    {
        wxBitmap icon;
        wxImageList *image_list = new wxImageList(16,16);
        image_list->Add(wxIcon(work0_xpm));
        image_list->Add(wxIcon(work1_xpm));
        image_list->Add(wxIcon(work2_xpm));
        image_list->Add(wxIcon(work3_xpm));
        image_list->Add(wxIcon(work4_xpm));
        image_list->Add(wxIcon(work5_xpm));
        image_list->Add(wxIcon(work6_xpm));
       
        mWorkListCtl->SetImageList(new wxImageList(*image_list), wxIMAGE_LIST_SMALL);
        mStackListCtl->SetImageList(image_list, wxIMAGE_LIST_SMALL);
    }

    

    //ReloadNodes();
    mTimer.SetOwner(this, ID_MFTIMER);
    mTimer.Start(TIMER_INTERVAL * 1000);
    //UpdateSummary();

    return true;
}

/*!
 * Member initialisation 
 */

void MainFrame::Init()
{
    WAR_ASSERT(NULL == mpThis);
    mpThis = this;

    WAR_ASSERT(NULL == mpCtx);
    mpCtx = new Ice::Context();

////@begin MainFrame member initialisation
    mWorkHoursStatusFilterCtl = NULL;
    mOnlyActiveCtl = NULL;
    mNodeFilterTextCtl = NULL;
    mTreeCtl = NULL;
    mTabCtl = NULL;
    mSummaryCtl = NULL;
    mWorkListCtl = NULL;
    mInvoicesListCtl = NULL;
    mStackListCtl = NULL;
    mCheckInOutButtonCtl = NULL;
    mPauseButtonCtl = NULL;
    mStatusBarCtl = NULL;
////@end MainFrame member initialisation

    mIsPaused = false;
    mIsCrippled = true;
    mIsAdmin = false;
}
/*!
 * Control creation for MainFrame
 */

void MainFrame::CreateControls()
{    
////@begin MainFrame content construction
    MainFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_EXIT, _("&Quit"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_PROPERTIES, _("&Properties"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_MANAGE_USERS, _("&Users"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_MANAGE_RATES, _("&Rates Manager"), _T(""), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_GLOBAL_SETTINGS, _("Global &System settings"), _T(""), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_BACKUP_DATABASE, _("Backup data"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_RESTORE_DATABASE, _("Restore data"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu13 = new wxMenu;
    itemMenu13->Append(ID_EDIT_CUSTOMER, _("&Edit"), _T(""), wxITEM_NORMAL);
    itemMenu13->Append(ID_NEW_CUSTOMER, _("Add a new &Customer"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu13, _("&Customer"));
    wxMenu* itemMenu16 = new wxMenu;
    itemMenu16->Append(ID_EDIT_PROJECT, _("&Edit"), _T(""), wxITEM_NORMAL);
    itemMenu16->Append(ID_NEW_PROJECT, _("&Add a new Project"), _T(""), wxITEM_NORMAL);
    itemMenu16->Append(ID_ID_NEW_TASK, _("Add a new &Task"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu16, _("&Project"));
    wxMenu* itemMenu20 = new wxMenu;
    itemMenu20->Append(ID_EDIT_TASK, _("&Edit"), _T(""), wxITEM_NORMAL);
    itemMenu20->Append(ID_NEW_TASK, _("&Add a new Task"), _T(""), wxITEM_NORMAL);
    itemMenu20->Append(ID_NEW_WORK, _("Add &Work"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu20, _("&Task"));
    wxMenu* itemMenu24 = new wxMenu;
    itemMenu24->Append(ID_EDIT_WORK, _("&Edit\t..."), _T(""), wxITEM_NORMAL);
    itemMenu24->Append(ID_DELETE_WORK, _("Item"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu24, _("&Work"));
    wxMenu* itemMenu27 = new wxMenu;
    itemMenu27->Append(ID_EXPORT_INVOICES_TO_XML, _("Export to XML"), _T(""), wxITEM_NORMAL);
    itemMenu27->Append(ID_EXPORT_INVOICES_TO_HTML, _("Export to HTML"), _T(""), wxITEM_NORMAL);
    itemMenu27->Append(ID_EXPORT_TO_EMAIL, _("Export to email"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu27, _("Invoices"));
    wxMenu* itemMenu31 = new wxMenu;
    itemMenu31->Append(wxID_ABOUT, _("&About"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu31, _("Help"));
    itemFrame1->SetMenuBar(menuBar);

    wxToolBar* itemToolBar33 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    itemToolBar33->SetToolBitmapSize(wxSize(32, 32));
    wxBitmap itemtool34Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/add_user.png")));
    wxBitmap itemtool34BitmapDisabled;
    itemToolBar33->AddTool(ID_NEW_CUSTOMER, _T(""), itemtool34Bitmap, itemtool34BitmapDisabled, wxITEM_NORMAL, _("Add a new customer"), wxEmptyString);
    wxBitmap itemtool35Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/add_project.png")));
    wxBitmap itemtool35BitmapDisabled;
    itemToolBar33->AddTool(ID_NEW_PROJECT, _T(""), itemtool35Bitmap, itemtool35BitmapDisabled, wxITEM_NORMAL, _("Add a new project"), wxEmptyString);
    wxBitmap itemtool36Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/new_task.png")));
    wxBitmap itemtool36BitmapDisabled;
    itemToolBar33->AddTool(ID_NEW_TASK, _T(""), itemtool36Bitmap, itemtool36BitmapDisabled, wxITEM_NORMAL, _("Add a new task in a project"), wxEmptyString);
    wxBitmap itemtool37Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/add_work.png")));
    wxBitmap itemtool37BitmapDisabled;
    itemToolBar33->AddTool(ID_NEW_WORK, _T(""), itemtool37Bitmap, itemtool37BitmapDisabled, wxITEM_NORMAL, _("Add work hours"), wxEmptyString);
    wxBitmap itemtool38Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/billing_small.png")));
    wxBitmap itemtool38BitmapDisabled;
    itemToolBar33->AddTool(ID_BILLING, _T(""), itemtool38Bitmap, itemtool38BitmapDisabled, wxITEM_NORMAL, _("Billing - Calculate charges and make incoices"), wxEmptyString);
    itemToolBar33->AddSeparator();
    wxBitmap itemtool40Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/reload.png")));
    wxBitmap itemtool40BitmapDisabled;
    itemToolBar33->AddTool(wxID_REFRESH, _T(""), itemtool40Bitmap, itemtool40BitmapDisabled, wxITEM_NORMAL, _("Refresh the information on the screen"), wxEmptyString);
    itemToolBar33->AddSeparator();
    wxArrayString mWorkHoursStatusFilterCtlStrings;
    mWorkHoursStatusFilterCtlStrings.Add(_("Currently Active"));
    mWorkHoursStatusFilterCtlStrings.Add(_("Done, waiting to be invoiced"));
    mWorkHoursStatusFilterCtlStrings.Add(_("Done, hold back"));
    mWorkHoursStatusFilterCtlStrings.Add(_("Free"));
    mWorkHoursStatusFilterCtlStrings.Add(_("Invoived"));
    mWorkHoursStatusFilterCtlStrings.Add(_("Not invoiced or free (pending for invoice)"));
    mWorkHoursStatusFilterCtlStrings.Add(_("All"));
    mWorkHoursStatusFilterCtl = new wxChoice( itemToolBar33, ID_WORK_HOURS_STATUS_FILTER, wxDefaultPosition, wxDefaultSize, mWorkHoursStatusFilterCtlStrings, 0 );
    mWorkHoursStatusFilterCtl->SetStringSelection(_("5"));
    mWorkHoursStatusFilterCtl->SetHelpText(_("Filter the work-hour selection"));
    if (MainFrame::ShowToolTips())
        mWorkHoursStatusFilterCtl->SetToolTip(_("Work-hours status to show"));
    itemToolBar33->AddControl(mWorkHoursStatusFilterCtl);
    wxBitmap itemtool43Bitmap(itemFrame1->GetBitmapResource(wxT("icons/32x32/reset_filter.png")));
    wxBitmap itemtool43BitmapDisabled;
    itemToolBar33->AddTool(ID_RESET_FILTERS, _T(""), itemtool43Bitmap, itemtool43BitmapDisabled, wxITEM_NORMAL, _("Reset the filters"), wxEmptyString);
    itemToolBar33->AddSeparator();
    itemToolBar33->Realize();
    itemFrame1->SetToolBar(itemToolBar33);

    wxSplitterWindow* itemSplitterWindow45 = new wxSplitterWindow( itemFrame1, ID_SPLITTER_TOP_STACK, wxDefaultPosition, wxSize(100, 100), wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow45->SetMinimumPaneSize(0);

    wxSplitterWindow* itemSplitterWindow46 = new wxSplitterWindow( itemSplitterWindow45, ID_SPLITTER_PROJECTS_STACK, wxDefaultPosition, wxSize(100, 100), wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow46->SetMinimumPaneSize(0);

    wxSplitterWindow* itemSplitterWindow47 = new wxSplitterWindow( itemSplitterWindow46, ID_SPLITTER_PROJECTS_SUMMARY, wxDefaultPosition, wxSize(100, 100), wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow47->SetMinimumPaneSize(0);

    wxPanel* itemPanel48 = new wxPanel( itemSplitterWindow47, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel48->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer49 = new wxFlexGridSizer(2, 1, 0, 0);
    itemFlexGridSizer49->AddGrowableRow(1);
    itemFlexGridSizer49->AddGrowableCol(0);
    itemFlexGridSizer49->AddGrowableCol(1);
    itemPanel48->SetSizer(itemFlexGridSizer49);

    wxFlexGridSizer* itemFlexGridSizer50 = new wxFlexGridSizer(1, 3, 0, 0);
    itemFlexGridSizer50->AddGrowableCol(3);
    itemFlexGridSizer49->Add(itemFlexGridSizer50, 0, wxGROW|wxALIGN_TOP|wxALL, 5);
    mOnlyActiveCtl = new wxToggleButton( itemPanel48, ID_TOGGLEBUTTON1, _("Only active"), wxDefaultPosition, wxDefaultSize, 0 );
    mOnlyActiveCtl->SetValue(false);
    itemFlexGridSizer50->Add(mOnlyActiveCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel48, wxID_STATIC, _("Filter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText52, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mNodeFilterTextCtl = new wxTextCtrl( itemPanel48, ID_NOTE_FILTER_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(mNodeFilterTextCtl, 0, wxGROW|wxALIGN_TOP|wxALL, 5);

    mTreeCtl = new wxTreeCtrl( itemPanel48, ID_TREECTRL, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE );
    itemFlexGridSizer49->Add(mTreeCtl, 0, wxGROW|wxGROW|wxALL, 5);

    mTabCtl = new wxNotebook( itemSplitterWindow47, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT|wxNB_TOP );

    mSummaryCtl = new wxHtmlWindow( mTabCtl, ID_HTMLWINDOW, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER );

    mTabCtl->AddPage(mSummaryCtl, _("Summary"));

    mWorkListCtl = new wxListCtrl( mTabCtl, ID_WORK_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );

    mTabCtl->AddPage(mWorkListCtl, _("Work Hours"));

    mInvoicesListCtl = new wxListCtrl( mTabCtl, ID_INVOICES_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );

    mTabCtl->AddPage(mInvoicesListCtl, _("Invoices"));

    itemSplitterWindow47->SplitVertically(itemPanel48, mTabCtl, 300);
    itemSplitterWindow46->Initialize(itemSplitterWindow47);
    wxSplitterWindow* itemSplitterWindow59 = new wxSplitterWindow( itemSplitterWindow45, ID_SPLITTER_STACK_CONTROL, wxDefaultPosition, wxSize(100, 100), wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow59->SetMinimumPaneSize(0);

    mStackListCtl = new wxListCtrl( itemSplitterWindow59, ID_STACK, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );

    wxPanel* itemPanel61 = new wxPanel( itemSplitterWindow59, ID_PANEL1, wxDefaultPosition, wxSize(-1, 0), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxVERTICAL);
    itemPanel61->SetSizer(itemBoxSizer62);

    mCheckInOutButtonCtl = new wxToggleButton( itemPanel61, ID_CHECK_IN_OUT, _("Check in"), wxDefaultPosition, wxSize(-1, 50), 0 );
    mCheckInOutButtonCtl->SetValue(false);
    itemBoxSizer62->Add(mCheckInOutButtonCtl, 0, wxGROW|wxALL, 5);

    mPauseButtonCtl = new wxToggleButton( itemPanel61, ID_PAUSE, _("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
    mPauseButtonCtl->SetValue(false);
    itemBoxSizer62->Add(mPauseButtonCtl, 0, wxGROW|wxALL, 5);

    itemBoxSizer62->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemSplitterWindow59->SplitVertically(mStackListCtl, itemPanel61, 600);
    itemSplitterWindow45->SplitHorizontally(itemSplitterWindow46, itemSplitterWindow59, 390);

    mStatusBarCtl = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    mStatusBarCtl->SetFieldsCount(3);
    itemFrame1->SetStatusBar(mStatusBarCtl);

    // Connect events and objects
    mInvoicesListCtl->Connect(ID_INVOICES_LIST, wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainFrame::OnInvoicesListLeftDClick), NULL, this);
////@end MainFrame content construction

    // Create custom windows not generated automatically here.
////@begin MainFrame content initialisation
////@end MainFrame content initialisation

    // Initialize the work-list
    {
        mWorkListCtl->InsertColumn(WLC_ORIGIN, _("Origin"), wxLIST_FORMAT_LEFT, 120);
        mWorkListCtl->InsertColumn(WLC_FROM, _("From"), wxLIST_FORMAT_LEFT, 120);
        mWorkListCtl->InsertColumn(WLC_TO, _("To"), wxLIST_FORMAT_LEFT, 50);
        mWorkListCtl->InsertColumn(WLC_PAUSED, _("Pause"), wxLIST_FORMAT_RIGHT, 50);
        mWorkListCtl->InsertColumn(WLC_ELAPSED, _("Used"), wxLIST_FORMAT_RIGHT, 50);
        mWorkListCtl->InsertColumn(WLC_CHARGE, _("Sum"), wxLIST_FORMAT_RIGHT, 50);
        mWorkListCtl->InsertColumn(WLC_NOTES, _("Description"), wxLIST_FORMAT_LEFT, 500);
    }

     // Initialize the invoices-list
    {
        mInvoicesListCtl->InsertColumn(ILC_DATE, _("Date"), wxLIST_FORMAT_LEFT, 80);
        mInvoicesListCtl->InsertColumn(ILC_ID, _("#"), wxLIST_FORMAT_RIGHT, 50);
        mInvoicesListCtl->InsertColumn(ILC_CUST, _("Customer"), wxLIST_FORMAT_LEFT, 110);
        mInvoicesListCtl->InsertColumn(ILC_USER, _("User"), wxLIST_FORMAT_LEFT, 80);
        mInvoicesListCtl->InsertColumn(ILC_HOURS, _("Hours"), wxLIST_FORMAT_RIGHT, 65);
        mInvoicesListCtl->InsertColumn(ILC_CHARGE, _("Sum"), wxLIST_FORMAT_RIGHT, 80);
        mInvoicesListCtl->InsertColumn(ILC_PROFIT, _("Profit"), wxLIST_FORMAT_RIGHT, 80);
    }

    

    // Initialize the stack-list
    {
        mStackListCtl->InsertColumn(SLC_FROM, _("From"), wxLIST_FORMAT_LEFT, 50);
        mStackListCtl->InsertColumn(SLC_TO, _("To"), wxLIST_FORMAT_LEFT, 50);
        mStackListCtl->InsertColumn(SLC_PAUSED, _("Pause"), wxLIST_FORMAT_RIGHT, 50);
        mStackListCtl->InsertColumn(SLC_ELAPSED, _("Used"), wxLIST_FORMAT_RIGHT, 50);
        mStackListCtl->InsertColumn(WLC_NOTES, _("Description"), wxLIST_FORMAT_LEFT, 500);
    }
}

/*!
 * Should we show tooltips?
 */

bool MainFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap MainFrame::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    
    
#if 0

    // Bitmap retrieval
////@begin MainFrame bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("icons/32x32/add_user.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/add_user.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/32x32/add_project.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/add_project.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/32x32/new_task.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/new_task.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/32x32/add_work.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/add_work.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/32x32/billing_small.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/billing_small.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/32x32/reload.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/reload.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("icons/32x32/reset_filter.png"))
    {
        wxBitmap bitmap(_T("icons/32x32/reset_filter.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    return wxNullBitmap;
////@end MainFrame bitmap retrieval

#endif // 0
}

/*!
 * Get icon resources
 */

wxIcon MainFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MainFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MainFrame icon retrieval
}
/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_CUSTOMER
 */

void MainFrame::OnNewCustomerClick( wxCommandEvent& event )
{
    CustomerDlg dlg;
    dlg.mData.mActiveStatus = WHID::ASAUTO;
    dlg.Create(0, WhidEngine::GetEngine().GetWhidControl(), this);
    if (dlg.ShowModal() == wxID_OK)
    {
        try
        {
            WHID::CustomerPrx cust = 
                WhidEngine::GetEngine().GetWhidControl()->CreateCustomer(dlg.mData, CTX);
            if (cust)
            {
                wxTreeItemId tree_item = mTreeCtl->GetRootItem();
                AddChild<TreeNodeCustomer>(tree_item, cust, true);
                cust->SetRates(dlg.mRates, CTX);
            }
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to create customer: "
                << ex
                << war_endl;
        }

        //ReloadNodes();
    }
}


void MainFrame::ReloadNodes()
{
    if (IsCrippled())
        return;

    const bool filterActive = mOnlyActiveCtl->GetValue();
    
    std::string filter_text;
    STR_FROM_DLG(filter_text, mNodeFilterTextCtl);
    std::string my_filter;
    if (!filter_text.empty())
    {
        my_filter = "%";
        my_filter += filter_text;
        my_filter += "%";
    }
    
    mTreeCtl->DeleteAllItems();
    wxTreeItemId root = mTreeCtl->AddRoot(_("Customers"));
   
    try
    {
        WHID::NodeInfoList list;
        WhidEngine::GetEngine().GetWhidControl()->GetCustomers(
            WHID::CFNORMAL, my_filter.c_str(), filterActive, list, CTX);

        AddChildren<TreeNodeCustomer>(root, list, true, filterActive);
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to load nodes: "
            << ex
            << war_endl;
    }
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_CUSTOMER
 */

void MainFrame::OnEditCustomerClick( wxCommandEvent& event )
{
    TreeNodeCustomer *node = GetSelectedCustomer();
    if (NULL != node)
    {
        CustomerDlg dlg;
        dlg.mData.mActiveStatus = WHID::ASAUTO;
        node->mNode->GetData(dlg.mData, CTX);
        dlg.Create(node->mNode, WhidEngine::GetEngine().GetWhidControl(), this);
        if (dlg.ShowModal() == wxID_OK)
        {
            try
            {
                node->mNode->SetData(dlg.mData, CTX);
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to update customer: "
                    << ex
                    << war_endl;
            }

            //ReloadNodes();
        }
    }
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_EDIT_CUSTOMER
 */

void MainFrame::OnEditCustomerUpdate( wxUpdateUIEvent& event )
{
    // Find the current selected customer
    event.Enable(GetSelectedCustomer() != NULL);
}


TreeNodeCustomer *MainFrame::GetSelectedCustomer(wxTreeItemId *treeItem)
{
    return GetSelected<TreeNodeCustomer>(treeItem);
} 

TreeNodeProject *MainFrame::GetSelectedProject(wxTreeItemId *treeItem)
{
    return GetSelected<TreeNodeProject>(treeItem);
} 

TreeNodeTask *MainFrame::GetSelectedTask(wxTreeItemId *treeItem)
{
    return GetSelected<TreeNodeTask>(treeItem);
} 

int MainFrame::GetSelectedWorkItem()
{
        return mWorkListCtl->GetNextItem(-1, 
            wxLIST_NEXT_ALL, 
            wxLIST_STATE_SELECTED); 
}

/*!
 * wxEVT_CONTEXT_MENU event handler for ID_MAINFRAME
 */

void MainFrame::OnContextMenu( wxContextMenuEvent& event )
{
    const int id = event.GetId();
    wxMenu *pmenu = NULL;

    // See if the event belongs to the server-tree
    if (id == mTreeCtl->GetId())
    {
        // See if we have an active selected node
        wxTreeItemId selected_item = mTreeCtl->GetSelection();

        if (selected_item.IsOk())
        {
            // Select the proper menu according to the selected type
            TreeNode *node = 
                dynamic_cast<TreeNode *>(mTreeCtl->GetItemData(selected_item));
            if (node) switch(node->GetType())
            {
                case WHID::NODECUSTOMER:
                    pmenu = AppResources::CreateCustomerPopupMenu();
                    break;
                 case WHID::NODEPROJECT:
                    pmenu = AppResources::CreateProjectPopupMenu();
                    break;
                case WHID::NODETASK:
                    pmenu = AppResources::CreateTaskPopupMenu();
                    break;
            }
        }
    }
    else if (id == mWorkListCtl->GetId())
    {
        if (event.GetInt() >= 0)
            pmenu = AppResources::CreateWorkPopupMenu();

        //EditEvent(event.GetInt());
    }
    else if (id == mInvoicesListCtl->GetId())
    {
        if (event.GetInt() >= 0)
            pmenu = AppResources::CreateInvoicesPopupMenu();

        //EditEvent(event.GetInt());
    }

    //if (NULL == pmenu)
    //{
    //    // Use the default menu where no nodes are selected
    //    pmenu = AppResources::CreateDefaultContextMenuForServerTree();
    //}

    if (pmenu)
        PopupMenu(pmenu);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_PROJECT
 */

void MainFrame::OnNewProjectClick( wxCommandEvent& event )
{
  wxTreeItemId tree_item = (void*)NULL;
    TreeNodeCustomer *customer = GetSelectedCustomer(&tree_item);
    if (customer)
    {
        ProjectDlg dlg;
        dlg.mData.mActiveStatus = WHID::ASAUTO;
        dlg.Create(0, WhidEngine::GetEngine().GetWhidControl(), this);
        if (dlg.ShowModal() == wxID_OK)
        {
            try
            {
                WHID::ProjectPrx prj = 
                    WHID::CustomerPrx::uncheckedCast(
                    customer->mNode)->CreateProject(dlg.mData, CTX);
                if (prj)
                {
                    AddChild<TreeNodeProject>(tree_item, prj, true);
                    prj->SetRates(dlg.mRates, CTX);
                }
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to create customer: "
                    << ex
                    << war_endl;
            }

            //ReloadNodes();
        }
    }
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_NEW_PROJECT
 */

void MainFrame::OnNewProjectUpdate( wxUpdateUIEvent& event )
{
    event.Enable((GetSelectedCustomer() != NULL) && IS_ADMIN);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_PROJECT
 */

void MainFrame::OnEditProjectClick( wxCommandEvent& event )
{
    TreeNodeProject *node = GetSelectedProject();
    if (NULL != node)
    {
        ProjectDlg dlg;
        dlg.mData.mActiveStatus = WHID::ASAUTO;
        node->mNode->GetData(dlg.mData, CTX);
        dlg.Create(node->mNode, WhidEngine::GetEngine().GetWhidControl(), this);
        if (dlg.ShowModal() == wxID_OK)
        {
            try
            {
                node->mNode->SetData(dlg.mData, CTX);
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to update customer: "
                    << ex
                    << war_endl;
            }

            //ReloadNodes();
        }
    }
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_EDIT_PROJECT
 */

void MainFrame::OnEditProjectUpdate( wxUpdateUIEvent& event )
{
    event.Enable(GetSelectedProject() != NULL);
}


/*!
 * wxEVT_COMMAND_TREE_ITEM_EXPANDING event handler for ID_PROJECTS
 */

void MainFrame::OnProjectsItemExpanding( wxTreeEvent& event )
{
    wxTreeItemId id = event.GetItem();
    if (id.IsOk())
    {

        const bool filterActive = mOnlyActiveCtl->GetValue();

        TreeNode *node = 
            dynamic_cast<TreeNode *>(mTreeCtl->GetItemData(id));
        if (NULL != node)
        {
            try
            {
                WHID::NodeInfoList children;
                node->mNode->GetChildren(filterActive, children, CTX);

                switch(node->GetType())
                {
                case WHID::NODECUSTOMER:
                    AddChildren<TreeNodeProject>(id, children, true, filterActive);
                    break;
                case WHID::NODEPROJECT:
                    AddChildren<TreeNodeTask>(id, children, false, filterActive);
                    break;
                default:
                    mTreeCtl->SetItemHasChildren(id, false);
                }
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to load nodes: "
                    << ex
                    << war_endl;
            }
        }
    }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_TASK
 */

void MainFrame::OnNewTaskClick( wxCommandEvent& event )
{
  wxTreeItemId tree_item = (void *)NULL;
    TreeNodeProject *project = GetSelectedProject(&tree_item);
    if (project)
    {
        TaskDlg dlg;
        dlg.mData.mActiveStatus = WHID::ASAUTO;
        dlg.Create(0, WhidEngine::GetEngine().GetWhidControl(), this);
        if (dlg.ShowModal() == wxID_OK)
        {
            try
            {
                WHID::TaskPrx task = 
                    WHID::ProjectPrx::uncheckedCast(
                    project->mNode)->CreateTask(dlg.mData, CTX);
                if (task)
                {
                    AddChild<TreeNodeTask>(tree_item, task, false);
                    task->SetRates(dlg.mRates, CTX);
                }
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to create project: "
                    << ex
                    << war_endl;
            }

            //ReloadNodes();
        }
    }
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_NEW_TASK
 */

void MainFrame::OnNewTaskUpdate( wxUpdateUIEvent& event )
{
     event.Enable((GetSelectedProject() != NULL) && IS_ADMIN);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_TASK
 */

void MainFrame::OnEditTaskClick( wxCommandEvent& event )
{
    TreeNodeTask *node = GetSelectedTask();
    if (NULL != node)
    {
        TaskDlg dlg;
        dlg.mData.mActiveStatus = WHID::ASAUTO;
        node->mNode->GetData(dlg.mData, CTX);
        dlg.Create(node->mNode, WhidEngine::GetEngine().GetWhidControl(), this);
        if (dlg.ShowModal() == wxID_OK)
        {
            try
            {
                node->mNode->SetData(dlg.mData, CTX);
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to update customer: "
                    << ex
                    << war_endl;
            }

            //ReloadNodes();
        }
    }
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_EDIT_TASK
 */

void MainFrame::OnEditTaskUpdate( wxUpdateUIEvent& event )
{
    event.Enable(GetSelectedTask() != NULL);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_REFRESH
 */

void MainFrame::OnREFRESHClick( wxCommandEvent& event )
{
    ReloadNodes();
    UpdateSummary();
    ReloadCurrentList();    

    RefreshStack();
}


/*!
 * wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK
 */

void MainFrame::OnNotebookPageChanged( wxNotebookEvent& event )
{
    ReloadCurrentList();
}

void MainFrame::ReloadWorklist(const bool statsOnly)
{
    if (IsCrippled())
        return;

    double total_pause = 0, total_hours = 0, total_sum = 0;

    if (!statsOnly)
    {
        mWorkListCtl->DeleteAllItems();
        mWorkListItems.clear();
    }

    try
    {
        //wxTreeItemId node = mTreeCtl->GetSelection();

        TreeNode *node = GetSelected<TreeNode>();
        if (NULL != node)
        {
            if (!statsOnly)
            {
                WHID::HaveChargesPrx::uncheckedCast(node->mNode)->GetWork(
                    USER,
                    (WHID::WorkFilterE)
                    mWorkHoursStatusFilterCtl->GetCurrentSelection(),
                    "", "",
                    mWorkListItems,
                    CTX);
            }

            const bool stat_for_selected = mWorkListCtl->GetSelectedItemCount() > 1;

            WHID::WorkInfoList::iterator work;
            int row = 0;
            for(work = mWorkListItems.begin()
                ; work != mWorkListItems.end()
                ; work++, row++)
            {
                const WHID::WorkInfo& wi = *work;

                bool do_stat = true;
                if (stat_for_selected)
                {
                    if (!mWorkListCtl->GetItemState(row, wxLIST_STATE_SELECTED))
                        do_stat = false;
                }

                if (do_stat)
                {
                    total_pause += wi.mMinutesPaused;
                    total_hours += wi.mMinutesUsed;
                    total_sum += wi.mCharge;
                }

                if (!statsOnly)
                    SetDataInWorklist(-1, row, wi);
            }

            if (!mWorkListItems.empty())
            {
                // Add summary filed

                WHID::WorkInfo wi;
                wi.mMinutesUsed = (Ice::Int)total_hours;
                wi.mMinutesPaused = (Ice::Int)total_pause;
                wi.mCharge = total_sum;
                wi.mStatus = (WHID::WorkStatusE)-1;

                int my_row = -1;
                if (statsOnly)
                    my_row = mWorkListCtl->GetItemCount() -1;

                SetDataInWorklist(my_row, -1, wi, true);
            }
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to load work-hours: "
            << ex
            << war_endl;
    }
}

int MainFrame::SetDataInWorklist(int nitem, 
                                 int row,
                                 const WHID::WorkInfo wi,
                                 const bool isStats)
{
    WarUtf8 from_str, to_str, notes_str;

    notes_str = WarLog::Safe(wi.mNotes.c_str());

    if (!isStats)
    {
        WarTime wt_from, wt_to;
        wt_from.AssignFromAnsiDateTime(wi.mFromTime.c_str(), false, true);
        wt_to.AssignFromAnsiDateTime(wi.mToTime.c_str(), false, true);
        //wt_from.ToLT();
        //wt_to.ToLT();
        from_str = wt_from.FormatLT("%x %H:%M");
        to_str = wt_to.FormatLT("%H:%M");
    }

    WarCollector<wxChar> origin;
    if (!isStats)
        origin << wi.mCustomerName << "/" << wi.mProjectName << "/" << wi.mTaskName;


    wxString wx_paused, wx_elapsed, wx_charge;
    int hours = 0, minutes = 0;
    hours = wi.mMinutesPaused / 60;
    minutes = wi.mMinutesPaused % 60;

    wx_paused = wxString::Format(_("%02d:%02d"), hours, minutes);

    hours = wi.mMinutesUsed / 60;
    minutes = wi.mMinutesUsed % 60;
    wx_elapsed = wxString::Format(_("%02d:%02d"), hours, minutes);

    wx_charge = wxString::Format(_("%.0f"), wi.mCharge);

    if (-1 == nitem)
    {
        const int my_row = (0 <= row) ? row : mWorkListCtl->GetItemCount();
        // Add a new line
        nitem = mWorkListCtl->InsertItem(
            my_row, origin.GetValue().c_str(), 
            wi.mStatus);
        mWorkListCtl->SetItemData(nitem, row);
    }
    else
    {
        // Update
        mWorkListCtl->SetItem(nitem, 0, origin.GetValue().c_str(), wi.mStatus);
    }

    if (-1 == row)
    {
        mWorkListCtl->SetItemBackgroundColour(nitem, *wxLIGHT_GREY);
    }
    
    mWorkListCtl->SetItem(nitem, WLC_FROM, from_str.GetUnicode().c_str());
    mWorkListCtl->SetItem(nitem, WLC_TO, to_str.GetUnicode().c_str());
    mWorkListCtl->SetItem(nitem, WLC_PAUSED, wx_paused);
    mWorkListCtl->SetItem(nitem, WLC_ELAPSED, wx_elapsed);
    mWorkListCtl->SetItem(nitem, WLC_CHARGE, wx_charge);
    mWorkListCtl->SetItem(nitem, WLC_NOTES, notes_str.GetUnicode().c_str());

    return nitem;
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_WORK
 */

void MainFrame::OnNewWorkClick( wxCommandEvent& event )
{
    TreeNodeTask *task = GetSelectedTask();
    if (task)
    {
        try
        {
            //WorkDlg dlg(task->mNode, this);

            WarTime now;
            //now.ToLT();
            int align = 15;
            struct tm tm;
            WarTime::WarLocaltime(now.GetTime(), tm);
            int minutes = (tm.tm_hour * 60) + tm.tm_min;
            int from_minutes = (minutes / align) * align;
            /*if (minutes % align)
                from_minutes -= align;*/
            int to_minutes = from_minutes + 60;

            char buf[128];
            sprintf(buf, "%04d-%02d-%02d %02d:%02d:00",
                tm.tm_year + 1900, tm.tm_mon + 1,
                tm.tm_mday, (from_minutes / 60), (from_minutes % 60));

            WHID::WorkData data;
            data.mMinutesPaused = 0;
            data.mFromTime = buf;
            //data. = WHID::WSPENDINGINV;

            if (tm.tm_hour == 23)
            {
                // Roll over to next day
                now.mTime += 24 * 60 * 60 * 1000;
                tm = now.GetTm();
                to_minutes -= 60;
            }

            sprintf(buf, "%04d-%02d-%02d %02d:%02d:00",
                tm.tm_year + 1900, tm.tm_mon + 1,
                tm.tm_mday, (to_minutes / 60), (to_minutes % 60));

            data.mToTime = buf;

            WHID::WorkPrx work = WHID::TaskPrx::uncheckedCast(
                task->mNode)->CreateWork(USER, data, CTX);
            WorkDlg dlg;
            dlg.mTrackLastSetDate = true;
            dlg.Create(work, WhidEngine::GetEngine().GetWhidControl(), this);
            if (dlg.ShowModal() == wxID_OK)
            {
                ReloadCurrentList();
            }
            else
                work->Delete(CTX);

        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to load work-hours: "
                << ex
                << war_endl;
        }
    }

}

/*!
 * wxEVT_UPDATE_UI event handler for ID_NEW_WORK
 */

void MainFrame::OnNewWorkUpdate( wxUpdateUIEvent& event )
{
    event.Enable(GetSelectedTask() != NULL);
}



/*!
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_PROJECTS
 */

void MainFrame::OnProjectsSelChanged( wxTreeEvent& event )
{
    ReloadCurrentList();

    // Remove the selectition on the stack.
    UnselectStack();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MANAGE_RATES
 */

void MainFrame::OnManageRatesClick( wxCommandEvent& event )
{
    ChooseRateDlg dlg;
    dlg.Create(WhidEngine::GetEngine().GetWhidControl(), this);
    dlg.ShowModal();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECK_IN_OUT
 */

void MainFrame::OnCheckInOutClick( wxCommandEvent& event )
{
    // See if a stack-item is selected
    const long stack_item = GetSelectedStackItem();
    if (0 <= stack_item)
    {
        // Check-out action

        // Find the iterator for the seected stack-item
        stack_t::iterator item;
        int row = 0;
        for(item = mWorkStack.begin()
            ; (item != mWorkStack.end()) && (row < stack_item)
            ; item++)
        {
            row++;
        }

        if (item != mWorkStack.end())
        {

            WHID::WorkData data;

            try
            {
                item->mWork->CloseTimer(CTX);
                StackResetTimer(*item);
            }
            catch(const Ice::Exception& ex)
            {
                WAR_ERRLOG;
                err_log << "Failed to close timer: "
                    << ex
                    << war_endl;
            }

            mWorkStack.erase(item);

            // Make sure the pause-timer on the last item is off
            if (!mWorkStack.empty())
                StackResetTimer(mWorkStack.back());
        }
        
        RefreshStack();
        ReloadCurrentList();

        return;
    }

    // Check-in action

    // Create a new work.item.
    TreeNodeTask *task = GetSelectedTask();
    if (!task)
        return;

    stacked_info item;
    item.mPaused.mTime = 0;

    item.mWork = WHID::TaskPrx::uncheckedCast(
        task->mNode)->StartTimer(USER, CTX);

    {
        WorkDlg dlg;
        dlg.Create(item.mWork, 
            WhidEngine::GetEngine().GetWhidControl(), this);
        if (dlg.ShowModal() == wxID_OK)
        {
            if (item.mWork->GetStatus(CTX) == WHID::WSACTIVE)
            {
                // Start the timer for the uppermost item
                if (!mWorkStack.empty())
                    mWorkStack.back().mPaused.Reset();

                mWorkStack.push_back(item);
                RefreshStack();

                // Select the item
                mStackListCtl->SetItemState(mStackListCtl->GetItemCount() -1,
                    wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            }

        }
        else
        {
            item.mWork->Delete(CTX);
        }

        ReloadCurrentList();
    }

    // Put it on the stack
    
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECK_IN_OUT
 */

void MainFrame::OnCheckInOutUpdate( wxUpdateUIEvent& event )
{
    wxString checked_text = _("Check Out");
    wxString unchecked_text = _("Check In");

    const int selected_item = GetSelectedStackItem();

    if (selected_item >= 0)
    {
        if (mWorkStack[selected_item].mWork)
        {
            event.Enable(true);
            if (!mCheckInOutButtonCtl->GetValue())
                mCheckInOutButtonCtl->SetValue(true);
            if (mCheckInOutButtonCtl->GetLabel() != checked_text)
                mCheckInOutButtonCtl->SetLabel(checked_text);
        }
        else
        {
            // Pause
             event.Enable(false);
        }
    }
    else
    {
        event.Enable(GetSelectedTask() != NULL);
        if (mCheckInOutButtonCtl->GetValue())
            mCheckInOutButtonCtl->SetValue(false);
        if (mCheckInOutButtonCtl->GetLabel() != unchecked_text)
            mCheckInOutButtonCtl->SetLabel(unchecked_text);
    }
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PAUSE
 */

void MainFrame::OnPauseClick( wxCommandEvent& event )
{
    if (mIsPaused)
    {
        // Remove

        if (!mWorkStack.empty())
        {
            if (!mWorkStack.back().mWork)
                mWorkStack.pop_back();

            RefreshStack();
        }

        return;
    }

    // Add a pause to the stack   

    stacked_info item;
    item.mPaused.mTime = 0;

    if (!mWorkStack.empty())
        mWorkStack.back().mPaused.Reset();

    mWorkStack.push_back(item);
    RefreshStack();

    // Select the item
    mStackListCtl->SetItemState(mStackListCtl->GetItemCount() -1,
        wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_PAUSE
 */

void MainFrame::OnPauseUpdate( wxUpdateUIEvent& event )
{
    event.Enable(mIsPaused || (mWorkStack.size() > 0));

    if (mIsPaused)
    {
        if (!mPauseButtonCtl->GetValue())
            mPauseButtonCtl->SetValue(true);
    }
    else 
    {
        if (mPauseButtonCtl->GetValue())
            mPauseButtonCtl->SetValue(false);
    }
}


void MainFrame::RefreshStack()
{
again:

    mStackListCtl->DeleteAllItems();
    mIsPaused = false;

    try
    {
        stack_t::iterator item;
        int row = 0;
        for(item = mWorkStack.begin()
            ; item != mWorkStack.end()
            ; item++, row++)
        {
            const bool is_last = row == ((int)mWorkStack.size() -1);
            const bool is_pause = item->mWork == false;

            if (is_pause)
            {
                // Remove pauses that's not on the top of the stack
                if (!is_last)
                {
                    mWorkStack.erase(item);
                    goto again;
                }


                mIsPaused = true;
            }
            else
            {
                // Reset the timer on the last element. This must
                // never be "on hold".

                if (is_last && !item->mPaused.IsEmpty())
                    StackResetTimer(*item);
            }

            WHID::WorkInfo work;
            if (!is_pause)
                item->mWork->GetInfo(work, CTX);
            
            WarUtf8 notes_str = WarLog::Safe(work.mNotes.c_str());

            WarTime wt_from, wt_to;

            if (is_pause)
            {
                wt_from = item->mCreated;
                work.mMinutesPaused = (int)
                    ((wt_to.mTime - item->mCreated.mTime) / 60000L);
                notes_str = "Pause";
            }
            else
            {
                wt_from.AssignFromAnsiDateTime(work.mFromTime.c_str(), false, true);
                wt_to.AssignFromAnsiDateTime(work.mToTime.c_str(), false, true);
            }

            WarUtf8 from_str = wt_from.FormatLT("%H:%M");
            WarUtf8 to_str = wt_to.FormatLT("%H:%M");

            wxString wx_paused, wx_elapsed, wx_charge;

            int paused = work.mMinutesPaused;
            if (!item->mPaused.IsEmpty())
            {
                WarTime now;
                int local_pause = (int)((now.mTime - item->mPaused.mTime) / 60000L);
                if (local_pause > 0)
                    paused += local_pause;
            }

            wx_paused = WhidApp::GetMinutes(paused);
            wx_elapsed = WhidApp::GetMinutes(work.mMinutesUsed);
            //wx_elapsed = WhidApp::GetMinutes();

            int image = is_pause ? 5 : item->mWork->GetStatus(CTX);
            if (!item->mPaused.IsEmpty())
                image = 6;

            const int nitem = mStackListCtl->InsertItem(
                row, wxString(from_str.GetUnicode().c_str()), 
                image);
            //mStackListCtl->SetItemData(nitem, item);
            mStackListCtl->SetItem(nitem, SLC_TO, to_str.GetUnicode().c_str());
            mStackListCtl->SetItem(nitem, SLC_PAUSED, wx_paused);
            if (!is_pause)
                mStackListCtl->SetItem(nitem, SLC_ELAPSED, wx_elapsed);
            mStackListCtl->SetItem(nitem, SLC_NOTES, notes_str.GetUnicode().c_str());

            if (!item->mPaused.IsEmpty())
            {
                mStackListCtl->SetItemBackgroundColour(nitem, 
                    wxTheColourDatabase->Find(_("WHEAT")));
            }
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to load stacked work: "
            << ex
            << war_endl;
    }
}

void MainFrame::UnselectStack()
{
    long item = -1;     
    for ( ;; ) 
    { 
        item = mStackListCtl->GetNextItem(item, 
            wxLIST_NEXT_ALL, 
            wxLIST_STATE_SELECTED); 
        if ( item == -1 ) 
            break; 

        mStackListCtl->SetItemState(item, 0, wxLIST_STATE_SELECTED);
    } 
}

int MainFrame::GetSelectedStackItem()
{
    return mStackListCtl->GetNextItem(-1, 
        wxLIST_NEXT_ALL, 
        wxLIST_STATE_SELECTED); 
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_WORK_LIST
 */

void MainFrame::OnWorkListSelected( wxListEvent& event )
{
    event.Skip();
    UnselectStack();
    ReloadWorklist(true);
}


/*!
 * wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_PROJECTS
 */

void MainFrame::OnProjectsItemActivated( wxTreeEvent& event )
{
    UnselectStack();
    OnCheckInOutClick(event);
}


/*!
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_WORK_LIST
 */

void MainFrame::OnWorkListItemActivated( wxListEvent& event )
{
    EditWork(event.GetData());
}

void MainFrame::EditWork(const int itemIndex)
{
    if (0 <= itemIndex)
    {
        try
        {

            WorkDlg dlg;
            dlg.Create(mWorkListItems[itemIndex].mNode, 
                WhidEngine::GetEngine().GetWhidControl(), this);
            if (dlg.ShowModal() == wxID_OK)
            {
                ReloadCurrentList();
            }
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to load work-hours: "
                << ex
                << war_endl;
        }
    }
}


/*!
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_STACK
 */

void MainFrame::OnStackItemActivated( wxListEvent& event )
{
    int index = event.GetIndex();
    if (0 <= index)
    {
        if (!mWorkStack[index].mWork)
        {
            wxMessageBox(_("No properties for pauses..."));
            return;
        }

        try
        {
            WorkDlg dlg;
            dlg.Create(mWorkStack[index].mWork, 
                WhidEngine::GetEngine().GetWhidControl(), this);
            if (dlg.ShowModal() == wxID_OK)
            {
                ReloadCurrentList();

                RefreshStack();
                // Select the item
                mStackListCtl->SetItemState(index,
                    wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

            }
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to load work-hours: "
                << ex
                << war_endl;
        }
    }
}


void MainFrame::UpdateWorkListItem(WHID::WorkPrx work)
{
    const TabPagesE tab = (TabPagesE) mTabCtl->GetSelection();
    if (TAB_WORKLIST != tab)
        return; // The workk-lits is not active

    const Ice::Identity id = work->ice_getIdentity();
    Ice::Identity tmp_id;

    const int items = mWorkListCtl->GetItemCount();
    const int work_items = (int)mWorkListItems.size();

    for(int row = 0; row < items; row++)
    {
        int item = mWorkListCtl->GetItemData(row);
        if ((item >= 0) && (item < work_items))
        {
            tmp_id = mWorkListItems[item].mNode->ice_getIdentity();
            if (id == tmp_id)
            {
                // Update the cached info 
                work->GetInfo(mWorkListItems[item], CTX);

                // Wpdate the visual list
                SetDataInWorklist(row, row, mWorkListItems[item]);
            }
        }
    }
}

void MainFrame::OnTimer(wxTimerEvent& event)
{
    // Do the stack
    
    try
    {
        stack_t::iterator item, next_item;
        for(item = mWorkStack.begin(); item != mWorkStack.end(); item++)
        {
            if (item->mWork)
            {
                item->mWork->SetEndtimeToNow(CTX);

                // Update this item in work-list
                UpdateWorkListItem(item->mWork);
            }
        }

        ReloadWorklist(true);
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to update pause: "
            << ex
            << war_endl;
    }

    // refresh the stack, but make sure the selected item remains selected
    int selected_stack_item = GetSelectedStackItem();
    RefreshStack();
    if (selected_stack_item >= 0)
    {
        mStackListCtl->SetItemState(selected_stack_item,
            wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    }
}

void MainFrame::StackResetTimer(stacked_info& item)
{
    if (!item.mPaused.IsEmpty())
    {
        WarTime now;
        int minutes = (int)((now.mTime - item.mPaused.mTime) / 60000L);
        if (minutes > 0)
        {
            if (item.mWork)
                item.mWork->AddPause(minutes, CTX);
        }

        item.mPaused.mTime = 0;
    }
}




/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_MAINFRAME
 */

void MainFrame::OnCloseWindow( wxCloseEvent& event )
{
    event.Skip();

    // Close the items on the stakk before we exit.
    try
    {
        stack_t::iterator item, next_item;
        for(item = mWorkStack.begin(); item != mWorkStack.end(); item++)
        {
            if (item->mWork)
            {
                mWorkStack.back().mWork->CloseTimer(CTX);
                StackResetTimer(*item);
            }
        }

        mWorkStack.clear();
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to close timer: "
            << ex
            << war_endl;
    }
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_WORK_HOURS_STATUS_FILTER
 */

void MainFrame::OnWorkHoursStatusFilterSelected( wxCommandEvent& event )
{
    ReloadCurrentList();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_RESET_FILTERS
 */

void MainFrame::OnResetFiltersClick( wxCommandEvent& event )
{
    mWorkHoursStatusFilterCtl->SetSelection(WHID::WFOPEN);
    mOnlyActiveCtl->SetValue(false);
    mNodeFilterTextCtl->SetValue(wxT(""));
}


void MainFrame::UpdateSummary()
{
    if (IsCrippled())
        return;

#define M(val) WhidApp::GetMinutesAsString(val)

    WarCollector<wxChar> out;

    std::vector<int> day_totals;
    int grand_total = 0;
    day_totals.resize(7,0);

    out << "<html><head><title>Summary</title></head>\n";
    out << "<h1>Summary week</h1>\n";
    
    out << "<table border=1><tr>"
        << "<th>Customer</th>"
        << "<th>Project</th>"
        << "<th>Mon</th>"
        << "<th>Tue</th>"
        << "<th>Wed</th>"
        << "<th>Thu</th>"
        << "<th>Fri</th>"
        << "<th>Sat</th>"
        << "<th>Sun</th>"
        << "<th>Tot</th>\n</tr>\n";
       
    try
    {
        std::string when = WarTime().FormatLT();
        WHID::ProjectSummaryList list;

        USER->GetProjectSummary(WHID::PSMWEEK, when, list, CTX);

        WHID::ProjectSummaryList::iterator prj;
        int day_index = 0;
        for(prj = list.begin(); prj != list.end(); prj++)
        {

            out << "<tr><td>" 
                << prj->mCompanyName
                << "</td><td>"
                << prj->mProjectName
                << "</td>";

            
            WHID::WorkMinutesList::const_iterator day;
            day_index = 0;
            for(day = prj->mWorked.begin()
                ; day != prj->mWorked.end()
                ; day++)
            {
                
                out << "<td>" << M(*day) << "</td>";
                day_totals[day_index] += *day;
                grand_total += *day;
                day_index++;
            }

            out << "<td>" << M(prj->mTotalMinutes) << "</td></tr>\n";
        }

        out << "<tr><td colspan=\"2\"></td>";
        for(day_index = 0; day_index < 7; day_index++)
            out << "<td>" << M(day_totals[day_index]) << "</td>";
        out << "<td>" << M(grand_total) << "</td></tr>\n";
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to close timer: "
            << ex
            << war_endl;
    }
    
    out << "</table></html>\n";

    // Convert to unicode
    mSummaryCtl->SetPage(out.GetValue().c_str());

#undef M
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_PROPERTIES
 */

void MainFrame::OnPROPERTIESClick( wxCommandEvent& event )
{
    PropertiesDlg dlg;
    dlg.Create(this);
    dlg.ShowModal();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void MainFrame::OnABOUTClick( wxCommandEvent& event )
{
    AboutDlg dlg(this);
    dlg.ShowModal();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_BILLING
 */

void MainFrame::OnBillingClick( wxCommandEvent& event )
{
    BillingWiz wiz(this);
    wiz.Run();
}

bool MainFrame::HaveDatabase() const
{
    return WhidEngine::GetEngine().GetDbPool().GetCurrentConnections() > 0;
}
/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void MainFrame::OnEXITClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in MainFrame.
    // Before editing this code, remove the block markers.
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in MainFrame. 
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_MANAGE_RATES
 */

void MainFrame::OnManageRatesUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsCrippled() && IS_ADMIN);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_NEW_CUSTOMER
 */

void MainFrame::OnNewCustomerUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsCrippled() && IS_ADMIN);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_ID_NEW_TASK
 */

void MainFrame::OnIdNewTaskUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsCrippled() && IS_ADMIN);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_BILLING
 */

void MainFrame::OnBillingUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsCrippled());
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_WORK
 */

void MainFrame::OnEditWorkClick( wxCommandEvent& event )
{
    EditWork(GetSelectedWorkItem());
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_EDIT_WORK
 */

void MainFrame::OnEditWorkUpdate( wxUpdateUIEvent& event )
{
    event.Enable(GetSelectedWorkItem() >= 0);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_DELETE_WORK
 */

void MainFrame::OnDeleteWorkClick( wxCommandEvent& event )
{
    long item = -1;     
    for ( ;; ) 
    { 
        item = mWorkListCtl->GetNextItem(item, 
            wxLIST_NEXT_ALL, 
            wxLIST_STATE_SELECTED); 
        if ( item == -1 ) 
            break; 

        try
        {
            mWorkListItems[item].mNode->Delete(CTX);
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to delete one or more items. "
                << ex
                << war_endl;
            
            break;
        }
    } 

    ReloadCurrentList();
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_DELETE_WORK
 */

void MainFrame::OnDeleteWorkUpdate( wxUpdateUIEvent& event )
{
    event.Enable(GetSelectedWorkItem() >= 0);
}


void MainFrame::LoadOptions()
{
    try
    {
        WhidEngine::GetEngine().GetWhidControl()->GetOptions(mOptions);
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to load global options: "
            << ex
            << war_endl;
    }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_GLOBAL_SETTINGS
 */

void MainFrame::OnGlobalSettingsClick( wxCommandEvent& event )
{
    SystemSettingsDlg dlg(this, mOptions);
    const int result = dlg.ShowModal();
    if (result == wxID_OK)
    {
        try
        {
            mOptions = dlg.mOptions;
            WhidEngine::GetEngine().GetWhidControl()->SetOptions(mOptions, CTX);

        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to load global options: "
                << ex
                << war_endl;
        }
    }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MANAGE_USERS
 */

void MainFrame::OnManageUsersClick( wxCommandEvent& event )
{
    UserManager user_manager;
    user_manager.Create(WhidEngine::GetEngine().GetWhidControl(), this);
    user_manager.ShowModal();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MANAGE_USERS
 */

void MainFrame::OnManageUsersUpdate( wxUpdateUIEvent& event )
{
    // Check that we are in multiuser-mode
    event.Enable(IS_ADMIN);
}

bool MainFrame::Login()
{
    if (IsCrippled() || !IsMultiuser())
    {
        mIsAdmin = true;
        mStatusBarCtl->SetStatusText(_("Default user"), SB_USER);
        mStatusBarCtl->SetStatusText(_("Admin"), SB_ADMIN);
        return true;
    }

    // Try to log on
    LogonDlg dlg;
    dlg.Create(this);
	int tries = 0;
	for(tries = 0; tries < 5; tries++)
	{
		if (tries > 0)
		{
			wxBusyInfo wait(_("Waiting a bit before the next retry...")); 
			WarTime::Sleep(tries * 1000);
		}

		if (dlg.ShowModal() == wxID_OK)
		{
			try
			{
				WhidEngine::GetEngine().GetWhidGateway()->Login(dlg.mLogonName,
					dlg.mPasswd, mSession);

				// If no exception was thrown the login was successful

				// Initialize the CTX session variable.
	            
				std::string str_key;
				WarItoa(str_key, mSession.mSessionKey);
				CTX["ses_key"] = str_key;
				CTX["ses_cookie"] = mSession.mCookie;

				WhidEngine::GetEngine().GetWhidGateway()->GetUserFromSession(
					mSession, mUserPrx, CTX);

				WHID::UserInfo ui;
				USER->GetInfo(ui, CTX);
				mIsAdmin = ui.mAdmin;

				WarCollector<wxChar> wx_user = ui.mName;
				mStatusBarCtl->SetStatusText(wx_user.GetValue().c_str(), SB_USER);
				if (IsAdmin())
					mStatusBarCtl->SetStatusText(_("Admin"), SB_ADMIN);
				else
					mStatusBarCtl->SetStatusText(_("User"), SB_ADMIN);

				return true;
			}
			catch(WHID::AccessDeniedException)
			{
				wxMessageBox(_("Access denied. Is the user-name and password correct?"));
			}
			catch(Ice::Exception& ex)
			{
				WAR_ERRLOG;
				err_log << "Login failed. " << ex << war_endl;
			}
		}
	}// for tries

    return false;
}

void MainFrame::ReloadInvoices()
{
    if (IsCrippled())
        return;

    mInvoicesListItems.clear();
    mInvoicesListCtl->DeleteAllItems();

    try
    {
        wxTreeItemId node = mTreeCtl->GetSelection();
        if (node.IsOk())
        {
            TreeNode *pnode = dynamic_cast<TreeNode *>(mTreeCtl->GetItemData(node));
            if (NULL == pnode)
                return; // No data!
            pnode->mNode->GetInvoices(USER, mInvoicesListItems, CTX);
        }
        else
        {
            // Current user
            USER->GetInvoices(USER, mInvoicesListItems, CTX);
        }

        const int num_invoices = (int)mInvoicesListItems.size();
        for(int row = 0; row < num_invoices; row++)
        {
            const WHID::InvoiceInfo& inv = mInvoicesListItems[row];

            WarTime date;
            date.AssignFromAnsiDateTime(inv.mInvoiceDate.c_str());

            WarCollector<wxChar> conv;

            conv = date.FormatLT("%x");

            const int nitem = mInvoicesListCtl->InsertItem(
                row, wxString(conv.GetValue().c_str()));

            mInvoicesListCtl->SetItemData(nitem, row);

            char buf[64];
            sprintf(buf, "%d", (int)inv.mId);
            conv = buf;
            mInvoicesListCtl->SetItem(nitem, ILC_ID, conv.GetValue().c_str());
            
            conv = inv.mUserName;
            mInvoicesListCtl->SetItem(nitem, ILC_USER, conv.GetValue().c_str());

            conv = inv.mCustomerName;
            mInvoicesListCtl->SetItem(nitem, ILC_CUST, conv.GetValue().c_str());

            sprintf(buf, "%.02lf", (double)inv.mHours);
            conv = buf;
            mInvoicesListCtl->SetItem(nitem, ILC_HOURS, conv.GetValue().c_str());

            sprintf(buf, "%.02lf", (double)inv.mCharge);
            conv = buf;
            mInvoicesListCtl->SetItem(nitem, ILC_CHARGE, conv.GetValue().c_str());           

            sprintf(buf, "%.02lf", (double)inv.mProfit);
            conv = buf;
            mInvoicesListCtl->SetItem(nitem, ILC_PROFIT, conv.GetValue().c_str());           
        }
    }
    catch(WHID::AccessDeniedException)
    {
        wxMessageBox(_("Access denied. You don't have permission to see invoices."));
    }
    catch(Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught exception: " << ex << war_endl;
    }
}


void MainFrame::ReloadCurrentList()
{
    if (NULL == mTabCtl)
        return; 

    const TabPagesE tab = (TabPagesE) mTabCtl->GetSelection();
    switch(tab)
    {
    case TAB_WORKLIST:
        ReloadWorklist();
        break;
    case TAB_INVOICES:
        ReloadInvoices();
        break;
    default:
      ; // Compiler food
    }
}


/*!
 * wxEVT_LEFT_DCLICK event handler for ID_INVOICES_LIST
 */

void MainFrame::OnInvoicesListLeftDClick( wxMouseEvent& event )
{
   ExportSelectedInvoicesToHtml();
}

void MainFrame::GetSelectedInvoices(WHID::InvoiceList& invoices)
{
    invoices.clear();
    int nitem = -1;
    while((nitem = mInvoicesListCtl->GetNextItem(nitem, 
        wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) >= 0)
    {
        const int ix = (int)mInvoicesListCtl->GetItemData(nitem);
        const WHID::InvoiceInfo ii = mInvoicesListItems[ix];
        invoices.push_back(ii.mInvoice);
    }
}

void MainFrame::ExportSelectedInvoicesToXml()
{
    war_syspath_t path;
    path << WAR_HOME_APP_DATA_AND_SETTINGS 
        << "whid" DEBUG_CONF_EXT;

    WarPath<wxChar> wx_path = path.GetPath();

    wxFileDialog dlg(this, _("Save to XML file"), wx_path.GetPath(), 
        _("bill.xml"), _("XML files (*.xml)|*.xml"), wxSAVE);
    if (dlg.ShowModal() == wxID_OK)
    {
        try
        {
            path.Reset();
            path << dlg.GetPath();

            WHID::InvoiceList invoices;
            GetSelectedInvoices(invoices);
            ExportToXml(path, invoices);
        }
        catch(WarException& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to export the data: "
                << ex
                << war_endl;
        }
    }
}

void MainFrame::ExportSelectedInvoicesToHtml()
{
    war_syspath_t path;
    path << WAR_HOME_APP_DATA_AND_SETTINGS 
        << "whid" DEBUG_CONF_EXT;


    WarPath<wxChar> wx_path = path.GetPath();
    wxFileDialog dlg(this, _("Save to HTML file"), wx_path.GetPath(), 
        _("bill.html"), _("HTML files (*.html)|*.htm"), wxSAVE);
    if (dlg.ShowModal() == wxID_OK)
    {
        try
        {
            path.Reset();
            path << dlg.GetPath();

            WHID::InvoiceList invoices;
            GetSelectedInvoices(invoices);
            ExportToHtml(path, invoices);

            //ShellExecute(NULL, _T("OPEN"), dlg.GetPath(), NULL, NULL, SW_SHOW);
            BROWSER(dlg.GetPath());

        }
        catch(WarException& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to export the data: "
                << ex
                << war_endl;
        }
    }
}



/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_INVOICES_TO_XML
 */

void MainFrame::OnEXPORTINVOICESTOXMLClick( wxCommandEvent& event )
{
    ExportSelectedInvoicesToXml();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_EXPORT_INVOICES_TO_XML
 */

void MainFrame::OnEXPORTINVOICESTOXMLUpdate( wxUpdateUIEvent& event )
{
    event.Enable((mTabCtl->GetSelection() == TAB_INVOICES)
        && (mInvoicesListCtl->GetSelectedItemCount() > 0));
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_INVOICES_TO_HTML
 */

void MainFrame::OnEXPORTINVOICESTOHTMLClick( wxCommandEvent& event )
{
    ExportSelectedInvoicesToHtml();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_EXPORT_INVOICES_TO_HTML
 */

void MainFrame::OnEXPORTINVOICESTOHTMLUpdate( wxUpdateUIEvent& event )
{
    event.Enable((mTabCtl->GetSelection() == TAB_INVOICES)
        && (mInvoicesListCtl->GetSelectedItemCount() > 0));
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_BACKUP_DATABASE
 */

void MainFrame::OnBACKUPDATABASEClick( wxCommandEvent& event )
{
    war_syspath_t path;
    path << WAR_HOME_APP_DATA_AND_SETTINGS 
        << "whid" DEBUG_CONF_EXT;

    WarPath<wxChar> wx_path = path.GetPath();
    wxFileDialog dlg(this, _("Save to XML file"), wx_path.GetPath(), 
        _("whid_backup.xml"), _("XML files (*.xml)|*.xml"), wxSAVE);
    if (dlg.ShowModal() == wxID_OK)
    {
        try
        {
            path.Reset();
            path << dlg.GetPath();
            wxBusyInfo wait(_("Backing up data...")); 
            WhidEngine::GetEngine().Backup(path);
        }
        catch(WarException& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to export the data: "
                << ex
                << war_endl;
        }
    }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_RESTORE_DATABASE
 */

void MainFrame::OnRESTOREDATABASEClick( wxCommandEvent& event )
{
    war_syspath_t path;
    path << WAR_HOME_APP_DATA_AND_SETTINGS 
        << "whid" DEBUG_CONF_EXT;
    
    WarPath<wxChar> wx_path = path.GetPath();
    wxFileDialog dlg(this, _("Save to XML file"), wx_path.GetPath(), 
        _("whid_backup.xml"), _("XML files (*.xml)|*.xml"), wxOPEN);
    if (dlg.ShowModal() == wxID_OK)
    {
        try
        {
            path.Reset();
            path << dlg.GetPath();
            wxBusyInfo wait(_("Restoring data...")); 
            WhidEngine::GetEngine().Restore(path);
        }
        catch(const WarException& ex)
        {
            WAR_ERRLOG;
            err_log << "Failed to export the data: "
                << ex
                << war_endl;
        }
    }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BACKUP_DATABASE
 */

void MainFrame::OnBACKUPDATABASEUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsCrippled() && IS_ADMIN);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RESTORE_DATABASE
 */

void MainFrame::OnRESTOREDATABASEUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsCrippled() && IS_ADMIN);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON1
 */

void MainFrame::OnTOGGLEBUTTON1Click( wxCommandEvent& event )
{
    ReloadNodes();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_NOTE_FILTER_TEXT
 */

void MainFrame::OnNOTEFILTERTEXTUpdated( wxCommandEvent& event )
{
    ReloadNodes();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_TO_EMAIL
 */

void MainFrame::OnEXPORTTOEMAILClick( wxCommandEvent& event )
{
    try
    {
        WHID::InvoiceList invoices;
        GetSelectedInvoices(invoices);
        EmailInvoices(invoices, this);
    }
    catch(WarException& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to export the data: "
            << ex.what()
            << ex
            << war_endl;
    }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_EXPORT_TO_EMAIL
 */

void MainFrame::OnEXPORTTOEMAILUpdate( wxUpdateUIEvent& event )
{
    event.Enable(mInvoicesListCtl->GetSelectedItemCount() > 0);
}



/*!
 * wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_WORK_LIST
 */

void MainFrame::OnWORKLISTDeselected( wxListEvent& event )
{
    ReloadWorklist(true);
}

