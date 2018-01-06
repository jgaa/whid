
#ifndef INVOICE_UTIL_H
#   define INVOICE_UTIL_H

class wxWindows;

#include <WarPath.h>

class InvoiceUtil
{
public:
    InvoiceUtil(void);
    ~InvoiceUtil(void);

    void EmailInvoices(const WHID::InvoiceList& invoices,
        wxWindow *parentWindow);

    void ExportToHtml(const war_syspath_t& path,
        const WHID::InvoiceList& invoices) 
        throw(WarException);

    void ExportToXml(const war_syspath_t& path,
        const WHID::InvoiceList& invoices) 
        throw(WarException);

};



#endif //INVOICE_UTIL_H


