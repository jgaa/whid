#pragma once
#include "NodeDlg.h"

class CustomerDlg :
    public NodeDlg
{
public:
    CustomerDlg(void);
    virtual ~CustomerDlg(void);

protected:
    void OnCustomizeToLevel();
    
};
