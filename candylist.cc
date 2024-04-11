#include "candylist.h"
#include "candyitem.h"
#include <QSettings>

CandyList::CandyList()
{
    CandyItem::startKeepAlive();

    QSettings settings("canets", "cake");
    for (const QString &group : settings.childGroups()) {
        CandyItem *item = new CandyItem;
        item->setText(group);
        addItem(item);
        item->update();
    }
}

CandyList::~CandyList()
{
    CandyItem::stopKeepAlive();

    while (count() > 0) {
        CandyItem *item = dynamic_cast<CandyItem *>(takeItem(0));
        item->shutdown();
    }
}
