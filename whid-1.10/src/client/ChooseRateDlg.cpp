/////////////////////////////////////////////////////////////////////////////
// Name:        ChooseRateDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 18:16:20
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ChooseRateDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"

#include <ice/WarIceException.h>
#include "MainFrame.h"

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

////@begin XPM images
////@end XPM images

/*!
 * ChooseRateDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ChooseRateDlg, wxDialog )

/*!
 * ChooseRateDlg event table definition
 */

BEGIN_EVENT_TABLE( ChooseRateDlg, wxDialog )

////@begin ChooseRateDlg event table entries
    EVT_CHECKBOX( ID_HIDE_INACTIVE, ChooseRateDlg::OnHideInactiveClick )

    EVT_BUTTON( ID_ADD_NEW, ChooseRateDlg::OnAddNewClick )
    EVT_UPDATE_UI( ID_ADD_NEW, ChooseRateDlg::OnADDNEWUpdate )

    EVT_BUTTON( wxID_OK, ChooseRateDlg::OnOKClick )

////@end ChooseRateDlg event table entries

  EVT_LIST_ITEM_ACTIVATED(ID_RATELIST , ChooseRateDlg::OnRateListActivated) 

END_EVENT_TABLE()

/*!
 * ChooseRateDlg constructors
 */

ChooseRateDlg::ChooseRateDlg( )
{
    Init();
}

ChooseRateDlg::ChooseRateDlg(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    WAR_ASSERT(false) ; // Dont use this ctor
    Init();
    Create(0, parent, id, caption, pos, size, style);
}

/*!
 * ChooseRateDlg creator
 */

bool ChooseRateDlg::Create(WHID::WhidControlPrx prx, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mWhidControl = prx;

////@begin ChooseRateDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ChooseRateDlg creation

    // Initialize
    mRateListCtl->InsertColumn(HC_NAME, _("Name"), wxLIST_FORMAT_LEFT, 200);
    mRateListCtl->InsertColumn(HC_TYPE, _("Type"), wxLIST_FORMAT_LEFT, 60);
    mRateListCtl->InsertColumn(HC_RATE, _("Rate"), wxLIST_FORMAT_RIGHT, 60);

    ReloadList();

    return true;
}

/*!
 * Member initialisation 
 */

void ChooseRateDlg::Init()
{
////@begin ChooseRateDlg member initialisation
    mRateListCtl = NULL;
    mHideInactiveCtl = NULL;
////@end ChooseRateDlg member initialisation
}
/*!
 * Control creation for ChooseRateDlg
 */

void ChooseRateDlg::CreateControls()
{    
////@begin ChooseRateDlg content construction
    ChooseRateDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Rates"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mRateListCtl = new wxListCtrl( itemDialog1, ID_RATELIST, wxDefaultPosition, wxSize(350, 250), wxLC_REPORT );
    itemStaticBoxSizer3->Add(mRateListCtl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

    mHideInactiveCtl = new wxCheckBox( itemDialog1, ID_HIDE_INACTIVE, _("Hide inactive rates"), wxDefaultPosition, wxDefaultSize, 0 );
    mHideInactiveCtl->SetValue(false);
    itemBoxSizer5->Add(mHideInactiveCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_ADD_NEW, _("Add new"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer8 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer8, 0, wxALIGN_LEFT|wxALL, 5);
    wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer8->AddButton(itemButton9);

    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer8->AddButton(itemButton10);

    itemStdDialogButtonSizer8->Realize();

////@end ChooseRateDlg content construction

    // Create custom windows not generated automatically here.
////@begin ChooseRateDlg content initialisation
////@end ChooseRateDlg content initialisation


}

/*!
 * Should we show tooltips?
 */

bool ChooseRateDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ChooseRateDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ChooseRateDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ChooseRateDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ChooseRateDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void ChooseRateDlg::OnOKClick( wxCommandEvent& event )
{
    const long nitem = mRateListCtl->GetNextItem(-1, 
        wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    mSelectedRate.mName = "";
    mSelectedRate.mRate = 0;

    if (0 <= nitem)
    {
        int index = mRateListCtl->GetItemData(nitem);
        if ((0 <= index) 
            && (mRates.size() > (WHID::RateInfoList::size_type)index))
            mSelectedRate = mRates[index];

    }

    EndModal(wxID_OK);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HIDE_INACTIVE
 */

void ChooseRateDlg::OnHideInactiveClick( wxCommandEvent& event )
{
    ReloadList();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_NEW
 */

void ChooseRateDlg::OnAddNewClick( wxCommandEvent& event )
{
    RateDlg dlg;
    
    dlg.Create(0, mWhidControl, this);
    if (dlg.ShowModal() == wxID_OK)
        ReloadList();
}


void ChooseRateDlg::ReloadList()
{
    mRateListCtl->DeleteAllItems();
    mRates.clear();

    try
    {
        const bool is_checked = mHideInactiveCtl->IsChecked();
        if (mWhidControl)
            mWhidControl->GetRates(is_checked, mRates, CTX);

        WHID::RateInfoList::iterator rate;
        int row = 0;
        for(rate = mRates.begin(); rate != mRates.end(); rate++, row++)
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


void ChooseRateDlg::OnRateListActivated(wxListEvent& event)
{
    int index = event.GetData();
    if (0 <= index)
    {
        try
        {
            RateDlg dlg;

            WHID::RatePrx rate = mRates[index].mRate;

            if (rate)
            {

                dlg.Create(rate, mWhidControl, this);
                if (dlg.ShowModal() == wxID_OK)
                    ReloadList();
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
 * wxEVT_UPDATE_UI event handler for ID_ADD_NEW
 */

void ChooseRateDlg::OnADDNEWUpdate( wxUpdateUIEvent& event )
{
    event.Enable(IS_ADMIN);
}

