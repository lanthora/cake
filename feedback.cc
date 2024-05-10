#include "feedback.h"
#include <QLabel>
#include <QVBoxLayout>

Feedback::Feedback(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("问题反馈");
    setFixedSize(500, 300);

    QLabel *label = new QLabel(
        "<b>QQ:</b><br>768305206<br><br>"
        "<b>Telegram:</b><br><a href=\"https://t.me/CandyUserGroup\">Click to Join</a><br><br>"
        "<b>Github:</b><br><a href=\"https://github.com/lanthora/cake/issues/new\">Create an issue</a><br>",
        this);

    label->setAlignment(Qt::AlignLeft);
    label->setOpenExternalLinks(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);
}
