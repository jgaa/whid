/////////////////////////////////////////////////////////////////////////////
// Name:        WorkDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 03:53:59
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WorkDlg.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "WorkRateManager.h"
////@end includes

#include "WorkDlg.h"


////@begin XPM images
////@end XPM images

/*!
 * WorkDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WorkDlg, wxDialog )

/*!
 * WorkDlg event table definition
 */

BEGIN_EVENT_TABLE( WorkDlg, wxDialog )

////@begin WorkDlg event table entries
    EVT_DATE_CHANGED( ID_FROM_DATE, WorkDlg::OnFromDateDateChanged )

    EVT_BUTTON( wxID_OK, WorkDlg::OnOKClick )
    EVT_UPDATE_UI( wxID_OK, WorkDlg::OnOKUpdate )

////@end WorkDlg event table entries

END_EVENT_TABLE()

/*!
 * WorkDlg constructors
 */

WorkDlg::WorkDlg( )
{
    Init();
}


/*!
 * WorkDlg creator
 */

bool WorkDlg::Create(WHID::WorkPrx prx, WHID::WhidControlPrx whidControl, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mWhidControl = whidControl;

////@begin WorkDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WorkDlg creation

    try
    {
        mWork = prx;
        mWork->GetData(mData, CTX);
        mWork->GetRates(mRates, CTX);
        mStatus = mWork->GetStatus(CTX);

        mRatesCtl->mRates = &mRates;
        mRatesCtl->mWhidControl = mWhidControl;
        mRatesCtl->ReloadRates();
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to fetch created work: "
            << ex
            << war_endl;

        return false;
    }

    // Set data in controls

    WarTime from_time, to_time;
    from_time.AssignFromAnsiDateTime(mData.mFromTime.c_str(), false, true);
    to_time.AssignFromAnsiDateTime(mData.mToTime.c_str(), false, true);

    wxDateTime wx_from(from_time.GetTime());
    wxDateTime wx_to(to_time.GetTime());

    if (mTrackLastSetDate && mLastSetDate.IsValid())
    {
        wx_to = wx_from = mLastSetDate;
    }
    
    // Go around a bug in wxwidgets 2.8.0
    wx_from.SetHour(0);
    wx_from.SetMinute(0);
    wx_from.SetSecond(0);
    wx_from.SetMillisecond(0);

    wx_to.SetHour(0);
    wx_to.SetMinute(0);
    wx_to.SetSecond(0);
    wx_to.SetMillisecond(0);

    STR_TO_DLG(mData.mNotes, mNotesCtl);
    RADIO_TO_DLG(mStatus, mStatusCtl, WHID::WorkStatusE);

    mFromDateCtl->SetValue(wx_from);
    mToDateCtl->SetValue(wx_to);

    ////from_time.ToUTC();
    //wxDateTime when = from_time.GetTime();
    //mFromTimeCtl->SetValue(WhidApp::GetMinutes(
    //    (when.GetHour() * 60) + when.GetMinute()));

    ////to_time.ToUTC();
    //when = to_time.GetTime();
    //mToTimeCtl->SetValue(WhidApp::GetMinutes(
    //    (when.GetHour() * 60) + when.GetMinute()));

    WarUtf8 tmp_utf8;
    tmp_utf8 = from_time.FormatLT("%H:%M");
    STR_TO_DLG(tmp_utf8, mFromTimeCtl);

    tmp_utf8 = to_time.FormatLT("%H:%M");
    STR_TO_DLG(tmp_utf8, mToTimeCtl);

    mPausedCtl->SetValue(WhidApp::GetMinutes(mData.mMinutesPaused));


    mRatesCtl->mAllowAdd = true;
    if (mWork && (mWork->GetStatus(CTX) == WHID::WSINVOICED))
        mRatesCtl->mAllowAdd = false;
    mRatesCtl->mRates = &mRates;
    mRatesCtl->mWhidControl = mWhidControl;
    mRatesCtl->ReloadRates();

    return true;
}

/*!
 * Member initialisation 
 */

void WorkDlg::Init()
{
////@begin WorkDlg member initialisation
    mTopLeftImageCtl = NULL;
    mFromDateCtl = NULL;
    mFromTimeCtl = NULL;
    mToDateCtl = NULL;
    mToTimeCtl = NULL;
    mPausedCtl = NULL;
    mStatusCtl = NULL;
    mNotesCtl = NULL;
    mOkButtonCtl = NULL;
////@end WorkDlg member initialisation

    mStatus = WHID::WSPENDINGINV;
    mData.mMinutesPaused = 0;
    mData.mFromTime = WarTime().FormatLT();
    mData.mToTime = WarTime().FormatLT();
    mTrackLastSetDate = 0;
}
/*!
 * Control creation for WorkDlg
 */

void WorkDlg::CreateControls()
{    
////@begin WorkDlg content construction
    WorkDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    mTopLeftImageCtl = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("icons/128x128/work.png")), wxDefaultPosition, wxSize(128, 128), 0 );
    itemBoxSizer2->Add(mTopLeftImageCtl, 0, wxALIGN_TOP|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxNotebook* itemNotebook5 = new wxNotebook( itemDialog1, ID_WORKTAB, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT|wxNB_TOP );

    wxPanel* itemPanel6 = new wxPanel( itemNotebook5, ID_WORK_PROPERTIES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemPanel6->SetSizer(itemBoxSizer7);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer7->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer9Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Time spent"));
    wxStaticBoxSizer* itemStaticBoxSizer9 = new wxStaticBoxSizer(itemStaticBoxSizer9Static, wxVERTICAL);
    itemBoxSizer8->Add(itemStaticBoxSizer9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer10 = new wxFlexGridSizer(2, 3, 0, 0);
    itemStaticBoxSizer9->Add(itemFlexGridSizer10, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText11 = new wxStaticText( itemPanel6, wxID_STATIC, _("From Time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer10->Add(itemStaticText11, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFromDateCtl = new wxDatePickerCtrl( itemPanel6, ID_FROM_DATE, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN|wxDP_DEFAULT );
    itemFlexGridSizer10->Add(mFromDateCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFromTimeCtl = new wxTextCtrl( itemPanel6, ID_FROM_TIME, _T(""), wxDefaultPosition, wxSize(40, -1), 0 );
    if (WorkDlg::ShowToolTips())
        mFromTimeCtl->SetToolTip(_("Time as hours[:minutes]"));
    itemFlexGridSizer10->Add(mFromTimeCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel6, wxID_STATIC, _("To Time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer10->Add(itemStaticText14, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mToDateCtl = new wxDatePickerCtrl( itemPanel6, ID_TO_DATE, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN|wxDP_DEFAULT );
    itemFlexGridSizer10->Add(mToDateCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mToTimeCtl = new wxTextCtrl( itemPanel6, ID_TO_TIME, _T(""), wxDefaultPosition, wxSize(40, -1), 0 );
    if (WorkDlg::ShowToolTips())
        mToTimeCtl->SetToolTip(_("Time as hours[:minutes]"));
    itemFlexGridSizer10->Add(mToTimeCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel6, wxID_STATIC, _("Paused/credited time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer10->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPausedCtl = new wxTextCtrl( itemPanel6, ID_PAUSED_TIME, _T(""), wxDefaultPosition, wxSize(40, -1), 0 );
    if (WorkDlg::ShowToolTips())
        mPausedCtl->SetToolTip(_("Time as hours[:minutes]"));
    itemFlexGridSizer10->Add(mPausedCtl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer10->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer8->Add(itemBoxSizer20, 0, wxALIGN_TOP|wxALL, 1);
    wxArrayString mStatusCtlStrings;
    mStatusCtlStrings.Add(_("&Open"));
    mStatusCtlStrings.Add(_("&Done"));
    mStatusCtlStrings.Add(_("&Hold back"));
    mStatusCtlStrings.Add(_("&Free"));
    mStatusCtlStrings.Add(_("&Invoice is sent"));
    mStatusCtl = new wxRadioBox( itemPanel6, ID_STATUS, _("Status"), wxDefaultPosition, wxDefaultSize, mStatusCtlStrings, 1, wxRA_SPECIFY_COLS );
    mStatusCtl->SetSelection(0);
    itemBoxSizer20->Add(mStatusCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Description/Notes"));
    wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxVERTICAL);
    itemBoxSizer7->Add(itemStaticBoxSizer22, 0, wxGROW|wxALL, 5);
    mNotesCtl = new wxTextCtrl( itemPanel6, ID_WORK_NOTES, _T(""), wxDefaultPosition, wxSize(-1, 150), wxTE_MULTILINE );
    itemStaticBoxSizer22->Add(mNotesCtl, 0, wxGROW|wxALL, 5);

    itemNotebook5->AddPage(itemPanel6, _("Work properties"));

    mRatesCtl = new WorkRateManager( itemNotebook5, WorkRateManager::ID_RATES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemNotebook5->AddPage(mRatesCtl, _("Rates"));

    itemBoxSizer4->Add(itemNotebook5, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer25 = new wxStdDialogButtonSizer;

    itemBoxSizer4->Add(itemStdDialogButtonSizer25, 0, wxALIGN_LEFT|wxALL, 5);
    mOkButtonCtl = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer25->AddButton(mOkButtonCtl);

    wxButton* itemButton27 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer25->AddButton(itemButton27);

    itemStdDialogButtonSizer25->Realize();

    // Connect events and objects
    itemPanel6->Connect(ID_WORK_PROPERTIES, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(WorkDlg::OnInitDialog), NULL, this);
////@end WorkDlg content construction
////@begin WorkDlg content initialisation
    // Initialise itemRateTab25 above (to NULL if providing an XRC handler).

////@end WorkDlg content initialisation

    if (NULL == mImage)
    {
        mImage = WhidApp::LoadImageFromResource(wxT("icons/128x128/work.png"));
    }
    if (mImage)
        mTopLeftImageCtl->SetBitmap(*mImage);

    

}

wxBitmap *WorkDlg::mImage = NULL;
wxDateTime WorkDlg::mLastSetDate;

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void WorkDlg::OnOKClick( wxCommandEvent& event )
{
// Get data from the controls
    const WHID::WorkStatusE old_status = mStatus;
    STR_FROM_DLG(mData.mNotes, mNotesCtl);
    RADIO_FROM_DLG(mStatus, mStatusCtl, WHID::WorkStatusE);

    // From
    wxDateTime wx_when = mFromDateCtl->GetValue();

    if (mTrackLastSetDate)
        mLastSetDate = wx_when;

    std::string utf8_when;

    STR_FROM_DLG(utf8_when, mFromTimeCtl);
    int minutes = 0, hours = 0;
    sscanf(utf8_when.c_str(), "%d:%d", &hours, &minutes);

    char buf[128];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:00",
        wx_when.GetYear(), wx_when.GetMonth() + 1,
        wx_when.GetDay(),
        hours, minutes);

    mData.mFromTime = buf;

    // To
    wx_when = mToDateCtl->GetValue();
    STR_FROM_DLG(utf8_when, mToTimeCtl);
    minutes = 0, hours = 0;
    sscanf(utf8_when.c_str(), "%d:%d", &hours, &minutes);

    sprintf(buf, "%04d-%02d-%02d %02d:%02d:00",
        wx_when.GetYear(), wx_when.GetMonth() + 1,
        wx_when.GetDay(),
        hours, minutes);

    mData.mToTime = buf;

    // Minutes
    STR_FROM_DLG(utf8_when, mPausedCtl);
    minutes = 0, hours = 0;
    sscanf(utf8_when.c_str(), "%d:%d", &hours, &minutes);
    mData.mMinutesPaused = (hours * 60) + minutes;

    // Save to proxy
    try
    {
        mWork->SetData(mData, CTX);
        mWork->SetRates(mRates, CTX);
        if (old_status != mStatus)
            mWork->SetStatus(mStatus, CTX);

        event.Skip();
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to update work: "
            << ex
            << war_endl;
    }

    EndModal(wxID_OK);
}

/*!
 * Should we show tooltips?
 */

bool WorkDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WorkDlg::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WorkDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WorkDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WorkDlg icon retrieval
}

/*!
 * wxEVT_DATE_CHANGED event handler for ID_FROM_DATE
 */

void WorkDlg::OnFromDateDateChanged( wxDateEvent& event )
{
    wxDateTime when = event.GetDate();
    wxDateTime to_time = mToDateCtl->GetValue();
    if (when != to_time)
        mToDateCtl->SetValue(when);

    mFromTimeCtl->SetFocus();
    mFromTimeCtl->SetSelection(-1,-1);
    event.Skip();
}


/*!
 * wxEVT_INIT_DIALOG event handler for ID_WORK_PROPERTIES
 */

void WorkDlg::OnInitDialog( wxInitDialogEvent& event )
{
    mFromDateCtl->SetFocus();
    event.Skip();
    try
    {
        if (mWork && (mWork->GetStatus(CTX) == WHID::WSINVOICED))
        {
            mFromDateCtl->Enable(false);
            mFromTimeCtl->Enable(false);
            mToDateCtl->Enable(false);
            mToTimeCtl->Enable(false);
            mPausedCtl->Enable(false);
            mStatusCtl->Enable(false);
            mNotesCtl->SetEditable(false);
            mRatesCtl->Enable(false);
            mRatesCtl->mAllowAdd = false;
            mOkButtonCtl->Enable(false);
        }
    }
    catch(const Ice::Exception)
    {
        ;
    }

}



/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void WorkDlg::OnOKUpdate( wxUpdateUIEvent& event )
{
    event.Enable(true);
}

