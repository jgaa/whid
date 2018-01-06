#pragma once
#include "NodeDlg.h"

class ProjectDlg :
    public NodeDlg
{
public:
    ProjectDlg(void);
    virtual ~ProjectDlg(void);

protected:
    void OnCustomizeToLevel();

    static wxBitmap *mImage;
    
};
