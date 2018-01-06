/////////////////////////////////////////////////////////////////////////////
// Name:        HTMLReport.h
// Purpose:     
// Author:      Jarle Aase
// Modified by: 
// Created:     06/08/2007 11:01:15
// RCS-ID:      
// Copyright:   Copyright 2007 Jarle (jgaa) Aase All Rights Reserved
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _HTMLREPORT_H_
#define _HTMLREPORT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "HTMLReport.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes


#include <wx/listbox.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_HTMLREPORT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_HTMLREPORT_TITLE _("HTML Report")
#define SYMBOL_HTMLREPORT_IDNAME ID_HTMLREPORT
#define SYMBOL_HTMLREPORT_SIZE wxSize(400, 300)
#define SYMBOL_HTMLREPORT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * HTMLReport class declaration
 */

class HTMLReport: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( HTMLReport )
    DECLARE_EVENT_TABLE()

public:
    typedef std::vector<std::string> files_t;

    /// Constructors
    HTMLReport();
    HTMLReport( wxWindow* parent, wxWindowID id = SYMBOL_HTMLREPORT_IDNAME, const wxString& caption = SYMBOL_HTMLREPORT_TITLE, const wxPoint& pos = SYMBOL_HTMLREPORT_POSITION, const wxSize& size = SYMBOL_HTMLREPORT_SIZE, long style = SYMBOL_HTMLREPORT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_HTMLREPORT_IDNAME, const wxString& caption = SYMBOL_HTMLREPORT_TITLE, const wxPoint& pos = SYMBOL_HTMLREPORT_POSITION, const wxSize& size = SYMBOL_HTMLREPORT_SIZE, long style = SYMBOL_HTMLREPORT_STYLE );

    /// Destructor
    ~HTMLReport();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin HTMLReport event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOKClick( wxCommandEvent& event );

////@end HTMLReport event handler declarations

////@begin HTMLReport member function declarations

    int GetMReport() const { return mReport ; }
    void SetMReport(int value) { mReport = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end HTMLReport member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin HTMLReport member variables
    wxListBox* mReportCtl;
    int mReport;
    /// Control identifiers
    enum {
        ID_HTMLREPORT = 10181,
        ID_LISTBOX1 = 10182
    };
////@end HTMLReport member variables

    files_t mFiles;
};

#endif
    // _HTMLREPORT_H_
