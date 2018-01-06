/////////////////////////////////////////////////////////////////////////////
// Name:        
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     17/12/2006 20:09:49
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation ""
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
////@end includes
#include "wx/xrc/xmlres.h"

#include "app_resources.h"

////@begin XPM images
////@end XPM images

/*!
 * Resource functions
 */

////@begin AppResources resource functions

/*!
 * Menu creation function for ID_TASK_POPUP_MENU
 */

wxMenu* AppResources::CreateTaskPopupMenu()
{
    wxMenu* itemMenu1 = new wxMenu;
    itemMenu1->Append(ID_EDIT_TASK, _("&Edit Task"), _T(""), wxITEM_NORMAL);
    return itemMenu1;
}


/*!
 * Menu creation function for ID_INVOICES_POPUP_MENU
 */

wxMenu* AppResources::CreateInvoicesPopupMenu()
{
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_EXPORT_INVOICES_TO_XML, _("Export to XML"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_EXPORT_INVOICES_TO_HTML, _("Export to HTML"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_EXPORT_TO_EMAIL, _("Export to Email"), _T(""), wxITEM_NORMAL);
    return itemMenu3;
}


/*!
 * Menu creation function for ID_WORK_POPUP_MENU
 */

wxMenu* AppResources::CreateWorkPopupMenu()
{
    wxMenu* itemMenu7 = new wxMenu;
    itemMenu7->Append(ID_EDIT_WORK, _("&Edit\t..."), _T(""), wxITEM_NORMAL);
    itemMenu7->Append(ID_DELETE_WORK, _("&Delete"), _T(""), wxITEM_NORMAL);
    return itemMenu7;
}


/*!
 * Menu creation function for ID_PROJECT_POPUP_MENU
 */

wxMenu* AppResources::CreateProjectPopupMenu()
{
    wxMenu* itemMenu10 = new wxMenu;
    itemMenu10->Append(ID_EDIT_PROJECT, _("&Edit Project"), _T(""), wxITEM_NORMAL);
    itemMenu10->Append(ID_NEW_TASK, _("Add a new &Task"), _T(""), wxITEM_NORMAL);
    return itemMenu10;
}


/*!
 * Menu creation function for ID_CUSTOMER_POPUP_MENU
 */

wxMenu* AppResources::CreateCustomerPopupMenu()
{
    wxMenu* itemMenu13 = new wxMenu;
    itemMenu13->Append(ID_NEW_CUSTOMER, _("Create a new &Customer"), _T(""), wxITEM_NORMAL);
    itemMenu13->Append(ID_NEW_PROJECT, _("Create a new &Project"), _T(""), wxITEM_NORMAL);
    itemMenu13->Append(ID_EDIT_CUSTOMER, _("&Edit Customer"), _T(""), wxITEM_NORMAL);
    return itemMenu13;
}

////@end AppResources resource functions

/*!
 * Get bitmap resources
 */

wxBitmap AppResources::GetBitmapResource( const wxString& name )
{
    return WhidApp::GetBitmapResource( name );
    // Bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AppResources::GetIconResource( const wxString& name )
{
    wxUnusedVar(name);
    // Icon retrieval
////@begin AppResources icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AppResources icon retrieval
}
