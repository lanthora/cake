#include "titlebar.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>

TitleBar::TitleBar(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    setObjectName("titleBar");
    setFixedHeight(38);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 0, 8, 0);
    layout->setSpacing(0);

    titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("titleBarText");
    layout->addWidget(titleLabel);
    layout->addStretch();

    auto *closeButton = new QPushButton(QString(QChar(0x00D7)), this);
    closeButton->setObjectName("titleBarCloseButton");
    closeButton->setFixedSize(44, 28);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setFocusPolicy(Qt::NoFocus);
    connect(closeButton, &QPushButton::clicked, this, [this] {
        if (auto *w = window()) {
            w->close();
        }
    });
    layout->addWidget(closeButton);
}

void TitleBar::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && window() != nullptr) {
        dragPosition = event->globalPosition().toPoint() - window()->frameGeometry().topLeft();
        event->accept();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && window() != nullptr) {
        window()->move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}
