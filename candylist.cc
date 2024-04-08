#include "candylist.h"

CandyItem *CandyList::addButton = new CandyItem;

CandyList::CandyList()
{
    // 网络列表最后一项 '+', 用于添加新客户端
    addButton->setText("+");
    addItem(addButton);
}

void CandyList::addCandyItem(CandyItem *item)
{
    insertItem(count() - 1, item);
    setCurrentItem(item);
}
