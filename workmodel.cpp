#include "workmodel.h"

#include <array>

using namespace std;

WorkModel::WorkModel()
{

}

void Work::setStatus(int newStatus)
{
    static const array<Status, 7> statuses = {
        {Work::Status::OPEN,
        Work::Status::DONE,
        Work::Status::INTERRUPTED, // App closed / crashed without updating the end time
        Work::Status::HELD_BACK, // Manual hold-back
        Work::Status::FREE,
        Work::Status::APPROVED,
        Work::Status::INVOICED}
    };

    status = statuses.at(static_cast<size_t>(newStatus));
}
