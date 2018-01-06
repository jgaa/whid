/////////////////////////////////////////////////////////////////////////////
// Name:        NewPasswdDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     23/07/2007 11:36:21
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NEWPASSWDDLG_H_
#define _NEWPASSWDDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "NewPasswdDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_NEWPASSWDDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_NEWPASSWDDLG_TITLE _("Password")
#define SYMBOL_NEWPASSWDDLG_IDNAME ID_NEWPASSWDDLG
#define SYMBOL_NEWPASSWDDLG_SIZE wxSize(400, 300)
#define SYMBOL_NEWPASSWDDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * NewPasswdDlg class declaration
 */

class NewPasswdDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( NewPasswdDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    NewPasswdDlg();
    NewPasswdDlg( wxWindow* parent, wxWindowID id = SYMBOL_NEWPASSWDDLG_IDNAME, const wxString& caption = SYMBOL_NEWPASSWDDLG_TITLE, const wxPoint& pos = SYMBOL_NEWPASSWDDLG_POSITION, const wxSize& size = SYMBOL_NEWPASSWDDLG_SIZE, long style = SYMBOL_NEWPASSWDDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_NEWPASSWDDLG_IDNAME, const wxString& caption = SYMBOL_NEWPASSWDDLG_TITLE, const wxPoint& pos = SYMBOL_NEWPASSWDDLG_POSITION, const wxSize& size = SYMBOL_NEWPASSWDDLG_SIZE, long style = SYMBOL_NEWPASSWDDLG_STYLE );

    /// Destructor
    ~NewPasswdDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin NewPasswdDlg event handler declarations

////@end NewPasswdDlg event handler declarations

////@begin NewPasswdDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end NewPasswdDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin NewPasswdDlg member variables
    /// Control identifiers
    enum {
        ID_NEWPASSWDDLG = 10154,
        ID_PASSWD = 10155
    };
////@end NewPasswdDlg member variables

    std::string mPasswd;
};

#endif
    // _NEWPASSWDDLG_H_
