#include "candylist.h"
#include "candy.h"
#include "candyitem.h"
#include <random>
#include <sstream>
#include <QSettings>

CandyList::CandyList()
{
    QSettings settings;
    settings.setFallbacksEnabled(false);

    if (settings.value("vmac").isNull()) {
        settings.setValue("vmac", randomHexString(16));
        settings.sync();
    }

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

int CandyList::randomHex()
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<int> distrib(0, 15);
    return distrib(engine);
}

QString CandyList::randomHexString(int length)
{
    QString retval;
    QTextStream out(&retval);
    std::stringstream ss;
    for (int i = 0; i < length; i++) {
        out << QString::number(randomHex(), 16);
    }
    return retval;
}
