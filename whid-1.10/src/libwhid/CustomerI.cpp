#include "libwhid.h"
#include "server/CustomerI.h"   // class implemented

#include "server/WhidEngine.h"
#include <database/WarDbConnection.h>
#include <database/WarDbQuery.h>
#include <ice/WarIceException.h>

#include "libwhid.h"
#include "server/ProjectI.h"
#include "server/InvoiceI.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

WarLog& operator << (WarLog& o, const WHID::CustomerPrx& c)
{
    o << c->Explain(Ice::Context());
    return o;
}

WarLog& operator << (WarLog& o, const CustomerI& c)
{
    o << c.Explain(Ice::Current());
    return o;
}

//============================= LIFECYCLE ====================================

CustomerI::CustomerI()
{
}// CustomerI

CustomerI::~CustomerI()
{
}// ~CustomerI


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================


void CustomerI::MakeId(const war_int64_t CustomerId, 
        Ice::Identity& id)
{
    WarIceCore::GetIdentity(id, "Customer", CustomerId);
}

WHID::CustomerPrx CustomerI::GetProxy(const war_int64_t CustomerId)
{
    Ice::Identity id;
    MakeId(CustomerId, id);

    return WHID::CustomerPrx::uncheckedCast(
        WhidEngine::GetEngine().GetAdapter().createProxy(id));
}


//============================= CALLBACK   ===================================
//============================= ICE METHODS   ================================

void CustomerI::GetProjects(bool onlyActive,
                            ::WHID::ProjectList& children,
                            const Ice::Current& current) const
{
    VALIDATE_SESSION;

    WHID::NodeInfoList list;
    GetChildren(onlyActive, list, current);
    WHID::NodeInfoList::iterator child;
    for(child = list.begin(); child != list.end(); child++)
    {
        WHID::ProjectPrx prx = WHID::ProjectPrx::uncheckedCast(child->mNode);

        children.push_back(prx);
    }
}

::WHID::NodePrx
CustomerI::GetParent(const Ice::Current& current)
{
    VALIDATE_SESSION;
    return 0; // Root level
}

::WHID::ProjectPrx
CustomerI::CreateProject(const ::WHID::NodeData& data,
			       const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;

    try
    {
        WarDbConnection db;
        WhidEngine::GetEngine().GetDbPool().GetConnection(db);
        WarDbQuery query(db);

        //war_int64_t parent = GetParentId();

        query = "INSERT INTO Projects "
            " (CustomerId, Name, Notes, ActiveStatus) "
            " VALUES (", GetNativeKey(),
            ",", data.mName, 
            ",", data.mNotes,
            ",", data.mActiveStatus,
            ")";

        query.Exec();

        WHID::ProjectPrx project = ProjectI::GetProxy(query.GetLastInsertId());
        project->SetData(data, current.ctx);
        return project;
    }
    catch(WarExceptionAlreadyExist)
    {
        WAR_NOTICELOG;
        notice_log << "A project witn name \""
            << data.mName
            << "\" already exist. Cannot create another one with the same name."
            << war_endl;

        throw WHID::AlreadyExistException();
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

::WHID::InvoicePrx
CustomerI::MakeInvoice(const ::WHID::ProjectList& projects,
                       const ::WHID::UserPrx& forUser,
                       const Ice::Current& current)
{
    VALIDATE_SESSION;

    if (!is_admin && (my_session.mUserId != forUser->GetId(current.ctx)))
    {
        WAR_NOTICELOG;
        notice_log << *this << " failed VALIDATE_SESSION_USER" << war_endl;
        throw WHID::AccessDeniedException();
    }

    try
    {

        war_int64_t invoice_id = 0;

        {
            WarDbConnection db;
            WhidEngine::GetEngine().GetDbPool().GetConnection(db);
            WarDbQuery query(db);

            query = "INSERT INTO Invoice (UserId, CustomerId, Charge, InvoiceDate) "
                " VALUES(", (war_int64_t)forUser->GetId(current.ctx),
                ",", GetNativeKey(),
                ",", 0.0,
                ",", db.GetDriver()->GetNowVerb(),
                ")";

            query.Exec();
            invoice_id = query.GetLastInsertId();
        }

        WHID::ChargeDataList all_charges;
        ::WHID::ProjectList::const_iterator prj;
        for(prj = projects.begin(); prj != projects.end(); prj++)
        {
            // Append to the list
            (*prj)->GetCharges(WHID::WFPENDINGINV, forUser, all_charges, current.ctx);
        }

        {
            WarDbConnection db;
            WhidEngine::GetEngine().GetDbPool().GetConnection(db);
            WarDbQuery query(db), query2(db);

            double charge_total = 0, discount_total = 0, hours_total = 0,
                cost_total = 0, profit_total = 0;

            war_int64_t work_id = -1;
            query = "UPDATE Work SET InvoiceId = ", invoice_id,
                " WHERE Id = ", work_id; 

            war_int64_t charge_id = -1;
            query2 = "UPDATE Charge SET InvoiceId = ", invoice_id,
                " WHERE Id = ", charge_id;

            WHID::ChargeDataList::iterator charge;
            for(charge = all_charges.begin()
                ; charge != all_charges.end()
                ; charge++)
            {
                charge_total += charge->mCharge;
                discount_total += charge->mDiscount;
                hours_total += charge->mNumHours;
                cost_total += charge->mCost;
                profit_total += charge->mProfit;

                charge_id = charge->mId;
                query2.ReBind();
                query2.Exec();

                if (work_id == charge->mWorkId)
                {
                    continue;
                }

                // Assign the work-item with this invoice
                work_id = charge->mWorkId;
                query.ReBind();
                query.Exec();
            }

            // Update the totals
            query = "UPDATE Invoice SET "
                " Charge = ", charge_total,
                ", Discount = ", discount_total,
                ", Hours = ", hours_total,
                ", Cost = ", cost_total,
                ", Profit = ", profit_total,
                " WHERE Id = ", invoice_id;

            query.Exec();
        }

        return InvoiceI::GetProxy(invoice_id);

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
CustomerI::GetCustomerData(::WHID::CustomerData& data,
				 const Ice::Current& current) const
{
    VALIDATE_SESSION;
    
    {
        WAR_AUTO_LOCK;
        data = mCustomerData;
    }
}

void
CustomerI::SetCustomerData(const ::WHID::CustomerData& data,
				 const Ice::Current& current)
{
    VALIDATE_SESSION_ADM;

    {
        WAR_AUTO_LOCK;
        mCustomerData = data;
        Flush(-1);
    }
}

//============================= ACCESS     ===================================
//============================= INQUIRY    ===================================

bool CustomerI::IsActive() const
{
    return mIsActive;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

void CustomerI::GetChildrenClause(WarDbQuery& query) const throw (WarException)
{
    query += " WHERE CustomerId = ", GetNativeKey();
}

void CustomerI::OnLoadAddJoins(WarDbQuery& query) throw (WarException)
{
}

void CustomerI::OnLoadAddToSelect(WarDbQuery& query) throw (WarException)
{
    query += ",IsActive , Email ";
}

void CustomerI::OnLoadAddToWhere(WarDbQuery& query) throw (WarException)
{
}

void CustomerI::OnLoadFetch(war_db_resultset_ptr_t& row) throw (WarException)
{
    row->Get(C_IS_ACTIVE, mIsActive);
    row->Get(C_EMAIL, mCustomerData.mEmail);
}

void CustomerI::OnLoadAddToSet(WarDbQuery& query) throw (WarException)
{
    query += ", IsActive = ", mIsActive, 
        ", Email = ", mCustomerData.mEmail;
}

void CustomerI::SetActive(const bool active)
{
    WAR_AUTO_LOCK;
    if (mIsActive != active)
    {
        mIsActive = active;
        Flush(-1);
    }
}

/////////////////////////////// PRIVATE    ///////////////////////////////////

