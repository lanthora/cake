#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "framelessdialog.h"
#include <QObject>

class Feedback : public FramelessDialog
{
    Q_OBJECT
public:
    Feedback(QWidget *parent = nullptr);
};

#endif // FEEDBACK_H
