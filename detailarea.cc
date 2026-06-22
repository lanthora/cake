#include "detailarea.h"
#include "candyitem.h"
#include "candy/candy.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

DetailArea::DetailArea()
{
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->hide();

    detailWidget = new QWidget;
    scrollArea->setWidget(detailWidget);

    QVBoxLayout *layout = new QVBoxLayout(detailWidget);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(10);

    selector = new QComboBox;
    selector->setEditable(true);
    selector->setInsertPolicy(QComboBox::NoInsert);
    selector->setCompleter(nullptr);
    selector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    selector->addItem("+ New");

    websocket = new QLineEdit;
    password = new QLineEdit;
    password->setEchoMode(QLineEdit::Password);
    tun = new QLineEdit;
    stun = new QLineEdit;
    port = new QLineEdit;
    discovery = new QLineEdit;
    route = new QLineEdit;
    localhost = new QLineEdit;
    mtu = new QLineEdit;
    removeButton = new QPushButton("Delete");
    saveButton = new QPushButton("Save");

    layout->addWidget(createInputRow("Network", selector));
    layout->addWidget(createInputRow("Server", websocket));
    layout->addWidget(createInputRow("Password", password));
    layout->addWidget(createInputRow("Address", tun));
    layout->addWidget(createInputRow("STUN", stun));
    layout->addWidget(createInputRow("Localhost", localhost));
    layout->addWidget(createInputRow("Port", port));
    layout->addWidget(createInputRow("Discovery (s)", discovery));
    layout->addWidget(createInputRow("Route Cost", route));
    layout->addWidget(createInputRow("MTU", mtu));
    layout->addStretch();

    connect(selector, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        if (text == "+ New") {
            reset(true);
        } else if (items.contains(text)) {
            selectItem(text);
        }
    });

    connect(saveButton, &QPushButton::clicked, this, &DetailArea::save);
    connect(removeButton, &QPushButton::clicked, this, &DetailArea::remove);

    outerLayout->addWidget(scrollArea);
    outerLayout->addWidget(createButtonRow());

    QSettings s;
    s.setFallbacksEnabled(false);
    if (s.value("vmac").isNull()) {
        s.setValue("vmac", QString::fromStdString(candy::create_vmac()));
        s.sync();
    }
    for (const QString &group : s.childGroups()) {
        CandyItem *item = new CandyItem(group);
        items[group] = item;
        selector->addItem(group);
        item->update();
    }

    if (!items.isEmpty()) {
        selectItem(items.firstKey());
    } else {
        reset(true);
    }
}

DetailArea::~DetailArea()
{
    for (auto item : items) {
        delete item;
    }
}

void DetailArea::selectItem(const QString &key)
{
    if (key.isEmpty() || !items.contains(key)) {
        return;
    }

    settings.beginGroup(key);
    websocket->setText(settings.value("websocket").toString());
    password->setText(settings.value("password").toString());
    tun->setText(settings.value("tun").toString());
    stun->setText(settings.value("stun").toString());
    port->setText(settings.value("port").toString());
    discovery->setText(settings.value("discovery").toString());
    route->setText(settings.value("route").toString());
    localhost->setText(settings.value("localhost").toString());
    mtu->setText(settings.value("mtu").toString());
    updateTitle(settings.value("expected").toString());
    settings.endGroup();

    removeButton->setEnabled(true);
    scrollArea->show();

    selector->setEditable(false);
    selector->blockSignals(true);
    selector->setCurrentText(key);
    selector->blockSignals(false);
}

void DetailArea::reset(bool fillDefault)
{
    selector->setEditable(true);

    websocket->setText("");
    password->setText("");
    tun->setText("");
    stun->setText("");
    port->setText("");
    discovery->setText("");
    route->setText("");
    localhost->setText("");
    mtu->setText("");

    if (fillDefault) {
        websocket->setText("wss://canets.org");
        stun->setText("stun://stun.canets.org");
        discovery->setText("300");
        route->setText("5");
        mtu->setText("1400");
        selector->setEditable(true);
        selector->blockSignals(true);
        selector->setCurrentText("candy");
        selector->blockSignals(false);
    } else {
        selector->clearEditText();
    }

    removeButton->setEnabled(false);
    updateTitle("New Network");

    scrollArea->show();
}

void DetailArea::save()
{
    const QString key = selector->currentText().trimmed();

    if (key.isEmpty()) {
        QMessageBox::warning(this, "", "Name cannot be empty");
        return;
    }
    if (websocket->text().isEmpty()) {
        QMessageBox::warning(this, "", "Server cannot be empty");
        return;
    }

    const bool isNew = !items.contains(key);

    if (isNew) {
        CandyItem *item = new CandyItem(key);
        items[key] = item;
        selector->addItem(key);
    }

    settings.beginGroup(key);
    settings.setValue("websocket", websocket->text());
    settings.setValue("password", password->text());
    settings.setValue("tun", tun->text());
    settings.setValue("stun", stun->text());
    settings.setValue("port", port->text());
    settings.setValue("discovery", discovery->text());
    settings.setValue("route", route->text());
    settings.setValue("localhost", localhost->text());
    settings.setValue("mtu", mtu->text());
    settings.endGroup();
    settings.sync();

    items[key]->update();
    selectItem(key);
}

void DetailArea::remove()
{
    const QString key = selector->currentText();

    if (key.isEmpty() || !items.contains(key)) {
        return;
    }

    removeButton->setEnabled(false);

    int idx = selector->findText(key);
    if (idx >= 0) {
        selector->blockSignals(true);
        selector->removeItem(idx);
        selector->blockSignals(false);
    }

    delete items.take(key);
    settings.remove(key);
    settings.sync();

    if (items.isEmpty()) {
        reset(true);
    } else {
        selectItem(items.firstKey());
    }
}

QWidget *DetailArea::createInputRow(const QString &label, QWidget *input)
{
    QWidget *row = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(row);
    hLayout->setContentsMargins(0, 4, 0, 4);
    QLabel *lbl = new QLabel(label);
    lbl->setMinimumWidth(110);
    lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hLayout->addWidget(lbl);
    hLayout->addWidget(input, 1);
    return row;
}

QWidget *DetailArea::createButtonRow()
{
    QWidget *row = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(row);
    hLayout->setContentsMargins(16, 8, 16, 8);
    hLayout->addStretch();
    hLayout->addWidget(removeButton);
    hLayout->addWidget(saveButton);
    return row;
}
