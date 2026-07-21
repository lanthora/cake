#include "feedback.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

static QWidget *createRow(QLabel *label, QLabel *value)
{
    QWidget *row = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(row);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);
    layout->addWidget(label);
    layout->addStretch();
    layout->addWidget(value);
    return row;
}

Feedback::Feedback(QWidget *parent)
    : FramelessDialog("Feedback", parent)
{
    resize(420, 260);
    setMinimumSize(380, 220);

    QLabel *subtitle = new QLabel("Have questions or feedback? Reach out to us through any of these channels.", this);
    subtitle->setObjectName("dialogSubtitle");
    subtitle->setWordWrap(true);

    QLabel *qqLabel = new QLabel("QQ Group", this);
    qqLabel->setObjectName("feedbackLabel");
    QLabel *qqValue = new QLabel("768305206", this);
    qqValue->setObjectName("feedbackValue");
    qqValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QWidget *qqRow = createRow(qqLabel, qqValue);

    QLabel *telegramLabel = new QLabel("Telegram", this);
    telegramLabel->setObjectName("feedbackLabel");
    QLabel *telegramValue = new QLabel(
        "<a href=\"https://t.me/CandyUserGroup\" style=\"color: #4a90d9; text-decoration: none;\">Join Group</a>", this);
    telegramValue->setObjectName("feedbackValue");
    telegramValue->setOpenExternalLinks(true);
    QWidget *telegramRow = createRow(telegramLabel, telegramValue);

    QLabel *githubLabel = new QLabel("GitHub", this);
    githubLabel->setObjectName("feedbackLabel");
    QLabel *githubValue = new QLabel(
        "<a href=\"https://github.com/lanthora/cake/issues/new\" style=\"color: #4a90d9; text-decoration: none;\">Create "
        "Issue</a>",
        this);
    githubValue->setObjectName("feedbackValue");
    githubValue->setOpenExternalLinks(true);
    QWidget *githubRow = createRow(githubLabel, githubValue);

    QVBoxLayout *layout = contentLayout();
    layout->setContentsMargins(32, 24, 32, 24);
    layout->setSpacing(8);
    layout->addWidget(subtitle);
    layout->addSpacing(8);
    layout->addWidget(qqRow);
    layout->addWidget(telegramRow);
    layout->addWidget(githubRow);
    layout->addStretch();
}
