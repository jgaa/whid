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

#ifndef _APP_RESOURCES_H_
#define _APP_RESOURCES_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface ""
#endif

/*!
 * Control identifiers
 */

////@begin control identifiers
    /// Control identifiers
    enum {
        ID_EDIT_TASK = 10042
    };
    /// Control identifiers
    enum {
        ID_EXPORT_INVOICES_TO_XML = 10158,
        ID_EXPORT_INVOICES_TO_HTML = 10159,
        ID_EXPORT_TO_EMAIL = 10183
    };
    /// Control identifiers
    enum {
        ID_EDIT_WORK = 10124,
        ID_DELETE_WORK = 10125
    };
    /// Control identifiers
    enum {
        ID_EDIT_PROJECT = 10034,
        ID_NEW_TASK = 10014
    };
    /// Control identifiers
    enum {
        ID_NEW_CUSTOMER = 10001,
        ID_NEW_PROJECT = 10011,
        ID_EDIT_CUSTOMER = 10033
    };
////@end control identifiers

class AppResources
{
public:
    AppResources() {}

/*!
 * Resource functions
 */

////@begin AppResources resource functions
    /// Menu creation function for ID_TASK_POPUP_MENU
    static wxMenu* CreateTaskPopupMenu();

    /// Menu creation function for ID_INVOICES_POPUP_MENU
    static wxMenu* CreateInvoicesPopupMenu();

    /// Menu creation function for ID_WORK_POPUP_MENU
    static wxMenu* CreateWorkPopupMenu();

    /// Menu creation function for ID_PROJECT_POPUP_MENU
    static wxMenu* CreateProjectPopupMenu();

    /// Menu creation function for ID_CUSTOMER_POPUP_MENU
    static wxMenu* CreateCustomerPopupMenu();

    /// Retrieves bitmap resources
    static wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    static wxIcon GetIconResource( const wxString& name );
////@end AppResources resource functions

};

#endif
    // _APP_RESOURCES_H_
