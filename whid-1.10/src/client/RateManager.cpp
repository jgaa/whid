/////////////////////////////////////////////////////////////////////////////
// Name:        RateManager.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 21:04:07
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "RateManager.h"
#endif


#include "libwhid.h"
#include "WhidApp.h"

#include <ice/WarIceException.h>


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

#include "ChooseRateDlg.h"
#include "RateDlg.h"
#include "RateManager.h"

////@begin XPM images
////@end XPM images

/*!
 * RateManager type definition
 */

IMPLEMENT_DYNAMIC_CLASS( RateManager, wxPanel )

/*!
 * RateManager event table definition
 */

BEGIN_EVENT_TABLE( RateManager, wxPanel )

////@begin RateManager event table entries
    EVT_BUTTON( ID_ADD_RATES, RateManager::OnAddRatesClick )
    EVT_UPDATE_UI( ID_ADD_RATES, RateManager::OnADDRATESUpdate )

    EVT_BUTTON( ID_REMOVE_RATE, RateManager::OnRemoveRateClick )
    EVT_UPDATE_UI( ID_REMOVE_RATE, RateManager::OnRemoveRateUpdate )

////@end RateManager event table entries

  EVT_LIST_ITEM_ACTIVATED(ID_RATESLIST , RateManager::OnRateListActivated) 

END_EVENT_TABLE()

/*!
 * RateManager constructors
 */

RateManager::RateManager( )
{
    Init();
}

bool RateManager::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    return RateManager::Create(parent, id, _("Rate Manager"), pos, size, style);
}


/*!
 * RateManager creator
 */

bool RateManager::Create( /*WHID::RateInfoList& rates,
                         WHID::WhidControlPrx whidControl,*/
                         wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    //mRates = rates;
    //mWhidControl = whidControl;

////@begin RateManager creation
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end RateManager creation

    mRateListCtl->InsertColumn(HC_NAME, _("Name"), wxLIST_FORMAT_LEFT, 200);
    mRateListCtl->InsertColumn(HC_TYPE, _("Type"), wxLIST_FORMAT_LEFT, 60);
    mRateListCtl->InsertColumn(HC_RATE, _("Rate"), wxLIST_FORMAT_RIGHT, 60);

    //ReloadRates();


    return true;
}

/*!
 * Member initialisation 
 */

void RateManager::Init()
{
////@begin RateManager member initialisation
    mRateListCtl = NULL;
    mDeleteButtonCtl = NULL;
////@end RateManager member initialisation
    mRates = NULL;
    mAllowAdd = false;
}
/*!
 * Control creation for RateManager
 */

void RateManager::CreateControls()
{    
////@begin RateManager content construction
    RateManager* itemPanel1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemPanel1, wxID_ANY, _("Selected Rates"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    mRateListCtl = new wxListCtrl( itemPanel1, ID_RATESLIST, wxDefaultPosition, wxSize(300, 200), wxLC_REPORT );
    itemBoxSizer4->Add(mRateListCtl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer6, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemPanel1, ID_ADD_RATES, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mDeleteButtonCtl = new wxButton( itemPanel1, ID_REMOVE_RATE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(mDeleteButtonCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end RateManager content construction

    // Create custom windows not generated automatically here.
////@begin RateManager content initialisation
////@end RateManager content initialisation
}

/*!
 * Should we show tooltips?
 */

bool RateManager::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap RateManager::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon RateManager::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin RateManager icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end RateManager icon retrieval
}

void RateManager::ReloadRates()
{
    if (!mRates)
        return;

    mRateListCtl->DeleteAllItems();

    try
    {

        WHID::RateInfoList::iterator rate;
        int row = 0;
        for(rate = mRates->begin(); rate != mRates->end(); rate++, row++)
        {
            WarUtf8 name = rate->mName;

            int icon = 0;

            wxString wx_charge, wx_type_name;
            wx_charge = wxString::Format(_("%.0f"), rate->mChargePerHour);

            switch(rate->mChargeType)
            {
            case WHID::RCTABSOLUTE:
                wx_type_name = _("Normal");
                break;
            case WHID::RTCADDVALUE:
                wx_type_name = _("Add");
                break;
            case WHID::RTCADDPERCENT:
                wx_type_name = _("Percent");
                break;
            case WHID::RTCEXTRAFEE:
                wx_type_name = _("Fee");
                break;
            }

            const int nitem = mRateListCtl->InsertItem(
                row, wxString(name.GetUnicode().c_str()), 
                icon);
            mRateListCtl->SetItemData(nitem, row);

            mRateListCtl->SetItem(nitem, HC_TYPE, wx_type_name);
            mRateListCtl->SetItem(nitem, HC_RATE, wx_charge);
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to list rates: "
            << ex
            << war_endl;
    }

}


void RateManager::OnRateListActivated(wxListEvent& event)
{
    int index = event.GetData();
    if (0 <= index)
    {
        try
        {
            RateDlg dlg;

            WHID::RatePrx rate = mRates->operator [](index).mRate;

            if (rate)
            {
                dlg.Create(rate, mWhidControl, this);
                if (dlg.ShowModal() == wxID_OK)
                    ReloadRates();
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_RATES
 */

void RateManager::OnAddRatesClick( wxCommandEvent& event )
{
    ChooseRateDlg dlg;
    dlg.Create(mWhidControl, this);
    if (dlg.ShowModal() == wxID_OK)
    {
        if (dlg.mSelectedRate.mRate)
        {
            mRates->push_back(dlg.mSelectedRate);
            ReloadRates();
        }
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REMOVE_RATE
 */

void RateManager::OnRemoveRateClick( wxCommandEvent& event )
{
    const long nitem = mRateListCtl->GetNextItem(-1, 
        wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (0 <= nitem)
    {
        int index = mRateListCtl->GetItemData(nitem);
        if ((0 <= index) 
            && (mRates->size() > (WHID::RateInfoList::size_type)index))
        {
            
            WHID::RateInfoList::iterator P;
            int row = 0;
            for(P = mRates->begin(); P != mRates->end(); P++, row++)
            {
                if (row == index)
                {
                    mRates->erase(P);
                    break;
                }
            }
            ReloadRates();
        }
    }
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_REMOVE_RATE
 */

void RateManager::OnRemoveRateUpdate( wxUpdateUIEvent& event )
{
    event.Enable(mAllowAdd
        && (mRateListCtl->GetNextItem(-1, 
        wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED) >= 0));
}



/*!
 * wxEVT_UPDATE_UI event handler for ID_ADD_RATES
 */

void RateManager::OnADDRATESUpdate( wxUpdateUIEvent& event )
{
    event.Enable(mAllowAdd);
}

