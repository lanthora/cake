#include "feedback.h"
#include <QVBoxLayout>

Feedback::Feedback(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("问题反馈");
    setFixedSize(300, 200);

    QLabel *QQLabel = new QLabel("QQ: 768305206", this);

    QLabel *TGLabel = new QLabel("TG: <a href=\"https://t.me/CandyUserGroup\">Click to Join</a>", this);
    TGLabel->setOpenExternalLinks(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(QQLabel, 0, Qt::AlignCenter);
    layout->addWidget(TGLabel, 0, Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);
}
