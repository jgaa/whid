/////////////////////////////////////////////////////////////////////////////
// Name:        RateDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 16:39:42
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "RateDlg.h"
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

////@begin includes
////@end includes

#include "RateDlg.h"
#include <ice/WarIceException.h>
#include "WhidApp.h"


////@begin XPM images
////@end XPM images

/*!
 * RateDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( RateDlg, wxDialog )

/*!
 * RateDlg event table definition
 */

BEGIN_EVENT_TABLE( RateDlg, wxDialog )

////@begin RateDlg event table entries
    EVT_CHECKBOX( ID_FROM_TO_HRS_ENABLED, RateDlg::OnFromToHrsEnabledClick )

    EVT_BUTTON( wxID_OK, RateDlg::OnOKClick )
    EVT_UPDATE_UI( wxID_OK, RateDlg::OnOKUpdate )

////@end RateDlg event table entries

    

END_EVENT_TABLE()

/*!
 * RateDlg constructors
 */

RateDlg::RateDlg( )
{
    Init();
}


/*!
 * RateDlg creator
 */

bool RateDlg::Create(WHID::RatePrx prx,
        WHID::WhidControlPrx ctl,
        wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mPrx = prx;
    mWhidControl = ctl;

////@begin RateDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end RateDlg creation


    // Load data from object
    LoadData();

    // Assign data to controls
    STR_TO_DLG(mData.mName, mNameCtl);
    STR_TO_DLG(mData.mNotes, mNotesCtl);
    DOUBLE_TO_DLG(mData.mChargePerHour, mChargeCtl);
    DOUBLE_TO_DLG(mData.mCost, mCostCtl);
    RADIO_TO_DLG(mData.mChargeType, mTypeCtl, WHID::RateChargeTypeE);
    SPIN_TO_DLG(mData.mPriority, mPriorityCtl);
    SPIN_TO_DLG(mData.mNumContinousHours , mNumContinousHoursCtl);
    SPIN_TO_DLG(mData.mMaxTimeBetweenContinousIntervals, mMaxTimeBetweenContinousIntervalsCtl);
    SPIN_TO_DLG(mData.mMinTime, mMinTimeCtl);
    SPIN_TO_DLG(mData.mAlign, mAlignCtl);
    SPIN_TO_DLG(mData.mAlignStart, mAlignStartCtl);
    SPIN_TO_DLG(mData.mAlignThreshold, mAlignThresholdCtl);
    BOOL_TO_DLG(mData.mActive, mEnabledCtl);

    mEnableContinousTimeCtl->SetValue(mData.mNumContinousHours > 0);
   
    if ((mData.mFromClock >= 0) && (mData.mToClock >= 0))
    {
        mFromClockCtl->SetValue(WhidApp::GetMinutes(mData.mFromClock));
        mToClockCtl->SetValue(WhidApp::GetMinutes(mData.mToClock));
        mFromToHoursEnabledCtl->SetValue(true);
    }
    else
    {
        INT_TO_DLG(-1, mFromClockCtl);
        INT_TO_DLG(-1, mToClockCtl);
        mFromToHoursEnabledCtl->SetValue(false);
    }

    // Day of week
#define CHECK(ref, tag) mDaysOfWeekCtl->Check(\
    ref, strstr(mData.mAppliesToDayMask.c_str(), #tag) != NULL)

    CHECK(DIW_ALL, a);
    CHECK(DIW_MOM, m);
    CHECK(DIW_TUE, t);
    CHECK(DIW_WED, w);
    CHECK(DIW_THUE, T);
    CHECK(DIW_FRI, f);
    CHECK(DIW_SAT, s);
    CHECK(DIW_SUN, S);
    CHECK(DIW_PUBH, p);
    CHECK(DIW_SPUBH, X);
#undef CHECK

    

    return true;
}

/*!
 * Member initialisation 
 */

void RateDlg::Init()
{
////@begin RateDlg member initialisation
    mNameCtl = NULL;
    mPriorityCtl = NULL;
    mTypeCtl = NULL;
    mChargeCtl = NULL;
    mCostCtl = NULL;
    mEnabledCtl = NULL;
    mFromToHoursEnabledCtl = NULL;
    mFromClockCtl = NULL;
    mToClockCtl = NULL;
    mEnableContinousTimeCtl = NULL;
    mNumContinousHoursCtl = NULL;
    mMaxTimeBetweenContinousIntervalsCtl = NULL;
    mDaysOfWeekCtl = NULL;
    mMinTimeCtl = NULL;
    mAlignCtl = NULL;
    mAlignThresholdCtl = NULL;
    mAlignStartCtl = NULL;
    mNotesCtl = NULL;
////@end RateDlg member initialisation

    // Set default values for new rates
    mData.mChargePerHour = 0;
    mData.mChargeType = WHID::RCTABSOLUTE;
    mData.mPriority = 10;
    mData.mFromClock = -1;
    mData.mToClock = -1;
    mData.mNumContinousHours = 0;
    mData.mMaxTimeBetweenContinousIntervals = 0;
    mData.mAppliesToDayMask = "a";
    mData.mMinTime = 30;
    mData.mAlign = 15;
    mData.mAlignThreshold = 5;
    mData.mFree = false;
    mData.mActive = true;

}
/*!
 * Control creation for RateDlg
 */

void RateDlg::CreateControls()
{    
////@begin RateDlg content construction
    RateDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBitmap* itemStaticBitmap3 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("icons/128x128/rule.png")), wxDefaultPosition, wxSize(128, 128), 0 );
    itemBoxSizer2->Add(itemStaticBitmap3, 0, wxALIGN_TOP|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxNotebook* itemNotebook5 = new wxNotebook( itemDialog1, ID_RATETAB, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT|wxNB_TOP );

    wxPanel* itemPanel6 = new wxPanel( itemNotebook5, ID_PROPERTIES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
    itemPanel6->SetSizer(itemFlexGridSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel6, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mNameCtl = new wxTextCtrl( itemPanel6, ID_NAMECTL, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer7->Add(mNameCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel6, wxID_STATIC, _("Priority"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPriorityCtl = new wxSpinCtrl( itemPanel6, ID_PRIORIRY_CTL, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer7->Add(mPriorityCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel6, wxID_STATIC, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText12, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mTypeCtlStrings;
    mTypeCtlStrings.Add(_("Normal charge"));
    mTypeCtlStrings.Add(_("Add amount to normal charge"));
    mTypeCtlStrings.Add(_("Add percentage to normal charge"));
    mTypeCtlStrings.Add(_("Extra fee"));
    mTypeCtl = new wxChoice( itemPanel6, ID_TYPE_CTL, wxDefaultPosition, wxDefaultSize, mTypeCtlStrings, 0 );
    itemFlexGridSizer7->Add(mTypeCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel6, wxID_STATIC, _("Amount (per hour)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mChargeCtl = new wxTextCtrl( itemPanel6, ID_CHARGE_CTL, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(mChargeCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel6, wxID_STATIC, _("Cost (our expense)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mCostCtl = new wxTextCtrl( itemPanel6, ID_RATE_COST, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(mCostCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer7->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mEnabledCtl = new wxCheckBox( itemPanel6, ID_ENABLED_CTL, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
    mEnabledCtl->SetValue(false);
    itemFlexGridSizer7->Add(mEnabledCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook5->AddPage(itemPanel6, _("Properties"));

    wxPanel* itemPanel20 = new wxPanel( itemNotebook5, ID_APPLIES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel20->SetSizer(itemBoxSizer21);

    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer21->Add(itemBoxSizer22, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer23Static = new wxStaticBox(itemPanel20, wxID_ANY, _("From/To hours"));
    wxStaticBoxSizer* itemStaticBoxSizer23 = new wxStaticBoxSizer(itemStaticBoxSizer23Static, wxHORIZONTAL);
    itemBoxSizer22->Add(itemStaticBoxSizer23, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer24 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer23->Add(itemFlexGridSizer24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mFromToHoursEnabledCtl = new wxCheckBox( itemPanel20, ID_FROM_TO_HRS_ENABLED, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
    mFromToHoursEnabledCtl->SetValue(false);
    itemFlexGridSizer24->Add(mFromToHoursEnabledCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer24->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel20, wxID_STATIC, _("From hour"), wxDefaultPosition, wxDefaultSize, 0 );
    if (RateDlg::ShowToolTips())
        itemStaticText27->SetToolTip(_("Like hour:minute or -1 if the rukle don't apply"));
    itemFlexGridSizer24->Add(itemStaticText27, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFromClockCtl = new wxTextCtrl( itemPanel20, ID_FROM_HOUR, _T(""), wxDefaultPosition, wxSize(60, -1), 0 );
    itemFlexGridSizer24->Add(mFromClockCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel20, wxID_STATIC, _("To Hour"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer24->Add(itemStaticText29, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mToClockCtl = new wxTextCtrl( itemPanel20, ID_TO_HOUR, _T(""), wxDefaultPosition, wxSize(60, -1), 0 );
    itemFlexGridSizer24->Add(mToClockCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer31Static = new wxStaticBox(itemPanel20, wxID_ANY, _("Continous time"));
    wxStaticBoxSizer* itemStaticBoxSizer31 = new wxStaticBoxSizer(itemStaticBoxSizer31Static, wxHORIZONTAL);
    itemBoxSizer22->Add(itemStaticBoxSizer31, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer32 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer31->Add(itemFlexGridSizer32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mEnableContinousTimeCtl = new wxCheckBox( itemPanel20, ID_ENABLE_CONTINOUS_TIME, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
    mEnableContinousTimeCtl->SetValue(false);
    itemFlexGridSizer32->Add(mEnableContinousTimeCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer32->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel20, wxID_STATIC, _("From # hours"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer32->Add(itemStaticText35, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mNumContinousHoursCtl = new wxSpinCtrl( itemPanel20, ID_SPINCTRL_FROM_HOURS, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer32->Add(mNumContinousHoursCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText37 = new wxStaticText( itemPanel20, wxID_STATIC, _("Max delays (minutes)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer32->Add(itemStaticText37, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMaxTimeBetweenContinousIntervalsCtl = new wxSpinCtrl( itemPanel20, ID_SPINCTRL_MAX_DELAYS, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer32->Add(mMaxTimeBetweenContinousIntervalsCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer39Static = new wxStaticBox(itemPanel20, wxID_ANY, _("Days of week"));
    wxStaticBoxSizer* itemStaticBoxSizer39 = new wxStaticBoxSizer(itemStaticBoxSizer39Static, wxVERTICAL);
    itemBoxSizer21->Add(itemStaticBoxSizer39, 0, wxALIGN_TOP|wxALL, 5);
    wxArrayString mDaysOfWeekCtlStrings;
    mDaysOfWeekCtlStrings.Add(_("All days"));
    mDaysOfWeekCtlStrings.Add(_("Monday"));
    mDaysOfWeekCtlStrings.Add(_("Tuesday"));
    mDaysOfWeekCtlStrings.Add(_("Wednsday"));
    mDaysOfWeekCtlStrings.Add(_("Thurstay"));
    mDaysOfWeekCtlStrings.Add(_("Friday"));
    mDaysOfWeekCtlStrings.Add(_("Saturday"));
    mDaysOfWeekCtlStrings.Add(_("Sunday"));
    //mDaysOfWeekCtlStrings.Add(_("Public holidays"));
    //mDaysOfWeekCtlStrings.Add(_("Special publig holidays"));
    mDaysOfWeekCtl = new wxCheckListBox( itemPanel20, ID_DAYS_OF_WEEK_CTL, wxDefaultPosition, wxDefaultSize, mDaysOfWeekCtlStrings, wxLB_SINGLE );
    itemStaticBoxSizer39->Add(mDaysOfWeekCtl, 0, wxALIGN_LEFT|wxALL, 5);

    itemNotebook5->AddPage(itemPanel20, _("Applies"));

    wxPanel* itemPanel41 = new wxPanel( itemNotebook5, ID_ADJUSTMENTS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxVERTICAL);
    itemPanel41->SetSizer(itemBoxSizer42);

    wxStaticBox* itemStaticBoxSizer43Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Startfee"));
    wxStaticBoxSizer* itemStaticBoxSizer43 = new wxStaticBoxSizer(itemStaticBoxSizer43Static, wxVERTICAL);
    itemBoxSizer42->Add(itemStaticBoxSizer43, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer44 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer43->Add(itemFlexGridSizer44, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText45 = new wxStaticText( itemPanel41, wxID_STATIC, _("Minimum minutes to charge"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer44->Add(itemStaticText45, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMinTimeCtl = new wxSpinCtrl( itemPanel41, ID_SPINCTRL_MIN_TIME, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer44->Add(mMinTimeCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Alignment"));
    wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxHORIZONTAL);
    itemBoxSizer42->Add(itemStaticBoxSizer47, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer48 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer47->Add(itemFlexGridSizer48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText49 = new wxStaticText( itemPanel41, wxID_STATIC, _("Align to nearest # minutes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer48->Add(itemStaticText49, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mAlignCtl = new wxSpinCtrl( itemPanel41, ID_SPINCTRL_ALIGN_NEAREST, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer48->Add(mAlignCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText51 = new wxStaticText( itemPanel41, wxID_STATIC, _("Threshold # minutes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer48->Add(itemStaticText51, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mAlignThresholdCtl = new wxSpinCtrl( itemPanel41, ID_SPINCTRL_TREASHOLD, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer48->Add(mAlignThresholdCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText53 = new wxStaticText( itemPanel41, wxID_STATIC, _("Align to offset # minutes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer48->Add(itemStaticText53, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mAlignStartCtl = new wxSpinCtrl( itemPanel41, ID_SPINCTRL_OFFSET, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer48->Add(mAlignStartCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook5->AddPage(itemPanel41, _("Adjustments"));

    mNotesCtl = new wxTextCtrl( itemNotebook5, ID_NOTES, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );

    itemNotebook5->AddPage(mNotesCtl, _("Notes"));

    itemBoxSizer4->Add(itemNotebook5, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer56 = new wxStdDialogButtonSizer;

    itemBoxSizer4->Add(itemStdDialogButtonSizer56, 0, wxALIGN_LEFT|wxALL, 5);
    wxButton* itemButton57 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer56->AddButton(itemButton57);

    wxButton* itemButton58 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer56->AddButton(itemButton58);

    itemStdDialogButtonSizer56->Realize();

////@end RateDlg content construction

    // Create custom windows not generated automatically here.
////@begin RateDlg content initialisation
////@end RateDlg content initialisation
}

/*!
 * Should we show tooltips?
 */

bool RateDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap RateDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon RateDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin RateDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end RateDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FROM_TO_HRS_ENABLED
 */

void RateDlg::OnFromToHrsEnabledClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FROM_TO_HRS_ENABLED in RateDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FROM_TO_HRS_ENABLED in RateDlg. 
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ENABLE_CONTINOUS_TIME
 */



void RateDlg::LoadData()
{
    try
    {
        if (!mPrx)
            return;

        mPrx->GetData(mData, CTX);
        
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught exception : "
            << ex
            << war_endl;
    }
}


void RateDlg::SaveData()
{
    try
    {
        if (!mPrx)
        {
            // Create a new one!
            mPrx = mWhidControl->AddRate(mData, CTX);
        }
        else
        {
            mPrx->SetData(mData, CTX);
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught exception : "
            << ex
            << war_endl;
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void RateDlg::OnOKClick( wxCommandEvent& event )
{
    // Get the values from te controls

    STR_FROM_DLG(mData.mName, mNameCtl);
    STR_FROM_DLG(mData.mNotes, mNotesCtl);
    DOUBLE_FROM_DLG(mData.mChargePerHour, mChargeCtl);
    DOUBLE_FROM_DLG(mData.mCost, mCostCtl)
    RADIO_FROM_DLG(mData.mChargeType, mTypeCtl, WHID::RateChargeTypeE);
    SPIN_FROM_DLG(mData.mPriority, mPriorityCtl);
    SPIN_FROM_DLG(mData.mNumContinousHours , mNumContinousHoursCtl);
    SPIN_FROM_DLG(mData.mMaxTimeBetweenContinousIntervals, mMaxTimeBetweenContinousIntervalsCtl);
    SPIN_FROM_DLG(mData.mMinTime, mMinTimeCtl);
    SPIN_FROM_DLG(mData.mAlign, mAlignCtl);
    SPIN_FROM_DLG(mData.mAlignThreshold, mAlignThresholdCtl);
    BOOL_FROM_DLG(mData.mActive, mEnabledCtl);

    if (!mEnableContinousTimeCtl->IsChecked())
        mData.mNumContinousHours = 0;
    
    if (!mFromToHoursEnabledCtl->IsChecked())
    {
        mData.mFromClock = mData.mToClock = -1;
    }
    else
    {
        std::string from_time, to_time;
        STR_FROM_DLG(from_time, mFromClockCtl);
        STR_FROM_DLG(to_time, mToClockCtl);

        mData.mFromClock = WhidApp::GetMinutes(from_time);
        mData.mToClock = WhidApp::GetMinutes(to_time);
    }
   
    mData.mAppliesToDayMask = "";

    // Day of week
#define CHECK(ref, tag) if (mDaysOfWeekCtl->IsChecked(ref)) { \
    mData.mAppliesToDayMask += #tag;}
   
    CHECK(DIW_ALL, a);
    CHECK(DIW_MOM, m);
    CHECK(DIW_TUE, t);
    CHECK(DIW_WED, w);
    CHECK(DIW_THUE, T);
    CHECK(DIW_FRI, f);
    CHECK(DIW_SAT, s);
    CHECK(DIW_SUN, S);
    CHECK(DIW_PUBH, p);
    CHECK(DIW_SPUBH, X);
#undef CHECK


    SaveData();

    EndModal(wxID_OK);
}



/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void RateDlg::OnOKUpdate( wxUpdateUIEvent& event )
{
    event.Enable(IS_ADMIN);
}

