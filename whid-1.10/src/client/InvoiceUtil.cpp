
#include "libwhid.h"
#include "InvoiceUtil.h"
#include "WhidApp.h"
#include <WarUtf8.h>
#include <WarFileWrapper.h>
#include <WarFastBuffer.h>
#include <WarTmpFileName.h>
#include <WarSmtpClient.h>
#include "EmailBillDlg.h"
#include <wx/busyinfo.h>
#include "HTMLReport.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#ifdef WIN32
#   include <shellapi.h>
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif



InvoiceUtil::InvoiceUtil(void)
{
}

InvoiceUtil::~InvoiceUtil(void)
{
}

void InvoiceUtil::EmailInvoices(const WHID::InvoiceList& invoices,
                                wxWindow *parentWindow)
{
    try
    {
        // Generate a HTML-file
        WarTmpFileName<wxChar> tmp_name;
        war_syspath_t tmp_html_file = tmp_name.GetTmpName(NULL, 
            _("whid-bill"), wxT("html"));
        ExportToHtml(tmp_html_file, invoices);
        WarPath<char> plain_file_name = tmp_html_file;

        // Put up a dialog with email-details and a text-field
        EmailBillDlg dlg;

        if (invoices.size() == 1)
        {
            // Take the email from the customer
            WHID::InvoiceInfo ii;
            invoices[0]->GetInfo(ii, CTX);

            WHID::CustomerData cd;
            ii.mCustomer->GetCustomerData(cd, CTX);

            dlg.mEmail = cd.mEmail;
        }

        dlg.Create(parentWindow);
        if (dlg.ShowModal() == wxID_OK)
        {
            wxBusyInfo busy(_("Sending message. Please wait..."));
            // Send the email with CC to the users email-address
            WarSmtpClient email;

            // Prepare attachment
            WarSmtpClient::attachments_t attachments;
            WarSmtpClient::attachment_t attachment;
            attachment.mPath = tmp_html_file;
            attachment.mType = WarSmtpClient::AT_FILE;
            attachment.mAttachmentName = "invoice.html";
            attachments.push_back(attachment);

            std::string server_host, reply_address, cram_md5_name, cram_md5_passwd;
            wxGetApp().mOptions.GetOption("/email/server/address", server_host);
            wxGetApp().mOptions.GetOption("/email/server/returnaddress", reply_address);

            wxGetApp().mOptions.GetOption("/email/auth/cram.md5/name", cram_md5_name);
            wxGetApp().mOptions.GetOption("/email/auth/cram.md5/passwd", cram_md5_passwd);

            WHID::UserData ud;
            USER->GetData(ud, CTX);

            war_smtp_client_auth_list_t auth;
            if (!cram_md5_name.empty())
            {
                auth.push_back(new WarSmtpClientAuthCramMd5(
                    cram_md5_name.c_str(), cram_md5_passwd.c_str()));
            }

            email.Create(server_host.c_str(), auth);
            email.Send(dlg.mEmail.c_str(),
                ud.mEmail.empty() ? NULL : ud.mEmail.c_str(),
                NULL,
                ud.mEmail.empty() ? "whid@whid.invalid" : ud.mEmail.c_str(),
                reply_address.empty() ? NULL : reply_address.c_str(),
                dlg.mSubject.c_str(), dlg.mMessage.c_str(),
                &attachments,
                "utf-8");        
        }

#ifdef WIN32
        DeleteFile(tmp_html_file.GetPath());
#else
        unlink(tmp_html_file.GetPath());
#endif
        
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to export and send the data: "
            << ex
            << war_endl;
    }
}

struct escape_str
{
	escape_str(const wxChar * val)
	{
		mVal = '\'';

        WarUtf8 transform = val;
        mVal += transform.GetUtf8();
		mVal += '\'';
	}

	operator const char *() 
	{
		return mVal.c_str();
	}

	std::string mVal;
};

void InvoiceUtil::ExportToHtml(const war_syspath_t& path,
                                        const WHID::InvoiceList& invoices) 
                                        throw(WarException)
{
    HTMLReport dlg;
    dlg.Create(NULL);
    if (dlg.ShowModal() != wxID_OK)
        return;

    std::string xsl_file = dlg.mFiles[dlg.mReport];


    wxBusyInfo busy(_("Exporting data. Please wait..."));
    try
    {
        WarTmpFileName<wxChar> tmp_name;
        war_syspath_t tmp_xml_file = tmp_name.GetTmpName(NULL, 
            _("whid-bill"), wxT("xml"));
        WarPath<char> my_tmp_xml_path = tmp_xml_file;

        ExportToXml(tmp_xml_file, invoices);

        WarFileWrapper out;

        {
            WarPath<char> html_path = path.GetPath();

            // Transform to HTML
            XALAN_USING_XERCES(XMLPlatformUtils);
            XALAN_USING_XALAN(XalanTransformer);
            XALAN_USING_XALAN(XSLTInputSource);
            XALAN_USING_XALAN(XSLTResultTarget);
            XALAN_USING_XALAN(XalanDOMString);

            XMLPlatformUtils::Initialize();
            XalanTransformer::initialize();

            XalanTransformer transformer;

            XSLTInputSource xml(my_tmp_xml_path.GetPath());
            XSLTInputSource xslt(xsl_file.c_str());
            XSLTResultTarget target(html_path.GetPath());



#define LNG(name, lng) \
            {\
                escape_str escape_##name(lng); \
                transformer.setStylesheetParam(#name, escape_##name);\
            }

            LNG(lng_report_summary, _("Report Summary"));
            LNG(lng_total_hours, _("Total hours"));
            LNG(lng_total, _("Total"));
            LNG(lng_cost, _("Cost"));
            LNG(lng_profit, _("Profit"));
            LNG(lng_invoice, _("Invoice #"));
            LNG(lng_project, _("Project"));
            LNG(lng_from, _("From"));
            LNG(lng_to, _("To"));
            LNG(lng_hours, _("Hours"));
            LNG(lng_rate, _("Rate"));
            LNG(lng_sum, _("Sum"));
            LNG(lng_discount, _("Discount"));            
            LNG(lng_task, _("Task")); 

            if (transformer.transform(xml, xslt, target))
            {
                const char *err = transformer.getLastError ();

                WAR_ERRLOG;
                err_log << "Failed to create html export: "
                    << err
                    << war_endl;
                return;
            }


            unlink(my_tmp_xml_path.GetPath());

        }
    }
    catch(WarException& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to export the data: "
            << ex
            << war_endl;
    }
}

void InvoiceUtil::ExportToXml(const war_syspath_t& path,
                              const WHID::InvoiceList& invoices) 
                              throw(WarException)
{
    std::string xml;
    xml = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<whid_invoices>\n";

    double total_hours = 0, total_charge = 0, total_cost = 0, total_profit = 0;
    
    WHID::InvoiceList::const_iterator invoice;
    for(invoice = invoices.begin()
        ; invoice != invoices.end()
        ; invoice++)
    {
        try
        {
            (*invoice)->GetAsXml(xml, total_hours, total_charge, 
                total_cost, total_profit, CTX);
        }
        catch(WHID::AccessDeniedException)
        {
            wxMessageBox(_("Access denied. You don't have permission to see invoices."));
        }
        catch(Ice::Exception& ex)
        {
            WAR_ERRLOG;
            err_log << "Caught exception: " << ex << war_endl;
        }
    }

    {
        char buf[128];
        sprintf(buf, "<summary hours=\"%.02lf\" charge=\"%.02lf\" " 
            "cost=\"%.02lf\" profit=\"%.02lf\"/>\n",
            total_hours, total_charge, total_cost, total_profit);

        xml += buf;
    }

    xml += "\n</whid_invoices>\n";

    WarFileWrapper fp;
    fp.Open(path, WAR_FOPEN_BINARY_WRITE);
    fp.Write(xml.c_str(), xml.size(), false);
    fp.Close();
}

