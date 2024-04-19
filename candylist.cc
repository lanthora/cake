#include "candylist.h"
#include "candy.h"
#include "candyitem.h"
#include <QSettings>

CandyList::CandyList()
{
    QSettings settings;
    for (const QString &group : settings.childGroups()) {
        CandyItem *item = new CandyItem;
        item->setText(group);
        addItem(item);
        item->update();
    }
}

CandyList::~CandyList()
{
    while (count() > 0) {
        CandyItem *item = dynamic_cast<CandyItem *>(takeItem(0));
        delete item;
    }
}
