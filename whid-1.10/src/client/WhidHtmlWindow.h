#ifndef WhidHtmlWindow_H
#  define WhidHtmlWindow_H


#include <wx/html/htmlwin.h>

class WhidHtmlWindow :
    public wxHtmlWindow
{
public:
    WhidHtmlWindow(wxWindow *parent, wxWindowID id = -1, 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxHW_DEFAULT_STYLE, 
        const wxString& name = wxT("whidHtmlWindow"));

    virtual ~WhidHtmlWindow(void);

    virtual wxHtmlOpeningStatus OnOpeningURL(wxHtmlURLType type,
        const wxString& url, 
        wxString *redirect);

    virtual void OnLinkClicked(const wxHtmlLinkInfo& link);
};

#endif // WhidHtmlWindow_H


