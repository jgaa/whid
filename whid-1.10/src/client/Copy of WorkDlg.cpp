/////////////////////////////////////////////////////////////////////////////
// Name:        WorkDlg.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     18/12/2006 00:57:01
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WorkDlg.h"
#endif

#include "libwhid.h"

#include "WhidApp.h"

#include <WarTime.h>

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
    EVT_BUTTON( wxID_OK, WorkDlg::OnOKClick )

////@end WorkDlg event table entries

END_EVENT_TABLE()

/*!
 * WorkDlg constructors
 */

WorkDlg::WorkDlg( )
{
    Init();
}

WorkDlg::WorkDlg(WHID::WorkPrx prx, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(prx, parent, id, caption, pos, size, style);
}

/*!
 * WorkDlg creator
 */

bool WorkDlg::Create(WHID::WorkPrx prx, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WorkDlg creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WorkDlg creation

    // Get data from proxy

    try
    {
        mWork = prx;
        mWork->GetData(mData);
        mStatus = mWork->GetStatus();
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
    from_time.AssignFromAnsiDateTime(mData.mFromTime.c_str());
    to_time.AssignFromAnsiDateTime(mData.mToTime.c_str());

    wxDateTime wx_from(from_time.GetTime());
    wxDateTime wx_to(to_time.GetTime());

    STR_TO_DLG(mData.mNotes, mNotesCtl);
    RADIO_TO_DLG(mStatus, mStatusCtl, WHID::WorkStatusE);

    mFromDateCtl->SetValue(wx_from);
    mToDateCtl->SetValue(wx_to);

    wxDateTime when = from_time.GetTime();
    mFromTimeCtl->SetValue(WhidApp::GetMinutes(
        (when.GetHour() * 60 * 60) + (when.GetMinute() * 60)));

    when = to_time.GetTime();
    mToTimeCtl->SetValue(WhidApp::GetMinutes(
        (when.GetHour() * 60 * 60) + (when.GetMinute() * 60)));

    mPausedCtl->SetValue(WhidApp::GetMinutes(mData.mMinutesPaused));

    return true;
}

/*!
 * Member initialisation 
 */

void WorkDlg::Init()
{
////@begin WorkDlg member initialisation
    mFromDateCtl = NULL;
    mFromTimeCtl = NULL;
    mToDateCtl = NULL;
    mToTimeCtl = NULL;
    mPausedCtl = NULL;
    mStatusCtl = NULL;
    mNotesCtl = NULL;
////@end WorkDlg member initialisation

    mStatus = WHID::WSPENDINGINV;
    mData.mMinutesPaused = 0;
    mData.mFromTime = WarTime().FormatLT();
    mData.mToTime = WarTime().FormatLT();
}
/*!
 * Control creation for WorkDlg
 */

void WorkDlg::CreateControls()
{    
////@begin WorkDlg content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_WORKDLG")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    mFromDateCtl = XRCCTRL(*this, "ID_FROM_DATE", wxDatePickerCtrl);
    mFromTimeCtl = XRCCTRL(*this, "ID_FROM_TIME", wxTextCtrl);
    mToDateCtl = XRCCTRL(*this, "ID_TO_DATE", wxDatePickerCtrl);
    mToTimeCtl = XRCCTRL(*this, "ID_TO_TIME", wxTextCtrl);
    mPausedCtl = XRCCTRL(*this, "ID_PAUSED_TIME", wxTextCtrl);
    mStatusCtl = XRCCTRL(*this, "ID_STATUS", wxRadioBox);
    mNotesCtl = XRCCTRL(*this, "ID_NOTES", wxTextCtrl);
////@end WorkDlg content construction

    // Create custom windows not generated automatically here.
////@begin WorkDlg content initialisation
////@end WorkDlg content initialisation
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
    // Bitmap retrieval
////@begin WorkDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WorkDlg bitmap retrieval
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
    std::string utf8_when;

    STR_FROM_DLG(utf8_when, mFromTimeCtl);
    int minutes = 0, hours = 0;
    sscanf(utf8_when.c_str(), "%d:%d", &minutes, hours);

    char buf[128];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:00",
        wx_when.GetYear(), wx_when.GetMonth(),
        wx_when.GetDay(),
        minutes, hours);

    mData.mFromTime = buf;

    // To
    wx_when = mToDateCtl->GetValue();
    STR_FROM_DLG(utf8_when, mToTimeCtl);
    minutes = 0, hours = 0;
    sscanf(utf8_when.c_str(), "%d:%d", &minutes, hours);

    sprintf(buf, "%04d-%02d-%02d %02d:%02d:00",
        wx_when.GetYear(), wx_when.GetMonth(),
        wx_when.GetDay(),
        minutes, hours);

    mData.mToTime = buf;

    // Minutes
    STR_FROM_DLG(utf8_when, mPausedCtl);
    minutes = 0, hours = 0;
    sscanf(utf8_when.c_str(), "%d:%d", &minutes, hours);
    mData.mMinutesPaused = (hours * 60) + minutes;

    // Save to proxy
    try
    {
        mWork->SetData(mData);
        if (old_status != mStatus)
            mWork->SetStatus(mStatus);
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to update work: "
            << ex
            << war_endl;
    }
}


