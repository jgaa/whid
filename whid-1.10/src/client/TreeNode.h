#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <wx/treectrl.h>

class TreeNode : public wxTreeItemData
{
public:
    TreeNode(const WHID::NodeTypeE nodeType);
    virtual ~TreeNode(void);

    WHID::NodePrx mNode;

    WHID::NodeTypeE GetType()
    {
        return mNodeType;
    }

protected:
    const WHID::NodeTypeE mNodeType;
};


#endif //TREE_NODE_H

