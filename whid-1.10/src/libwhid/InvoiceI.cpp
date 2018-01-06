#include "libwhid.h"
#include "server/InvoiceI.h"   // class implemented
#include "server/CustomerI.h"
#include "server/UserI.h"

#include <server/WhidEngine.h>
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>
#include <WarFastBuffer.h>
#include <WarTmpFileName.h>


/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::InvoicePrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const InvoiceI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

InvoiceI::InvoiceI()
{
}// InvoiceI

InvoiceI::~InvoiceI()
{
}// ~InvoiceI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void InvoiceI::Load () throw (WarException)
{
    WarDbConnection db;
    WhidEngine::GetEngine().GetDbPool().GetConnection(db);
    WarDbQuery query(db);
    war_db_resultset_ptr_t rows;

    enum ColsE { C_USERID, C_CUSTOMERID, C_CHARGE, C_DISCOUNT, C_INVOICEDATE,
        C_FROZEN, C_COST, C_PROFIT };
    query = "SELECT UserId, CustomerId, Charge, Discount, InvoiceDate, "
        " Frozen, Cost, Profit "
        " FROM Invoice "
        " WHERE Id  = ", GetNativeKey();

    query.Exec(rows);
    if (rows->IsEof())
        WAR_THROW_NOT_FOUND("Category not found");

    rows->Get(C_USERID, mUserId);
    rows->Get(C_CUSTOMERID, mCustomerId);
    rows->Get(C_CHARGE, mCharge);
    rows->Get(C_PROFIT, mProfit);
    rows->Get(C_COST, mCost);
    rows->Get(C_DISCOUNT, mDiscount);
    rows->Get(C_INVOICEDATE, mInvoiceDate);
    rows->Get(C_FROZEN, mFrozen);

    // Get hour summary
    query = "SELECT SUM(NumHours) FROM Charge "
        " WHERE InvoiceId = ", GetNativeKey();
    
    query.Exec(rows);
    rows->Get(0, mHours);

    mDirty = 0;
}

void InvoiceI::Save () throw (WarException)
{
    if (mDirty)
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        query = "UPDATE Invoice SET "
            " Charge = ", mCharge,
            ", Cost = ", mCost,
            ", Profit = ", mProfit,
            ", Discount = ", mDiscount,
            ", InvoiceDate = ", mInvoiceDate,
            ", Frozen = ", mFrozen,
            " WHERE Id = ", GetNativeKey();

        query.Exec();
    }
}

void InvoiceI::MakeId(const war_int64_t InvoiceId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "Invoice", InvoiceId);
}

WHID::InvoicePrx InvoiceI::GetProxy(const war_int64_t InvoiceId)
{
    Ice::Identity id;
    MakeId(InvoiceId, id);

    return WHID::InvoicePrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

void
InvoiceI::Commit(const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin\
        && (my_session.mUserId != mUserId))
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    if (mFrozen)
        return;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);
        war_db_resultset_ptr_t rows;

        // Update the work-items
        query = "UPDATE Work "
            " SET Status = ", WHID::WSINVOICED,
            " WHERE InvoiceId = ", GetNativeKey(),
            " AND Status = ", WHID::WSPENDINGINV;

        query.Exec();

        // Freeze the invoice
        mFrozen = true;
        Flush(-1);
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
}

void
InvoiceI::GetInfo(::WHID::InvoiceInfo& info,
                  const Ice::Current& current) const
{
    VALIDATE_SESSION;

    info.mCustomerName = CustomerI::GetProxy(mCustomerId)->GetName(current.ctx);
    info.mUserName = UserI::GetProxy(mUserId)->GetName(current.ctx);

    WAR_AUTO_LOCK;
    info.mId = atoi(GetNativeKey().c_str());
    info.mCharge = mCharge;
    info.mHours = mHours;
    info.mCost = mCost;
    info.mProfit = mProfit;
    info.mInvoice = GetProxy(atoi(GetNativeKey().c_str()));
    info.mFrozen = mFrozen;
    info.mInvoiceDate = mInvoiceDate;
    info.mCustomer = CustomerI::GetProxy(mCustomerId);
    info.mUser = UserI::GetProxy(mUserId);
}


::std::string
InvoiceI::Explain(const Ice::Current& current) const
{
    std::stringstream msg;
    {
        WAR_AUTO_LOCK;
        msg << "\"Invoice # " << GetNativeKey() << "\"";
    }

    return msg.str();
}

void
InvoiceI::GetAsXml(::std::string& xmlBuffer,
			 ::Ice::Double& totalHours,
			 ::Ice::Double& totalCharge,
			 ::Ice::Double& totalCost,
			 ::Ice::Double& totalProfit,
			 const Ice::Current& current)
{
    VALIDATE_SESSION;
    if (!is_admin\
        && (my_session.mUserId != mUserId))
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    WHID::UserPrx user = UserI::GetProxy(mUserId);

    WarFastBufferOutToStr<char> output_buffer(xmlBuffer);
    WarFastBuffer<WarFastBufferOutToStr<char> > xml(&output_buffer, 1024 * 32, false);

#define PUSH(tag) war_push_xml_tag<WarFastBufferOutToStr<char> > push_##tag(xml, #tag)
#define PUSHA(tag) war_push_xml_tag<WarFastBufferOutToStr<char> > push_##tag(xml, #tag, true)

    double project_hours = 0, project_charge = 0, 
        project_cost, project_profit,
        task_hours = 0, task_charge = 0, task_cost = 0, 
        task_profit = 0;

    try
    {
        WHID::InvoiceInfo info;
        GetInfo(info, current);

        PUSHA(invoice);
        xml.WriteXmlAttribute("id", info.mId);
        push_invoice.CloseInit(); // Close the <invoice> node
        
        {
            PUSHA(customer);
            xml.WriteXmlAttribute("name", info.mCustomerName);
        }

        totalHours += info.mHours;
        totalCharge += info.mCharge;
        totalCost += info.mCost;
        totalProfit += info.mProfit;

        {
            PUSHA(totals);
            xml.WriteXmlAttribute("charge", info.mCharge);
            xml.WriteXmlAttribute("hours", info.mHours);
            xml.WriteXmlAttribute("cost", info.mCost);
            xml.WriteXmlAttribute("profit", info.mProfit);
        }

        {
            PUSHA(info);
            xml.WriteXmlAttribute("date", info.mInvoiceDate);
        }

        WHID::ChargeDataList list;
        GetCharges(WHID::WFALL, user, list, current);

        {
            PUSH(projects);

            WHID::ChargeDataList::iterator node, peek;
            for(node = list.begin(); node != list.end(); node++)
            {
                project_hours = 0;
                project_charge = 0;
                project_cost = 0;
                project_profit = 0;
                task_hours = 0; 
                task_charge = 0;
                task_profit = 0;
                task_cost = 0;

                PUSH(project);

                {
                    PUSHA(info);

                    xml.WriteXmlAttribute("name", 
                        node->mProject->GetName(current.ctx));
                }

                bool is_same_project = false;
                bool is_same_task = false;
                bool is_same_work = false;

                do
                {
                    task_hours = 0; 
                    task_charge = 0;
                    task_cost = 0;
                    task_profit = 0;

                    PUSH(task);

                    {
                        PUSHA(info);
                        xml.WriteXmlAttribute("name", node->mTask->GetName(current.ctx));
                    }

                    do
                    {


                        PUSH(work);

                        {
                            WHID::WorkData w;
                            node->mWork->GetData(w, current.ctx);

                            {
                                PUSHA(info);
                                xml.WriteXmlAttribute("from", w.mFromTime);
                                xml.WriteXmlAttribute("to", w.mToTime);
                                xml.WriteXmlAttribute("paused", w.mMinutesPaused);
                            }

                            xml.WriteXml("descr", w.mNotes);
                        }

                        do
                        {
                            project_hours += node->mNumHours;
                            project_charge += node->mCharge;
                            project_cost += node->mCost;
                            project_profit += node->mProfit;
                            task_hours += node->mNumHours;
                            task_charge += node->mCharge;
                            task_cost += node->mCost;
                            task_profit += node->mProfit;

                            PUSH(charge);
                            {
                                PUSHA(info);

                                xml.WriteXmlAttribute("hours", node->mNumHours);
                                xml.WriteXmlAttribute("charge", node->mCharge);
                                xml.WriteXmlAttribute("cost", node->mCost);
                                xml.WriteXmlAttribute("profit", node->mProfit);

                                // Hide from/to-time for pauses.
                                if (node->mRate)
                                {
                                    xml.WriteXmlAttribute("discount", node->mDiscount);
                                    xml.WriteXmlAttribute("from", node->mWorkFrom);
                                    xml.WriteXmlAttribute("to", node->mWorkTo);
                                }
                            }

                            if (node->mRate)
                            {
                                PUSHA(rate);
                                xml.WriteXmlAttribute("name", node->mRateName);
                                xml.WriteXmlAttribute("charge_per_hour", node->mChargePrHour);
                                xml.WriteXmlAttribute("cost", node->mCost);
                            }

                            peek = node;
                            peek++;
                            is_same_work =  
                                (peek != list.end())
                                && (node->mWork->ice_getIdentity()
                                == peek->mWork->ice_getIdentity());

                            if (is_same_work)
                                node++;
                        } while(is_same_work);

                        peek = node;
                        peek++;
                        is_same_task =  
                            (peek != list.end())
                            && (node->mTask->ice_getIdentity()
                            == peek->mTask->ice_getIdentity());

                        if (is_same_task)
                            node++;
                    } while(is_same_task);

                    {
                        PUSHA(summary);
                        xml.WriteXmlAttribute("hours", task_hours);
                        xml.WriteXmlAttribute("charge", task_charge);
                        xml.WriteXmlAttribute("cost", task_cost);
                        xml.WriteXmlAttribute("profit", task_profit);
                    }

                    peek = node;
                    peek++;
                    is_same_project =  
                        (peek != list.end())
                        && (node->mProject->ice_getIdentity()
                        == peek->mProject->ice_getIdentity());

                    if (is_same_project)
                        node++;

                } while(is_same_project);

                {
                    PUSHA(summary);
                    xml.WriteXmlAttribute("hours", project_hours);
                    xml.WriteXmlAttribute("charge", project_charge);
                    xml.WriteXmlAttribute("cost", project_cost);
                    xml.WriteXmlAttribute("profit", project_profit);
                }
            }
        }
    }
    catch(const WarException& ex)
    {
        WAR_ERRLOG;
        err_log << *this << " caught an exception: "
            << ex.what()
            << war_endl;

        throw WHID::InternalErrorException();
    }
    catch(const Ice::Exception& ex)
    {
        WAR_ERRLOG;
        err_log << "Failed to fetch data: "
            << ex
            << war_endl;

        throw WHID::InternalErrorException();
    }

    xml.Flush(true);
}


//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

