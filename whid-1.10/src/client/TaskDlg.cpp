#include "libwhid.h"
#include "TaskDlg.h"
#include "WhidApp.h"

TaskDlg::TaskDlg(void)
{
}

TaskDlg::~TaskDlg(void)
{
}


void TaskDlg::OnCustomizeToLevel()
{
    if (NULL == mImage)
    {
        mImage = WhidApp::LoadImageFromResource(
            _("icons/128x128/task.png"));
    }
    if (mImage)
        mTopLeftImageCtl->SetBitmap(*mImage);

    SetTitle(_("Task"));
    mActiveStateLabelCtl->SetLabel(_("Is the task active?"));

    mNodeActiveStatusCtl->Delete(0);

}

wxBitmap *TaskDlg::mImage = NULL;

