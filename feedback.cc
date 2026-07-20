#include "feedback.h"
#include <QLabel>
#include <QVBoxLayout>

Feedback::Feedback(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Feedback");
    resize(400, 200);
    setMinimumSize(360, 180);

    QLabel *qq = new QLabel("QQ: 768305206", this);
    qq->setObjectName("feedbackItem");
    qq->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QLabel *telegram = new QLabel(
        "Telegram: <a href=\"https://t.me/CandyUserGroup\" style=\"color: #4a90d9; text-decoration: none;\">Join Group</a>", this);
    telegram->setObjectName("feedbackItem");
    telegram->setOpenExternalLinks(true);

    QLabel *github = new QLabel(
        "GitHub: <a href=\"https://github.com/lanthora/cake/issues/new\" style=\"color: #4a90d9; text-decoration: none;\">Create "
        "Issue</a>",
        this);
    github->setObjectName("feedbackItem");
    github->setOpenExternalLinks(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(32, 24, 32, 24);
    layout->setSpacing(10);
    layout->addWidget(qq);
    layout->addWidget(telegram);
    layout->addWidget(github);
}
