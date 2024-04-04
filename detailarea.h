#ifndef DETAILAREA_H
#define DETAILAREA_H

#include <QFrame>
#include <QListWidgetItem>

class DetailArea : public QFrame
{
    Q_OBJECT

public:
    explicit DetailArea();
    void update(QListWidgetItem *item);
};

#endif // DETAILAREA_H
