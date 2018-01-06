


#ifndef LIBWHID_H
#define LIBWHID_H

#ifdef HAVE_CONFIG_H
#   include "config.h"
#elif !defined(WIN32)
#   error
#endif

#include <warlib.h> // Include before Ice! 

#include <Ice/Ice.h>
#include <Ice/Config.h>

#include <WarCollector.h>
#include <WarLog.h>
#include <WarNativeStr.h>
#include <WarUtf8.h>
#include <WarAtoi.h>

#ifdef WHID_WITH_WXWIDGETS
#   include <wx/wxprec.h>
#   include <wx/intl.h>
#   include <wx/dialog.h>
#   include <wx/textctrl.h>
#   include <wx/statbmp.h>
#   include <wx/notebook.h>
#   include <wx/radiobox.h>
#   include <wx/panel.h>
#   include <wx/button.h>
#endif // WHID_WITH_WXWIDGETS


//#if defined(WIN32)
//#	if defined(LIBWHID_EXPORTS)
//#		define LIBWHID_API __declspec(dllexport)
//#	else
//#		define LIBWHID_API __declspec(dllimport)
//#	endif
//#else
#	define LIBWHID_API
//#endif

#include "whid.h"

#ifdef WHID_WITH_WXWIDGETS
#ifdef UNICODE
#   define STR_FROM_DLG(__variable, __control) {\
        wxString __tmp = __control->GetValue();\
        WarUtf8 __tmp_utf8 = __tmp.GetData(); \
        __variable = __tmp_utf8.GetUtf8();\
    }

#   define STR_TO_DLG(__variable, __control) {\
        WarUtf8 __tmp_utf8 = __variable;\
        wxString __tmp = __tmp_utf8.GetUnicode().c_str();\
        __control->SetValue(__tmp);\
        }
#else
#   define STR_FROM_DLG(__variable, __control) {\
        wxString __tmp = __control->GetValue();\
        __variable = __tmp.GetData();\
        WarUtf8::iso8859_1_to_utf8(__variable);\
    }
#   define STR_TO_DLG(__variable, __control) {\
        std::string __tmp = __variable; WarUtf8::utf8_to_iso8859_1(__tmp);\
        __control->SetValue(__tmp.c_str());\
        }
#endif

#define INT_TO_DLG(__variable, __control) {\
    wxString __tmp = wxString::Format(wxT("%d"), (int)__variable);\
    __control->SetValue(__tmp.c_str());\
    }

#define INT_FROM_DLG(__variable, __control) {\
        wxString __tmp = __control->GetValue();\
        WarUtf8 __tmp_utf8 = __tmp.GetData();\
        __variable = atoi(__tmp_utf8.GetUtf8().c_str());\
    }

#define SPIN_TO_DLG(__variable, __control) {\
    __control->SetValue(__variable);\
    }

#define SPIN_FROM_DLG(__variable, __control) {\
        __variable = __control->GetValue();\
    }

#define DOUBLE_TO_DLG(__variable, __control) {\
    wxString __tmp = wxString::Format(wxT("%.0lf"), (double)__variable);\
    __control->SetValue(__tmp.c_str());\
    }

#define DOUBLE_FROM_DLG(__variable, __control) {\
        wxString __tmp = __control->GetValue();\
        WarUtf8 __utf8_tmp = __tmp.GetData();\
        __variable = atof(__utf8_tmp.GetUtf8().c_str());\
    }

#define RADIO_TO_DLG(__variable, __control, enum_type) {\
    __control->SetSelection((int)__variable);\
    }


#define RADIO_TO_DLG(__variable, __control, enum_type) {\
    __control->SetSelection((int)__variable);\
    }

#define RADIO_FROM_DLG(__variable, __control, enum_type) {\
        __variable = (enum_type)__control->GetSelection();\
    }

#define CHOISE_TO_DLG(__variable, __control, enum_type) {\
    __control->SetSelection((int)__variable);\
    }

#define CHOISE_FROM_DLG(__variable, __control, enum_type) {\
        __variable = (enum_type)__control->GetSelection();\
    }


#define BOOL_TO_DLG(__variable, __control) {\
    __control->SetValue(__variable);\
    }

#define BOOL_FROM_DLG(__variable, __control) {\
        __variable = __control->IsChecked();\
    }


//#define STR_TO_UTF8(str) WarUtf8::iso8859_1_to_utf8(str)
//#define STR_FROM_UTF(str) WarUtf8::utf8_to_iso8859_1(str)
// STR_TO_UTF8(str, utf8)
// Convert from the internal string type to utf-8 
// for use by ice (or whatever). The source type can
// be std::string/std::wstring or const char * / const wchar_t *.
// The destination type is WarUtf8
//
// UTF8_TO_STR The oposite of above. The destination type
// can be std::string/std::wstring, WarCollector<war_sysch_t> or wxString

#ifdef UNICODE
    // Unicode

#   define STR_TO_UTF8(str, utf8) {\
        utf8 = str;\
        }

#   define UTF8_TO_STR(src, dst) {\
        WarUtf8 conv(src);\
        dst = conv.GetUnicode().c_str();\
        }

#else
    // Native 8-bit character set

#   define STR_TO_UTF8(str, utf8) {\
        WarCollector<wchar_t> conv(str);\
        utf8 = conv.GetValue();\
        }

#   define UTF8_TO_STR(src, dst) {\
        WarCollector<char> conv(src);\
        dst = conv.GetValue().c_str();\
        }
#endif
#endif // WHID_WITH_WXWIDGETS


#endif // LIBWHID_H

