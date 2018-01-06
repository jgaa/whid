/////////////////////////////////////////////////////////////////////////////
// Name:        EmailBillDlg.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     28/07/2007 12:03:21
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EMAILBILLDLG_H_
#define _EMAILBILLDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "EmailBillDlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

#include <wx/dialog.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_EMAILBILLDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_EMAILBILLDLG_TITLE _("Email")
#define SYMBOL_EMAILBILLDLG_IDNAME ID_EMAILBILLDLG
#define SYMBOL_EMAILBILLDLG_SIZE wxSize(400, 300)
#define SYMBOL_EMAILBILLDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * EmailBillDlg class declaration
 */

class EmailBillDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( EmailBillDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    EmailBillDlg();
    EmailBillDlg( wxWindow* parent, wxWindowID id = SYMBOL_EMAILBILLDLG_IDNAME, const wxString& caption = SYMBOL_EMAILBILLDLG_TITLE, const wxPoint& pos = SYMBOL_EMAILBILLDLG_POSITION, const wxSize& size = SYMBOL_EMAILBILLDLG_SIZE, long style = SYMBOL_EMAILBILLDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EMAILBILLDLG_IDNAME, const wxString& caption = SYMBOL_EMAILBILLDLG_TITLE, const wxPoint& pos = SYMBOL_EMAILBILLDLG_POSITION, const wxSize& size = SYMBOL_EMAILBILLDLG_SIZE, long style = SYMBOL_EMAILBILLDLG_STYLE );

    /// Destructor
    ~EmailBillDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin EmailBillDlg event handler declarations

////@end EmailBillDlg event handler declarations

////@begin EmailBillDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end EmailBillDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin EmailBillDlg member variables
    /// Control identifiers
    enum {
        ID_EMAILBILLDLG = 10167,
        ID_EMAIL_RCPT = 10168,
        ID_SUBJECT = 10170,
        ID_TEXTCTRL3 = 10169
    };
////@end EmailBillDlg member variables

    std::string mEmail;
    std::string mSubject;
    std::string mMessage;
};

#endif
    // _EMAILBILLDLG_H_
