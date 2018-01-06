/////////////////////////////////////////////////////////////////////////////
// Name:        NodeDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     16/12/2006 13:26:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "NodeDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"
#include <ice/WarIceException.h>
#include "MainFrame.h"
#include <wxwidgets/WarWxValidator.h>
#include <wx/statbox.h>
#include <wx/choice.h>

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "NodeRateManager.h"
////@end includes

#include "NodeDlg.h"
#include "RateManager.h"

////@begin XPM images
////@end XPM images

/*!
 * NodeDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( NodeDlg, wxDialog )

/*!
 * NodeDlg event table definition
 */

BEGIN_EVENT_TABLE( NodeDlg, wxDialog )

////@begin NodeDlg event table entries
    EVT_BUTTON( wxID_OK, NodeDlg::OnOKClick )
    EVT_UPDATE_UI( wxID_OK, NodeDlg::OnOKUpdate )

////@end NodeDlg event table entries

END_EVENT_TABLE()

/*!
 * NodeDlg constructors
 */

NodeDlg::NodeDlg( )
{
    Init();
}


/*!
 * NodeDlg creator
 */

bool NodeDlg::Create(WHID::NodePrx node,
                     WHID::WhidControlPrx whidControl,
                     wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mNode = node;
    mWhidControl = whidControl;

    // Must bee initialized
    WAR_ASSERT(mWhidControl);

////@begin NodeDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
////@end NodeDlg creation

    OnCustomizeToLevel();
   
    mRatesCtl->mRates = &mRates;
    mRatesCtl->mWhidControl = mWhidControl;

    try
    {
        WHID::CustomerPrx cust_prx = WHID::CustomerPrx::checkedCast(node);
        if (cust_prx)
            cust_prx->GetCustomerData(mCustomerData, CTX);
        else
        {
            mTabCtl->RemovePage(TP_CUSTDATA);
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught an exception: " << ex << war_endl;
    }
   

    SetData();

    mRatesCtl->mAllowAdd = IS_ADMIN;
    mRatesCtl->ReloadRates();

    return true;
}

/*!
 * Member initialisation 
 */

void NodeDlg::Init()
{
////@begin NodeDlg member initialisation
    mTopLeftImageCtl = NULL;
    mTabCtl = NULL;
    mNameCtl = NULL;
    mActiveStateLabelCtl = NULL;
    mNodeActiveStatusCtl = NULL;
    mNotesCtl = NULL;
    mCustomerDataCtl = NULL;
////@end NodeDlg member initialisation
    
}
/*!
 * Control creation for NodeDlg
 */

void NodeDlg::CreateControls()
{    
////@begin NodeDlg content construction
    NodeDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    mTopLeftImageCtl = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("icons/128x128/customer.png")), wxDefaultPosition, wxSize(128, 128), 0 );
    itemBoxSizer2->Add(mTopLeftImageCtl, 0, wxALIGN_TOP|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTabCtl = new wxNotebook( itemDialog1, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT|wxNB_TOP );

    wxPanel* itemPanel6 = new wxPanel( mTabCtl, ID_NODE_PROPERTIES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel6->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
    itemPanel6->SetSizer(itemFlexGridSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel6, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mNameCtl = new wxTextCtrl( itemPanel6, ID_NAME, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemFlexGridSizer7->Add(mNameCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer7->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mActiveStateLabelCtl = new wxStaticBox(itemPanel6, wxID_ANY, _("Is Customer active?"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(mActiveStateLabelCtl, wxHORIZONTAL);
    itemFlexGridSizer7->Add(itemStaticBoxSizer11, 0, wxALIGN_LEFT|wxGROW|wxALL, 5);
    wxArrayString mNodeActiveStatusCtlStrings;
    mNodeActiveStatusCtlStrings.Add(_("Auto (Enabled if any of it's projects are active)"));
    mNodeActiveStatusCtlStrings.Add(_("Active"));
    mNodeActiveStatusCtlStrings.Add(_("Inactive"));
    mNodeActiveStatusCtl = new wxChoice( itemPanel6, ID_NODE_ACTIVE_STATUS, wxDefaultPosition, wxDefaultSize, mNodeActiveStatusCtlStrings, 0 );
    mNodeActiveStatusCtl->SetStringSelection(_("0"));
    itemStaticBoxSizer11->Add(mNodeActiveStatusCtl, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel6, wxID_STATIC, _("Notes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mNotesCtl = new wxTextCtrl( itemPanel6, ID_NOTES, _T(""), wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE );
    itemFlexGridSizer7->Add(mNotesCtl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTabCtl->AddPage(itemPanel6, _("Properties"));

    mRatesCtl = new NodeRateManager( mTabCtl, NodeRateManager::ID_NODE_RATES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    mRatesCtl->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    mTabCtl->AddPage(mRatesCtl, _("Rates"));

    mCustomerDataCtl = new wxPanel( mTabCtl, ID_CUSTOMER_DATA, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    mCustomerDataCtl->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer17 = new wxFlexGridSizer(2, 2, 0, 0);
    mCustomerDataCtl->SetSizer(itemFlexGridSizer17);

    wxStaticText* itemStaticText18 = new wxStaticText( mCustomerDataCtl, wxID_STATIC, _("Email"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer17->Add(itemStaticText18, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl19 = new wxTextCtrl( mCustomerDataCtl, ID_EMAIL, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemTextCtrl19->SetMaxLength(128);
    itemFlexGridSizer17->Add(itemTextCtrl19, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTabCtl->AddPage(mCustomerDataCtl, _("Customer Data"));

    itemBoxSizer4->Add(mTabCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer20 = new wxStdDialogButtonSizer;

    itemBoxSizer4->Add(itemStdDialogButtonSizer20, 0, wxALIGN_LEFT|wxALL, 5);
    wxButton* itemButton21 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer20->AddButton(itemButton21);

    wxButton* itemButton22 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer20->AddButton(itemButton22);

    itemStdDialogButtonSizer20->Realize();

    // Set validators
    mNameCtl->SetValidator( WarWxValidator (mData.mName) );
    mNodeActiveStatusCtl->SetValidator( wxGenericValidator(& mAutoStatus) );
    mNotesCtl->SetValidator( WarWxValidator (mData.mNotes) );
    itemTextCtrl19->SetValidator( WarWxValidator (mCustomerData.mEmail) );
////@end NodeDlg content construction

    // Create custom windows not generated automatically here.
////@begin NodeDlg content initialisation
////@end NodeDlg content initialisation
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void NodeDlg::OnOKClick( wxCommandEvent& event )
{
    if ( Validate() && TransferDataFromWindow() ) 
    {

        try
        {
            WHID::CustomerPrx cust_prx = WHID::CustomerPrx::checkedCast(mNode);
            if (cust_prx)
                cust_prx->SetCustomerData(mCustomerData, CTX);
        }
        catch(const Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Caught an exception: " << ex << war_endl;
        }

        if (mNode) // save data
        {
            if (mNode->GetNodeType(CTX) == WHID::NODETASK)
                mAutoStatus++;

            mData.mActiveStatus = (WHID::ActiveStatusE)mAutoStatus;
            mNode->SetData(mData, CTX);
            mNode->SetRates(mRates, CTX);
        }

        EndModal(wxID_OK); 
    }
}

//void NodeDlg::SaveData()
//{
//    STR_FROM_DLG(mData.mName, mNameCtl);
//    STR_FROM_DLG(mData.mNotes, mNotesCtl); 
//    RADIO_FROM_DLG(mData.mActiveStatus, mActiveStateCtl, WHID::ActiveStatusE);
//}

void NodeDlg::SetData()
{
    if (mNode)
    {
        mNode->GetData(mData, CTX);
        mNode->GetRates(mRates, CTX);
    }

    //STR_TO_DLG(mData.mName, mNameCtl); 
    //STR_TO_DLG(mData.mNotes, mNotesCtl); 
    //CHOISE_TO_DLG(mData.mActiveStatus, mNodeActiveStatusCtl, WHID::ActiveStatusE);

    mAutoStatus = (int)mData.mActiveStatus;
    if (mAutoStatus && (mNode->GetNodeType(CTX) == WHID::NODETASK))
        --mAutoStatus; 
}


/*!
 * Should we show tooltips?
 */

bool NodeDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap NodeDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval

    return WhidApp::GetBitmapResource( name );


}

/*!
 * Get icon resources
 */

wxIcon NodeDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin NodeDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end NodeDlg icon retrieval
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void NodeDlg::OnOKUpdate( wxUpdateUIEvent& event )
{
    event.Enable(IS_ADMIN);
}

