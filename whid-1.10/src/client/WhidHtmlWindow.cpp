#include "libwhid.h"
#include "WhidHtmlWindow.h"
#include "WhidApp.h"

#ifdef WIN32
#   include <shellapi.h>
#endif

WhidHtmlWindow::WhidHtmlWindow(wxWindow *parent, wxWindowID id, 
        const wxPoint& pos, 
        const wxSize& size, 
        long style, 
        const wxString& name)
        : wxHtmlWindow(parent, id, pos, size, style, name)
{
}

WhidHtmlWindow::~WhidHtmlWindow(void)
{
}

wxHtmlOpeningStatus WhidHtmlWindow::OnOpeningURL(wxHtmlURLType type,
                                                 const wxString& url, 
                                                 wxString *redirect)
{
    return wxHTML_OPEN;
}

void WhidHtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link)
{
    WarUtf8 url = link.GetHref().GetData();

    BROWSER(link.GetHref());
}

