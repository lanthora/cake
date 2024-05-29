#include "feedback.h"
#include <QLabel>
#include <QVBoxLayout>

Feedback::Feedback(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("问题反馈");
    setFixedSize(500, 300);

    QLabel *qq = new QLabel(
        "<b>QQ:</b><br>"
        "768305206<br>",
        this);
    qq->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QLabel *telegram = new QLabel(
        "<b>Telegram:</b><br>"
        "<a href=\"https://t.me/CandyUserGroup\">Click to Join</a><br>",
        this);
    telegram->setOpenExternalLinks(true);

    QLabel *github = new QLabel(
        "<b>Github:</b><br>"
        "<a href=\"https://github.com/lanthora/cake/issues/new\">Create an issue</a>",
        this);
    github->setOpenExternalLinks(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(qq, 0, Qt::AlignLeft);
    layout->addWidget(telegram, 0, Qt::AlignLeft);
    layout->addWidget(github, 0, Qt::AlignLeft);
    layout->setAlignment(Qt::AlignCenter);
}
