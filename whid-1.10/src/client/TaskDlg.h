#pragma once
#include "NodeDlg.h"

class TaskDlg :
    public NodeDlg
{
public:
    TaskDlg(void);
    virtual ~TaskDlg(void);

protected:
    void OnCustomizeToLevel();

    static wxBitmap *mImage;
    
};
