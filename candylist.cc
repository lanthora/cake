#include "candylist.h"
#include "candyitem.h"
#include <QSettings>

CandyList::CandyList()
{
    QSettings settings("canets", "cake");
    for (const QString &group : settings.childGroups()) {
        CandyItem *item = new CandyItem;
        item->setText(group);
        addItem(item);
    }
}
