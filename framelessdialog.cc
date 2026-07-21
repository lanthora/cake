#include "framelessdialog.h"
#include "titlebar.h"
#include <QVBoxLayout>

FramelessDialog::FramelessDialog(const QString &title, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setWindowTitle(title);

    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    titleBar = new TitleBar(title, this);
    outer->addWidget(titleBar);

    m_contentLayout = new QVBoxLayout;
    m_contentLayout->setContentsMargins(32, 24, 32, 24);
    m_contentLayout->setSpacing(8);
    outer->addLayout(m_contentLayout);
}

QVBoxLayout *FramelessDialog::contentLayout() const
{
    return m_contentLayout;
}
