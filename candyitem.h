#ifndef CANDYITEM_H
#define CANDYITEM_H

#include <QListWidgetItem>

class CandyItem : public QListWidgetItem
{
public:
    explicit CandyItem();
    ~CandyItem();

    void update();

private:
    std::shared_ptr<void> candy;
    int randomHex();
    QString randomHexString(int length);
};

#endif // CANDYITEM_H
