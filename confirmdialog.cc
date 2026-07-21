#include "confirmdialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ConfirmDialog::ConfirmDialog(const QString &title, const QString &text, QWidget *parent)
    : FramelessDialog(title, parent)
{
    resize(420, 200);
    setMinimumSize(380, 180);

    QLabel *textLabel = new QLabel(text, this);
    textLabel->setWordWrap(true);

    buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    QVBoxLayout *layout = contentLayout();
    layout->setContentsMargins(32, 20, 32, 20);
    layout->setSpacing(12);
    layout->addWidget(textLabel);
    layout->addStretch();
    layout->addLayout(buttonLayout);
}

void ConfirmDialog::setConfirmButton(const QString &text, ButtonStyle style)
{
    QPushButton *button = new QPushButton(text, this);
    button->setObjectName(style == DangerButton ? "dangerButton" : "primaryButton");
    button->setMinimumWidth(88);
    button->setAutoDefault(true);
    button->setDefault(true);
    connect(button, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(button);
}

void ConfirmDialog::setCancelButton(const QString &text)
{
    QPushButton *button = new QPushButton(text, this);
    button->setMinimumWidth(88);
    button->setAutoDefault(false);
    connect(button, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->insertWidget(buttonLayout->count() - 1, button);
}
