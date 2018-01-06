#include "libwhid.h"
#include "ProjectDlg.h"
#include "WhidApp.h"

ProjectDlg::ProjectDlg(void)
{
}

ProjectDlg::~ProjectDlg(void)
{
}


void ProjectDlg::OnCustomizeToLevel()
{
    if (NULL == mImage)
    {
        mImage = WhidApp::LoadImageFromResource(
            _("icons/128x128/project.png"));
    }
    if (mImage)
        mTopLeftImageCtl->SetBitmap(*mImage);

    SetTitle(_("Project"));
    mActiveStateLabelCtl->SetLabel(_("Is the project active?"));
}

wxBitmap *ProjectDlg::mImage = NULL;

