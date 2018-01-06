/////////////////////////////////////////////////////////////////////////////
// Name:        BillingWiz.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     01/01/2007 14:44:01
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "BillingWiz.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"
#include <WarUtf8.h>
#include <WarFileWrapper.h>
#include <WarFastBuffer.h>
#include <WarTmpFileName.h>
#include <WarSmtpClient.h>
#include "EmailBillDlg.h"
#include <wx/busyinfo.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#ifdef WIN32
#   include <shellapi.h>
#endif



// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "BillingWiz.h"

////@begin XPM images
////@end XPM images

/*!
 * BillingWiz type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BillingWiz, wxWizard )

/*!
 * BillingWiz event table definition
 */

BEGIN_EVENT_TABLE( BillingWiz, wxWizard )

////@begin BillingWiz event table entries
////@end BillingWiz event table entries

END_EVENT_TABLE()

/*!
 * BillingWiz constructors
 */

BillingWiz::BillingWiz( )
{
    Init();
}

BillingWiz::BillingWiz( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
    Init();
    Create(parent, id, pos);
}

/*!
 * BillingWiz creator
 */

bool BillingWiz::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin BillingWiz creation
    wxBitmap wizardBitmap(GetBitmapResource(wxT("icons/128x128/billing.png")));
    wxWizard::Create( parent, id, _("Billing"), wizardBitmap, pos, wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    CreateControls();
////@end BillingWiz creation
    return true;
}

/*!
 * Member initialisation 
 */

void BillingWiz::Init()
{
////@begin BillingWiz member initialisation
////@end BillingWiz member initialisation

    wxDateTime current = wxDateTime::Now();

    mFromdate = current;
    mFromdate.SetDay(1);
    mFromdate.SetHour(0);
    mFromdate.SetMinute(0);
    mFromdate.SetSecond(0);
    mFromdate.SetMillisecond(0);

    mToDate = mFromdate;
    mToDate.SetToLastMonthDay();
}
/*!
 * Control creation for BillingWiz
 */

void BillingWiz::CreateControls()
{    
////@begin BillingWiz content construction
    wxWizard* itemWizard1 = this;

    BillWizTimeframePage* itemWizardPageSimple2 = new BillWizTimeframePage( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple2);

    BillWizProjectsPage* itemWizardPageSimple9 = new BillWizProjectsPage( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple9);

    BillWizChargesPage* itemWizardPageSimple17 = new BillWizChargesPage( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple17);

    BillWizInvoicingPage* itemWizardPageSimple21 = new BillWizInvoicingPage( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple21);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple9);
    lastPage = itemWizardPageSimple9;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple17);
    lastPage = itemWizardPageSimple17;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple21);
    lastPage = itemWizardPageSimple21;
////@end BillingWiz content construction

    // Create custom windows not generated automatically here.
////@begin BillingWiz content initialisation
////@end BillingWiz content initialisation
}

/*!
 * Runs the wizard.
 */

bool BillingWiz::Run()
{
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node)
    {
        wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
        if (startPage) return RunWizard(startPage);
        node = node->GetNext();
    }
    return false;
}

/*!
 * Should we show tooltips?
 */

bool BillingWiz::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BillingWiz::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin BillingWiz bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("icons/128x128/billing.png"))
    {
        wxBitmap bitmap(_T("icons/128x128/billing.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    return wxNullBitmap;
////@end BillingWiz bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BillingWiz::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BillingWiz icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BillingWiz icon retrieval
}

/*!
 * BillWizProjectsPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BillWizProjectsPage, wxWizardPageSimple )

/*!
 * BillWizProjectsPage event table definition
 */

BEGIN_EVENT_TABLE( BillWizProjectsPage, wxWizardPageSimple )

////@begin BillWizProjectsPage event table entries
    EVT_WIZARD_PAGE_CHANGING( -1, BillWizProjectsPage::OnSelectProjectsPageChanging )

    EVT_BUTTON( ID_SELECT_ALL, BillWizProjectsPage::OnSelectAllClick )

    EVT_BUTTON( ID_UNSELECT_ALL, BillWizProjectsPage::OnUnselectAllClick )

    EVT_BUTTON( ID_TOGGLE, BillWizProjectsPage::OnToggleClick )

////@end BillWizProjectsPage event table entries

END_EVENT_TABLE()

/*!
 * BillWizProjectsPage constructors
 */

BillWizProjectsPage::BillWizProjectsPage( )
{
    Init();
}

BillWizProjectsPage::BillWizProjectsPage( wxWizard* parent )
{
    Init();
    Create( parent );
}

/*!
 * BillWizProjectsPage creator
 */

bool BillWizProjectsPage::Create( wxWizard* parent )
{
////@begin BillWizProjectsPage creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end BillWizProjectsPage creation
    return true;
}

/*!
 * Member initialisation 
 */

void BillWizProjectsPage::Init()
{
////@begin BillWizProjectsPage member initialisation
    mProjectsToBillCtl = NULL;
////@end BillWizProjectsPage member initialisation
}
/*!
 * Control creation for BillWizProjectsPage
 */

void BillWizProjectsPage::CreateControls()
{    
////@begin BillWizProjectsPage content construction
    BillWizProjectsPage* itemWizardPageSimple9 = this;

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple9->SetSizer(itemBoxSizer10);

    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemWizardPageSimple9, wxID_ANY, _("Projects with work that can be billed"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
    itemBoxSizer10->Add(itemStaticBoxSizer11, 0, wxGROW|wxALL, 5);

    wxArrayString mProjectsToBillCtlStrings;
    mProjectsToBillCtl = new wxCheckListBox( itemWizardPageSimple9, ID_PROJECTS_TO_BILL, wxDefaultPosition, wxSize(-1, 330), mProjectsToBillCtlStrings, wxLB_SINGLE );
    itemStaticBoxSizer11->Add(mProjectsToBillCtl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer13, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton14 = new wxButton( itemWizardPageSimple9, ID_SELECT_ALL, _("Select All"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton15 = new wxButton( itemWizardPageSimple9, ID_UNSELECT_ALL, _("Unselect All"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemWizardPageSimple9, ID_TOGGLE, _("Toggle"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemButton16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end BillWizProjectsPage content construction

    // Create custom windows not generated automatically here.
////@begin BillWizProjectsPage content initialisation
////@end BillWizProjectsPage content initialisation
}

/*!
 * Should we show tooltips?
 */

bool BillWizProjectsPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BillWizProjectsPage::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin BillWizProjectsPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BillWizProjectsPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BillWizProjectsPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BillWizProjectsPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BillWizProjectsPage icon retrieval
}


bool BillWizProjectsPage::TransferDataFromWindow()
{
    return true;
}

bool BillWizProjectsPage::TransferDataToWindow()
{  
    if (!mProjectsToBillCtl)
        CreateControls();   

    wxBusyInfo wait(_("Fetching billable projects...")); 

    try
    {
        // Populate the list
        mProjectsToBillCtl->Clear();
        mProjects.clear();

        BillingWiz *wiz = dynamic_cast<BillingWiz *>(GetParent());
        WAR_ASSERT(NULL != wiz);
        WarTime from_date = wiz->mFromdate.GetTicks();
        from_date.ToLT();
        WarTime to_date = wiz->mToDate.GetTicks();
        to_date.ToLT();

        USER->GetProjectsForBilling(
            from_date.FormatLT("%Y-%m-%d 00:00:00"),
            to_date.FormatLT("%Y-%m-%d 23:59:59"),
            mProjects, CTX);

        int row = 0;
        WHID::ProjectForBillingInfoList::iterator prj;
        for(prj = mProjects.begin()
            ; prj != mProjects.end()
            ; prj++, row++)
        {
            std::stringstream name;
            name << prj->mCompanyName << "/" << prj->mProjectName
                << " "
                << WhidApp::GetMinutesAsString(prj->mHoursToBill);

            WarUtf8 utf8 = name.str();            
            mProjectsToBillCtl->Append(utf8.GetUnicode().c_str());
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to fetch data from the database: "
            << ex
            << war_endl;

        return false;
    }

    return true;
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_ALL
 */

void BillWizProjectsPage::OnSelectAllClick( wxCommandEvent& event )
{
    const int num = mProjectsToBillCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        mProjectsToBillCtl->Check(i, true);
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_UNSELECT_ALL
 */

void BillWizProjectsPage::OnUnselectAllClick( wxCommandEvent& event )
{
    const int num = mProjectsToBillCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        mProjectsToBillCtl->Check(i, false);
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TOGGLE
 */

void BillWizProjectsPage::OnToggleClick( wxCommandEvent& event )
{
    const int num = mProjectsToBillCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        mProjectsToBillCtl->Check(i, 
            !mProjectsToBillCtl->IsChecked(i));
    }
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGING event handler for ID_SELECT_PROJECTS
 */

void BillWizProjectsPage::OnSelectProjectsPageChanging( wxWizardEvent& event )
{
    if (event.GetDirection())
    {
        // recalculate based on the selections

        wxBusyInfo wait(_("Calculating...")); 

        USER->ClearAllChargesNotInvoiced(CTX);

        BillingWiz *wiz = dynamic_cast<BillingWiz *>(GetParent());
        WAR_ASSERT(NULL != wiz);
        WarTime from_date = wiz->mFromdate.GetTicks();
        from_date.ToLT();
        WarTime to_date = wiz->mToDate.GetTicks();
        to_date.ToLT();

        const std::string my_from_date = from_date.FormatLT("%Y-%m-%d 00:00:00");
        const std::string my_to_date = to_date.FormatLT("%Y-%m-%d 23:59:59");


        const int num = mProjectsToBillCtl->GetCount();
        for(int i = 0; i < num; i++)
        {
            //mProjects[i].mProject->ClearPending();
            if (mProjectsToBillCtl->IsChecked(i))
            {
                mProjects[i].mProject->RecalculatePending(
                    USER, my_from_date, my_to_date, CTX);
            }
        }
    }
}


/*!
 * BillWizChargesPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BillWizChargesPage, wxWizardPageSimple )

/*!
 * BillWizChargesPage event table definition
 */

BEGIN_EVENT_TABLE( BillWizChargesPage, wxWizardPageSimple )

////@begin BillWizChargesPage event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, BillWizChargesPage::OnDoCalculatePageChanged )

    EVT_TREE_SEL_CHANGED( ID_CHARGES_TREE, BillWizChargesPage::OnChargesTreeSelChanged )

////@end BillWizChargesPage event table entries

END_EVENT_TABLE()

/*!
 * BillWizChargesPage constructors
 */

BillWizChargesPage::BillWizChargesPage( )
{
    Init();
}

BillWizChargesPage::BillWizChargesPage( wxWizard* parent )
{
    Init();
    Create( parent );
}

/*!
 * BillWizChargesPage creator
 */

bool BillWizChargesPage::Create( wxWizard* parent )
{
////@begin BillWizChargesPage creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end BillWizChargesPage creation
    return true;
}

/*!
 * Member initialisation 
 */

void BillWizChargesPage::Init()
{
////@begin BillWizChargesPage member initialisation
    mChargesTreeCtl = NULL;
    mChargesListCtl = NULL;
////@end BillWizChargesPage member initialisation
}
/*!
 * Control creation for BillWizChargesPage
 */

void BillWizChargesPage::CreateControls()
{    
////@begin BillWizChargesPage content construction
    BillWizChargesPage* itemWizardPageSimple17 = this;

    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(itemWizardPageSimple17, wxID_ANY, _("Calculated and aligned charges"));
    wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxVERTICAL);
    itemWizardPageSimple17->SetSizer(itemStaticBoxSizer18);

    mChargesTreeCtl = new wxTreeCtrl( itemWizardPageSimple17, ID_CHARGES_TREE, wxDefaultPosition, wxSize(500, 120), wxTR_HAS_BUTTONS |wxTR_TWIST_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_SINGLE );
    itemStaticBoxSizer18->Add(mChargesTreeCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mChargesListCtl = new wxListCtrl( itemWizardPageSimple17, ID_CHARGES_LIST, wxDefaultPosition, wxSize(100, 250), wxLC_REPORT );
    itemStaticBoxSizer18->Add(mChargesListCtl, 0, wxGROW|wxALL, 5);

////@end BillWizChargesPage content construction

    // Create custom windows not generated automatically here.
////@begin BillWizChargesPage content initialisation
////@end BillWizChargesPage content initialisation

    mChargesListCtl->InsertColumn(LC_TASK, _("Task"),  wxLIST_FORMAT_LEFT, 110);
    mChargesListCtl->InsertColumn(LC_FROM, _("From"),  wxLIST_FORMAT_LEFT, 100);
    mChargesListCtl->InsertColumn(LC_TO, _("To"),  wxLIST_FORMAT_LEFT, 40);
    mChargesListCtl->InsertColumn(LC_HOURS, _("Hours"),  wxLIST_FORMAT_RIGHT, 45);
    mChargesListCtl->InsertColumn(LC_RATE, _("Rate"),  wxLIST_FORMAT_LEFT, 60);
    mChargesListCtl->InsertColumn(LC_RATE_PR_HOUR, _("r/hr"),  wxLIST_FORMAT_RIGHT, 50);
    mChargesListCtl->InsertColumn(LC_CHARGE, _("Charge"),  wxLIST_FORMAT_RIGHT, 60);
    mChargesListCtl->InsertColumn(LC_DISCOUNT, _("Discount"),  wxLIST_FORMAT_RIGHT, 50);
}

/*!
 * Should we show tooltips?
 */

bool BillWizChargesPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BillWizChargesPage::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin BillWizChargesPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BillWizChargesPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BillWizChargesPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BillWizChargesPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BillWizChargesPage icon retrieval
}


bool BillWizChargesPage::TransferDataToWindow()
{
    if (!mChargesTreeCtl)
        CreateControls();

    wxBusyInfo wait(_("Updating data...")); 

    try
    {
        mChargesTreeCtl->DeleteAllItems();
        mChargesListCtl->DeleteAllItems();

        wxTreeItemId root = mChargesTreeCtl->AddRoot(_("Companys"));
        mChargesTreeCtl->SetItemHasChildren(root);
        
        WHID::ProjectWithChargeForBillingInfoList list;
        USER->GetProjectsReadyForInvoicing(list, CTX);

        node_map_t cust_map;

        int row = 0;
        WHID::ProjectWithChargeForBillingInfoList::iterator prj;
        for(prj = list.begin()
            ; prj != list.end()
            ; prj++, row++)
        {
            // Resolve customer
            node_map_t::iterator cust_node = cust_map.find(prj->mCompanyId);
            if (cust_node == cust_map.end())
            {
                std::stringstream tmp_name;
                tmp_name.precision(2);
                tmp_name << prj->mCompanyName; // << " (" << prj->mCharge << ")";
                WarUtf8 name = tmp_name.str();
                wxTreeItemId new_id = mChargesTreeCtl->AppendItem(root, 
                    name.GetUnicode().c_str(), -1, -1, 
                    new CustTreeItemData(prj->mCustomer));

                mChargesTreeCtl->SetItemHasChildren(new_id);

                cust_node = cust_map.insert(node_map_t::value_type(
                    prj->mCompanyId, new_id)).first;
            }

            // Add project
            std::stringstream tmp_name;
                tmp_name.precision(2);
                tmp_name.setf(std::ios_base::fixed, std::ios_base::floatfield);
                tmp_name << prj->mProjectName << " (" << prj->mCharge << ")";

            WarUtf8 name = tmp_name.str();
            mChargesTreeCtl->AppendItem(cust_node->second, 
                name.GetUnicode().c_str(), -1, -1, 
                new ProjTreeItemData(prj->mProject));
        }

        mChargesTreeCtl->Expand(root);
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to fetch data from the database: "
            << ex
            << war_endl;

        return false;
    }

    return true;
}

bool BillWizChargesPage::TransferDataFromWindow()
{
    return true;
}

/*!
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_CHARGES_TREE
 */

void BillWizChargesPage::OnChargesTreeSelChanged( wxTreeEvent& event )
{
    // Update the list
    mChargesListCtl->DeleteAllItems();

    WHID::ChargeDataList list;

    wxTreeItemId id = event.GetItem();
    if (id)
    {
        wxTreeItemData *data = mChargesTreeCtl->GetItemData(id);
        if (data)
        {
            CustTreeItemData *cust = dynamic_cast<CustTreeItemData *>(data);
            if (cust)
            {
                cust->mCustomer->GetCharges(WHID::WFPENDINGINV, USER, list, CTX);
            }
            else
            {
                ProjTreeItemData *prj = dynamic_cast<ProjTreeItemData *>(data);
                if (prj)
                {
                    prj->mProject->GetCharges(WHID::WFPENDINGINV, USER, list, CTX);
                }
            }

        }
    }

    double total_charge = 0, total_hours = 0, total_discount = 0;
    WHID::ChargeDataList::iterator charge;
    long row = 0;
    for(charge = list.begin(); charge != list.end(); charge++)
    {
        WarTime from_time, to_time;
       from_time.AssignFromAnsiDateTime(charge->mWorkFrom.c_str());
        to_time.AssignFromAnsiDateTime(charge->mWorkTo.c_str());

        WarUtf8 name = charge->mTaskName, rate = charge->mRateName,
            from = from_time.FormatLT("%Y-%m-%d %H:%M"),
            to = to_time.FormatLT("%H:%M");

        const int nitem = mChargesListCtl->InsertItem(row++, 
            name.GetUnicode().c_str());
        mChargesListCtl->SetItem(nitem, LC_FROM, from.GetUnicode().c_str());
        mChargesListCtl->SetItem(nitem, LC_TO, to.GetUnicode().c_str());
        mChargesListCtl->SetItem(nitem, LC_HOURS, wxString::Format(_("%1.2lf"),
            charge->mNumHours));
        mChargesListCtl->SetItem(nitem, LC_RATE, rate.GetUnicode().c_str());
        mChargesListCtl->SetItem(nitem, LC_RATE_PR_HOUR, wxString::Format(_("%1.2lf"),
            charge->mChargePrHour));
        mChargesListCtl->SetItem(nitem, LC_CHARGE, wxString::Format(_("%1.2lf"),
            charge->mCharge));
        mChargesListCtl->SetItem(nitem, LC_DISCOUNT, wxString::Format(_("%1.2lf"),
            charge->mDiscount));

        if (!charge->mRate)
        {
            mChargesListCtl->SetItemBackgroundColour(nitem, 
                wxTheColourDatabase->Find(wxT("pink")));
        }

        total_charge += charge->mCharge;
        total_hours += charge->mNumHours;
        total_discount += charge->mDiscount;
    }

    if (!list.empty())
    {
        // Add summary
        const int nitem = mChargesListCtl->InsertItem(row++, _("Total"));

        mChargesListCtl->SetItem(nitem, LC_HOURS, wxString::Format(_("%1.2lf"),
            total_hours));

         mChargesListCtl->SetItem(nitem, LC_CHARGE, wxString::Format(_("%1.2lf"),
            total_charge));

        mChargesListCtl->SetItem(nitem, LC_DISCOUNT, wxString::Format(_("%1.2lf"),
            total_discount));

        mChargesListCtl->SetItemBackgroundColour(nitem, 
            wxTheColourDatabase->Find(wxT("yellow")));
    }
}





/*!
 * BillWizInvoicingPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BillWizInvoicingPage, wxWizardPageSimple )

/*!
 * BillWizInvoicingPage event table definition
 */

BEGIN_EVENT_TABLE( BillWizInvoicingPage, wxWizardPageSimple )

////@begin BillWizInvoicingPage event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, BillWizInvoicingPage::OnInvoicingPageChanged )
    EVT_WIZARD_PAGE_CHANGING( -1, BillWizInvoicingPage::OnInvoicingPageChanging )

    EVT_BUTTON( ID_SELECT_ALl_INVOICES, BillWizInvoicingPage::OnSELECTALlINVOICESClick )

    EVT_BUTTON( ID_UNSELECT_ALL_INVOICES, BillWizInvoicingPage::OnUnselectAllInvoicesClick )

    EVT_BUTTON( ID_TOGGLE_ALL_INVOICES, BillWizInvoicingPage::OnToggleAllInvoicesClick )

    EVT_BUTTON( ID_EXPORT_TO_HTML, BillWizInvoicingPage::OnExportToHtmlClick )

    EVT_BUTTON( ID_EXPORT_TO_XML, BillWizInvoicingPage::OnExportToXmlClick )

    EVT_BUTTON( ID_SEND_ON_EMAIL, BillWizInvoicingPage::OnSendOnEmailClick )

////@end BillWizInvoicingPage event table entries

END_EVENT_TABLE()

/*!
 * BillWizInvoicingPage constructors
 */

BillWizInvoicingPage::BillWizInvoicingPage( )
{
    Init();
}

BillWizInvoicingPage::BillWizInvoicingPage( wxWizard* parent )
{
    Init();
    Create( parent );
}

/*!
 * BillWizInvoicingPage creator
 */

bool BillWizInvoicingPage::Create( wxWizard* parent )
{
////@begin BillWizInvoicingPage creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end BillWizInvoicingPage creation
    return true;
}

/*!
 * Member initialisation 
 */

void BillWizInvoicingPage::Init()
{
////@begin BillWizInvoicingPage member initialisation
    mInvoicesListCtl = NULL;
////@end BillWizInvoicingPage member initialisation
}
/*!
 * Control creation for BillWizInvoicingPage
 */

void BillWizInvoicingPage::CreateControls()
{    
////@begin BillWizInvoicingPage content construction
    BillWizInvoicingPage* itemWizardPageSimple21 = this;

    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple21->SetSizer(itemBoxSizer22);

    wxStaticBox* itemStaticBoxSizer23Static = new wxStaticBox(itemWizardPageSimple21, wxID_ANY, _("Invoices"));
    wxStaticBoxSizer* itemStaticBoxSizer23 = new wxStaticBoxSizer(itemStaticBoxSizer23Static, wxVERTICAL);
    itemBoxSizer22->Add(itemStaticBoxSizer23, 0, wxGROW|wxALL, 0);

    wxArrayString mInvoicesListCtlStrings;
    mInvoicesListCtl = new wxCheckListBox( itemWizardPageSimple21, ID_INVOICES, wxDefaultPosition, wxSize(-1, 300), mInvoicesListCtlStrings, wxLB_SINGLE );
    itemStaticBoxSizer23->Add(mInvoicesListCtl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer23->Add(itemBoxSizer25, 0, wxALIGN_RIGHT|wxALL, 0);

    wxButton* itemButton26 = new wxButton( itemWizardPageSimple21, ID_SELECT_ALl_INVOICES, _("Select all"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer25->Add(itemButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton27 = new wxButton( itemWizardPageSimple21, ID_UNSELECT_ALL_INVOICES, _("Unselect all"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer25->Add(itemButton27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton28 = new wxButton( itemWizardPageSimple21, ID_TOGGLE_ALL_INVOICES, _("Toggle"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer25->Add(itemButton28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer22->Add(itemBoxSizer29, 0, wxALIGN_RIGHT|wxALL, 0);

    wxButton* itemButton30 = new wxButton( itemWizardPageSimple21, ID_EXPORT_TO_HTML, _("Export to html"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer29->Add(itemButton30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton31 = new wxButton( itemWizardPageSimple21, ID_EXPORT_TO_XML, _("Export to xml"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer29->Add(itemButton31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton32 = new wxButton( itemWizardPageSimple21, ID_SEND_ON_EMAIL, _("Send on email"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer29->Add(itemButton32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end BillWizInvoicingPage content construction

    // Create custom windows not generated automatically here.
////@begin BillWizInvoicingPage content initialisation
////@end BillWizInvoicingPage content initialisation
}

/*!
 * Should we show tooltips?
 */

bool BillWizInvoicingPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BillWizInvoicingPage::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin BillWizInvoicingPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BillWizInvoicingPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BillWizInvoicingPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BillWizInvoicingPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BillWizInvoicingPage icon retrieval
}






bool BillWizInvoicingPage::TransferDataToWindow()
{  
    if (!mInvoicesListCtl)
        CreateControls();   

    wxBusyInfo wait(_("Updating...")); 

    try
    {
        mInvoicesListCtl->Clear();

        // Populate the list
        WHID::InvoiceInfoList::iterator inv;
        for(inv = mInvoices.begin(); inv != mInvoices.end(); inv++)
        {
            std::stringstream name;
            name << "#" << inv->mId << " " << inv->mCustomerName
                << " (" << inv->mCharge << ")";
            WarUtf8 conv = name.str();
            mInvoicesListCtl->Append(conv.GetUnicode());
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to fetch data from the database: "
            << ex
            << war_endl;

        return false;
    }

    return true;
}

/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_DO_CALCULATE
 */

void BillWizChargesPage::OnDoCalculatePageChanged( wxWizardEvent& event )
{
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_INVOICING
 */

void BillWizInvoicingPage::OnInvoicingPageChanged( wxWizardEvent& event )
{
    mInvoices.clear();

    if (event.GetDirection())
    {
        // Make invoices
        wxBusyInfo wait(_("Creating invoices...")); 
        USER->MakeInvoices(mInvoices, CTX);
        TransferDataToWindow();
    }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_ALl_INVOICES
 */

void BillWizInvoicingPage::OnSELECTALlINVOICESClick( wxCommandEvent& event )
{
    const int num = mInvoicesListCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        mInvoicesListCtl->Check(i, true);
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_UNSELECT_ALL_INVOICES
 */

void BillWizInvoicingPage::OnUnselectAllInvoicesClick( wxCommandEvent& event )
{
    const int num = mInvoicesListCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        mInvoicesListCtl->Check(i, false);
    }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TOGGLE_ALL_INVOICES
 */

void BillWizInvoicingPage::OnToggleAllInvoicesClick( wxCommandEvent& event )
{
    const int num = mInvoicesListCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        mInvoicesListCtl->Check(i, 
            !mInvoicesListCtl->IsChecked(i));
    }
}

void BillWizInvoicingPage::GetSelectedInvoices(WHID::InvoiceList& invoices)
{
    invoices.clear();
    const int num = mInvoicesListCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        if (mInvoicesListCtl->IsChecked(i))
        {
            invoices.push_back(mInvoices[i].mInvoice);
        }
    }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPORT_TO_HTML
 */

void BillWizInvoicingPage::OnExportToHtmlClick( wxCommandEvent& event )
{
    WarPath<wchar_t> path;
    path << WAR_HOME_APP_DATA_AND_SETTINGS 
        << "whid" DEBUG_CONF_EXT;

    wxFileDialog dlg(this, _("Save to XML file"), path.GetPath(), 
        wxT("bill.html"), _("HTML files (*.html)|*.html"), wxSAVE);

    if (dlg.ShowModal() == wxID_OK)
    {
        WHID::InvoiceList invoices;
        GetSelectedInvoices(invoices);

        war_syspath_t use_path = dlg.GetPath().GetData();
        ExportToHtml(use_path, invoices);

        //ShellExecute(NULL, _T("OPEN"), dlg.GetPath().GetData(), NULL, NULL, SW_SHOW);
        BROWSER(dlg.GetPath());
    }
}




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPORT_TO_XML
 */

void BillWizInvoicingPage::OnExportToXmlClick( wxCommandEvent& event )
{
    WarPath<wchar_t> path;
    path << WAR_HOME_APP_DATA_AND_SETTINGS 
        << "whid" DEBUG_CONF_EXT;

    wxFileDialog dlg(this, _("Save to XML file"), path.GetPath(), 
        _("bill.xml"), _("XML files (*.xml)|*.xml"), wxSAVE);
    if (dlg.ShowModal() == wxID_OK)
    {
      war_syspath_t use_path = dlg.GetPath().GetData();

        try
        {
            WHID::InvoiceList invoices;
            GetSelectedInvoices(invoices);
            ExportToXml(use_path, invoices);
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
* wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SEND_ON_EMAIL
*/

void BillWizInvoicingPage::OnSendOnEmailClick( wxCommandEvent& event )
{
    // Get the list of invoices
    WHID::InvoiceList invoices;
    const int num = mInvoicesListCtl->GetCount();
    for(int i = 0; i < num; i++)
    {
        if (mInvoicesListCtl->IsChecked(i))
        {
            invoices.push_back(mInvoices[i].mInvoice);
        }
    }

    EmailInvoices(invoices, this);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGING event handler for ID_INVOICING
 */

void BillWizInvoicingPage::OnInvoicingPageChanging( wxWizardEvent& event )
{
    if (event.GetDirection())
    {
        // Commit selected invoices

        const int num = mInvoicesListCtl->GetCount();
        for(int i = 0; i < num; i++)
        {
            if (mInvoicesListCtl->IsChecked(i))
            {
                mInvoices[i].mInvoice->Commit(CTX);
            }
        }
    }
}

/*!
 * BillWizTimeframePage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BillWizTimeframePage, wxWizardPageSimple )

/*!
 * BillWizTimeframePage event table definition
 */

BEGIN_EVENT_TABLE( BillWizTimeframePage, wxWizardPageSimple )

////@begin BillWizTimeframePage event table entries
////@end BillWizTimeframePage event table entries

END_EVENT_TABLE()

/*!
 * BillWizTimeframePage constructors
 */

BillWizTimeframePage::BillWizTimeframePage( )
{
    Init();
}

BillWizTimeframePage::BillWizTimeframePage( wxWizard* parent )
{
    Init();
    Create( parent );
}

/*!
 * BillWizTimeframePage creator
 */

bool BillWizTimeframePage::Create( wxWizard* parent )
{
////@begin BillWizTimeframePage creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end BillWizTimeframePage creation
    return true;
}

/*!
 * Member initialisation 
 */

void BillWizTimeframePage::Init()
{
////@begin BillWizTimeframePage member initialisation
    mFromDateCtl = NULL;
    mToDateCtl = NULL;
////@end BillWizTimeframePage member initialisation
}
/*!
 * Control creation for BillWizTimeframePage
 */

void BillWizTimeframePage::CreateControls()
{    
////@begin BillWizTimeframePage content construction
    BillWizTimeframePage* itemWizardPageSimple2 = this;

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemWizardPageSimple2->SetSizer(itemBoxSizer3);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(1, 4, 0, 0);
    itemBoxSizer3->Add(itemFlexGridSizer4, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("From Date"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFromDateCtl = new wxCalendarCtrl( itemWizardPageSimple2, ID_FROM_DATE_CALENDAR, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxCAL_SUNDAY_FIRST );
    itemFlexGridSizer4->Add(mFromDateCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("To date"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mToDateCtl = new wxCalendarCtrl( itemWizardPageSimple2, ID_TO_DATE_CALENDAR, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxCAL_SUNDAY_FIRST|wxSUNKEN_BORDER );
    itemFlexGridSizer4->Add(mToDateCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end BillWizTimeframePage content construction

    // Create custom windows not generated automatically here.
////@begin BillWizTimeframePage content initialisation
////@end BillWizTimeframePage content initialisation
}

/*!
 * Should we show tooltips?
 */

bool BillWizTimeframePage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BillWizTimeframePage::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
////@begin BillWizTimeframePage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BillWizTimeframePage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BillWizTimeframePage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BillWizTimeframePage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BillWizTimeframePage icon retrieval
}


bool BillWizTimeframePage::TransferDataFromWindow()
{
    BillingWiz *wiz = dynamic_cast<BillingWiz *>(GetParent());
    if (!wiz)
        return false;

    wiz->mFromdate = mFromDateCtl->GetDate();
    wiz->mToDate = mToDateCtl->GetDate();

    if (wiz->mFromdate > wiz->mToDate)
        return false;

    return true;
}

bool BillWizTimeframePage::TransferDataToWindow()
{
    if (!mFromDateCtl)
        CreateControls();

    BillingWiz *wiz = dynamic_cast<BillingWiz *>(GetParent());
    if (!wiz)
        return false;

    mFromDateCtl->SetDate(wiz->mFromdate);
    mToDateCtl->SetDate(wiz->mToDate);

    return true;
}




