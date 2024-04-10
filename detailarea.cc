#include "detailarea.h"
#include "candyitem.h"
#include "candylist.h"
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
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

void DetailArea::setCandyList(CandyList *candyList)
{
    this->candyList = candyList;

    if (candyList->count() == 0) {
        reset();
    }
}

void DetailArea::selectItem(QListWidgetItem *item)
{
    candyList->setCurrentItem(item);
    name->setEnabled(false);
    removeButton->setEnabled(true);
    detailWidget->show();

    name->setText(item->text());

    settings->beginGroup(item->text());
    websocket->setText(settings->value("websocket").toString());
    password->setText(settings->value("password").toString());
    tun->setText(settings->value("tun").toString());
    stun->setText(settings->value("stun").toString());
    port->setText(settings->value("port").toString());
    discovery->setText(settings->value("discovery").toString());
    route->setText(settings->value("route").toString());
    localhost->setText(settings->value("localhost").toString());
    settings->endGroup();

    return;
}

void DetailArea::reset()
{
    name->setText("");
    websocket->setText("");
    password->setText("");
    tun->setText("");
    stun->setText("");
    port->setText("");
    discovery->setText("");
    route->setText("");
    localhost->setText("");

    candyList->clearFocus();
    candyList->clearSelection();
    removeButton->setEnabled(false);
    name->setEnabled(true);
    detailWidget->show();
    return;
}

void DetailArea::save()
{
    if (name->text().isEmpty()) {
        QMessageBox::warning(this, "", "网卡名不能为空");
        return;
    }
    if (websocket->text().isEmpty()) {
        QMessageBox::warning(this, "", "服务端地址不能为空");
        return;
    }

    QListWidgetItem *item = nullptr;

    // 新增配置,确保网卡名不重复
    if (name->isEnabled()) {
        for (int idx = 0; idx < candyList->count(); ++idx) {
            if (candyList->item(idx)->text() == name->text()) {
                QMessageBox::warning(this, "", "网卡名已经存在,请重新设置");
                return;
            }
        }
        // TODO: 读出参数,根据网卡名更新配置,并重启对应的网络
        item = new CandyItem;
        item->setText((name->text()));
        candyList->addItem(item);
    } else {
        item = candyList->currentItem();
    }

    settings->beginGroup(item->text());
    settings->setValue("websocket", websocket->text());
    settings->setValue("password", password->text());
    settings->setValue("tun", tun->text());
    settings->setValue("stun", stun->text());
    settings->setValue("port", port->text());
    settings->setValue("discovery", discovery->text());
    settings->setValue("route", route->text());
    settings->setValue("localhost", localhost->text());
    settings->endGroup();

    selectItem(item);
    return;
}

void DetailArea::remove()
{
    removeButton->setEnabled(false);
    int row = candyList->currentRow();
    QListWidgetItem *item = candyList->takeItem(row);
    settings->remove(item->text());
    delete item;

    if (candyList->count() == 0) {
        reset();
    } else if (candyList->count() > row) {
        selectItem(candyList->item(row));
    } else {
        selectItem(candyList->item(candyList->count() - 1));
    }
    return;
}

QWidget *DetailArea::createInputWidget(QString key, QLineEdit *input)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QLabel *label = new QLabel;
    label->setText(key);
    label->setFixedWidth(110);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setContentsMargins(0, 0, 10, 0);
    input->setFixedWidth(400);
    layout->addWidget(label, 1);
    layout->addWidget(input, 5);
    widget->setFixedHeight(50);
    return widget;
}

QWidget *DetailArea::createSaveButton()
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);

    connect(removeButton, &QPushButton::clicked, this, &DetailArea::remove);
    layout->addWidget(removeButton);

    connect(saveButton, &QPushButton::clicked, this, &DetailArea::save);
    layout->addWidget(saveButton);

    return widget;
}
