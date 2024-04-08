#ifndef CANDYLIST_H
#define CANDYLIST_H

#include "candyitem.h"
#include <QListWidget>

class CandyList : public QListWidget
{
public:
    explicit CandyList();
    void addCandyItem(CandyItem *item);

    static CandyItem *addButton;
};

#endif // CANDYLIST_H
