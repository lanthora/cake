#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QDialog>
#include <QObject>

class Feedback : public QDialog
{
    Q_OBJECT
public:
    Feedback(QWidget *parent = nullptr);
};

#endif // FEEDBACK_H
