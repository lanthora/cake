#include "detailarea.h"
#include "candylist.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

DetailArea::DetailArea()
{
    detailWidget = new QWidget(this);
    detailWidget->hide();
    QVBoxLayout *layout = new QVBoxLayout(detailWidget);

    password->setEchoMode(QLineEdit::Password);

    layout->addWidget(createInputWidget("网卡名", name));
    layout->addWidget(createInputWidget("服务器地址", websocket));
    layout->addWidget(createInputWidget("口令", password));
    layout->addWidget(createInputWidget("静态地址", tun));
    layout->addWidget(createInputWidget("STUN", stun));
    layout->addWidget(createInputWidget("对等连接端口", port));
    layout->addWidget(createInputWidget("主动发现间隔", discovery));
    layout->addWidget(createInputWidget("本机路由代价", route));
    layout->addWidget(createInputWidget("本机内网地址", localhost));
    layout->addWidget(createSaveButton());
}

void DetailArea::update(QListWidgetItem *item)
{
    detailWidget->show();
    if (item == CandyList::addButton) {
        return;
    }

    name->setReadOnly(true);
}

void DetailArea::save()
{
    // TODO: 读出参数,根据网卡名更新配置,并重启对应的网络
    return;
}

QWidget *DetailArea::createInputWidget(QString key, QLineEdit *input)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QLabel *label = new QLabel;
    label->setText(key);
    label->setFixedWidth(110);
    label->setAlignment(Qt::AlignRight);
    label->setContentsMargins(0, 0, 10, 0);
    input->setFixedWidth(400);
    layout->addWidget(label, 1);
    layout->addWidget(input, 5);
    return widget;
}

QWidget *DetailArea::createSaveButton()
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QPushButton *saveButton = new QPushButton("保存");
    saveButton->setFixedWidth(150);
    connect(saveButton, &QPushButton::clicked, this, &DetailArea::save);
    layout->addWidget(saveButton);
    return widget;
}
