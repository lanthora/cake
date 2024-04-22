#ifndef CANDYLIST_H
#define CANDYLIST_H

#include <QListWidget>

class CandyList : public QListWidget
{
public:
    explicit CandyList();
    ~CandyList();

private:
    int randomHex();
    QString randomHexString(int length);
};

#endif // CANDYLIST_H
