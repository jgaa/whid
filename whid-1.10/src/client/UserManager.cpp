/////////////////////////////////////////////////////////////////////////////
// Name:        UserManager.cpp
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     06/07/2007 08:49:25
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "UserManager.h"
#endif

#include "libwhid.h"
#include "WhidApp.h"
#include <WarUtf8.h>
#include "UserDlg.h"


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

#include "UserManager.h"

////@begin XPM images
////@end XPM images


/*!
 * UserManager type definition
 */

IMPLEMENT_DYNAMIC_CLASS( UserManager, wxDialog )


/*!
 * UserManager event table definition
 */

BEGIN_EVENT_TABLE( UserManager, wxDialog )

////@begin UserManager event table entries
    EVT_INIT_DIALOG( UserManager::OnInitDialog )

    EVT_LIST_ITEM_SELECTED( ID_USER_LIST, UserManager::OnUSERLISTSelected )

    EVT_BUTTON( ID_ADD_USER, UserManager::OnADDUSERClick )
    EVT_UPDATE_UI( ID_ADD_USER, UserManager::OnADDUSERUpdate )

    EVT_BUTTON( ID_DELETE_USER, UserManager::OnDELETEUSERClick )
    EVT_UPDATE_UI( ID_DELETE_USER, UserManager::OnDELETEUSERUpdate )

    EVT_BUTTON( ID_EDIT_USER, UserManager::OnEditUserClick )
    EVT_UPDATE_UI( ID_EDIT_USER, UserManager::OnEditUserUpdate )

////@end UserManager event table entries

END_EVENT_TABLE()


/*!
 * UserManager constructors
 */

UserManager::UserManager()
{
    Init();
}

UserManager::UserManager( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    WAR_ASSERT(false);
    Init();
    //Create(parent, id, caption, pos, size, style);
}


/*!
 * UserManager creator
 */

bool UserManager::Create(WHID::WhidControlPrx prx, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mWhidControl = prx;

////@begin UserManager creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end UserManager creation

    mUserListCtl->InsertColumn(HC_NAME, _("Name"), wxLIST_FORMAT_LEFT, 200);
    mUserListCtl->InsertColumn(HC_LOGIN_NAME, _("LoginName"), wxLIST_FORMAT_LEFT, 80);
    mUserListCtl->InsertColumn(HC_ACTIVE, _("Active"), wxLIST_FORMAT_CENTER, 45);
    mUserListCtl->InsertColumn(HC_ADMIN, _("Admin"), wxLIST_FORMAT_CENTER, 45);

    return true;
}


/*!
 * UserManager destructor
 */

UserManager::~UserManager()
{
////@begin UserManager destruction
////@end UserManager destruction
}


/*!
 * Member initialisation
 */

void UserManager::Init()
{
////@begin UserManager member initialisation
    mUserListCtl = NULL;
////@end UserManager member initialisation
}


/*!
 * Control creation for UserManager
 */

void UserManager::CreateControls()
{    
////@begin UserManager content construction
    UserManager* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    mUserListCtl = new wxListCtrl( itemDialog1, ID_USER_LIST, wxDefaultPosition, wxSize(400, 400), wxLC_REPORT );
    itemBoxSizer2->Add(mUserListCtl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemDialog1, ID_ADD_USER, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton5->SetDefault();
    itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_DELETE_USER, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_EDIT_USER, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Connect events and objects
    mUserListCtl->Connect(ID_USER_LIST, wxEVT_LEFT_DCLICK, wxMouseEventHandler(UserManager::OnLeftDClick), NULL, this);
////@end UserManager content construction

    // Create custom windows not generated automatically here.
////@begin UserManager content initialisation
////@end UserManager content initialisation
}


/*!
 * Should we show tooltips?
 */

bool UserManager::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap UserManager::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval

}

/*!
 * Get icon resources
 */

wxIcon UserManager::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin UserManager icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end UserManager icon retrieval
}


/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_USER_LIST
 */

void UserManager::OnUSERLISTSelected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_USER_LIST in UserManager.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_USER_LIST in UserManager. 
}


/*!
 * wxEVT_LEFT_DCLICK event handler for ID_USER_LIST
 */

void UserManager::OnLeftDClick( wxMouseEvent& event )
{
    EditUser();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_USER
 */

void UserManager::OnADDUSERClick( wxCommandEvent& event )
{
    UserDlg dlg;      
    dlg.Create(mWhidControl, NULL, this);
    if (dlg.ShowModal() == wxID_OK)
    {
        ReloadUsers();
    }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_ADD_USER
 */

void UserManager::OnADDUSERUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for ID_ADD_USER in UserManager.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_UPDATE_UI event handler for ID_ADD_USER in UserManager. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_USER
 */

void UserManager::OnDELETEUSERClick( wxCommandEvent& event )
{
    
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_DELETE_USER
 */

void UserManager::OnDELETEUSERUpdate( wxUpdateUIEvent& event )
{
    event.Enable(mUserListCtl->GetSelectedItemCount() > 0);
}


/*!
 * wxEVT_INIT_DIALOG event handler for ID_USERMANAGER
 */

void UserManager::OnInitDialog( wxInitDialogEvent& event )
{
    wxDialog::OnInitDialog(event);

    // Get the user-list
    ReloadUsers(); 
}

void UserManager::ReloadUsers()
{
    mUserListCtl->DeleteAllItems();
    mUsers.clear();

    try
    {
        
        mWhidControl->GetUserList(mUsers, CTX);

        const int num_users = (int)mUsers.size();
        int row = 0;
        for(; row < num_users; row++)
        {
            const WHID::UserInfo& user = mUsers[row];

            WarUtf8 name = user.mName,
                login_name = user.mLoginName;

            const int nitem = mUserListCtl->InsertItem(
                row, wxString(name.GetUnicode().c_str()));
            mUserListCtl->SetItemData(nitem, row);

            mUserListCtl->SetItem(nitem, HC_LOGIN_NAME, 
                wxString(login_name.GetUnicode().c_str()));
            mUserListCtl->SetItem(nitem, HC_ACTIVE, 
                user.mActive ? _("Yes") : _("No"));
             mUserListCtl->SetItem(nitem, HC_ADMIN, 
                user.mAdmin ? _("Yes") : _("No"));
        }
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Caught Exception: " << ex << war_endl;
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EDIT_USER
 */

void UserManager::OnEditUserClick( wxCommandEvent& event )
{
    EditUser();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_EDIT_USER
 */

void UserManager::OnEditUserUpdate( wxUpdateUIEvent& event )
{
    event.Enable(mUserListCtl->GetSelectedItemCount() > 0);
}

void UserManager::EditUser()
{
    // Is there an active item?
    WAR_DEBUGLOG;
    debug_log << "Doubleclicked" << war_endl;
    
    long item = -1;
    for ( ;; ) 
    { 
        item = mUserListCtl->GetNextItem(item, 
            wxLIST_NEXT_ALL, 
            wxLIST_STATE_SELECTED); 
        if (-1 == item) 
            break; 
        
        // this item is selected - do whatever is needed with it 
        UserDlg dlg;
        WHID::UserPrx user = mUsers[item].mUser;
        dlg.Create(mWhidControl, user, this);
        if (dlg.ShowModal() == wxID_OK)
        {
            ReloadUsers();
        }
    } 
}

