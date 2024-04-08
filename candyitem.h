#ifndef CANDYITEM_H
#define CANDYITEM_H

#include <QListWidgetItem>

class CandyItem : public QListWidgetItem
{
public:
    explicit CandyItem();
    ~CandyItem();

private:
    void *candy = nullptr;
};

#endif // CANDYITEM_H
