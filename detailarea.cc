#include "detailarea.h"
#include "candy/candy.h"
#include "candyitem.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QScrollArea>
#include <QStyledItemDelegate>
#include <QTimer>
#include <QVBoxLayout>

class DropDownBox : public QComboBox
{
public:
    using QComboBox::QComboBox;
    void showPopup() override
    {
        QComboBox::showPopup();
        QWidget *popup = findChild<QFrame *>();
        if (popup && popup->isVisible()) {
            QPoint pos = mapToGlobal(QPoint(0, height()));
            if (popup->y() != pos.y()) {
                popup->move(pos);
            }
        }
    }
};

class RoundedItemDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        if (option.state & (QStyle::State_MouseOver | QStyle::State_Selected)) {
            QPainterPath path;
            path.addRoundedRect(QRectF(option.rect).adjusted(3, 2, -3, -2), 4, 4);
            painter->setPen(Qt::NoPen);
            painter->fillPath(path, QColor("#e8f0fe"));
        }

        const QString text = index.data(Qt::DisplayRole).toString();
        painter->setPen(QColor("#222"));
        painter->setFont(option.font);
        painter->drawText(option.rect.adjusted(12, 0, -12, 0), Qt::AlignVCenter | Qt::AlignLeft, text);
        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        return QSize(100, 32);
    }
};

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

    QGridLayout *layout = new QGridLayout(detailWidget);
    layout->setContentsMargins(24, 16, 24, 16);
    layout->setHorizontalSpacing(24);
    layout->setVerticalSpacing(6);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);

    selector = new DropDownBox;
    selector->setMaxVisibleItems(8);
    selector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    selector->setItemDelegate(new RoundedItemDelegate(selector));

    newName = new QLineEdit;
    newName->setPlaceholderText("Enter network name...");

    networkStack = new QStackedWidget;
    networkStack->setFrameShape(QFrame::NoFrame);
    networkStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    networkStack->addWidget(selector);
    networkStack->addWidget(newName);

    ipAddress = new QLineEdit;
    ipAddress->setReadOnly(true);

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
    removeButton->setObjectName("dangerButton");
    saveButton = new QPushButton("Save");
    saveButton->setObjectName("primaryButton");
    cancelButton = new QPushButton("Cancel");
    cancelButton->setObjectName("dangerButton");
    cancelButton->hide();

    layout->addWidget(createInputRow("Name", networkStack), 0, 0);
    layout->addWidget(createInputRow("Network", ipAddress), 0, 1);
    layout->addWidget(createInputRow("Server", websocket), 1, 0);
    layout->addWidget(createInputRow("Password", password), 1, 1);
    layout->addWidget(createInputRow("Static Address", tun), 2, 0);
    layout->addWidget(createInputRow("STUN", stun), 2, 1);
    layout->addWidget(createInputRow("Localhost", localhost), 3, 0);
    layout->addWidget(createInputRow("Listen Port", port), 3, 1);
    layout->addWidget(createInputRow("Active Discovery Interval", discovery), 4, 0);
    layout->addWidget(createInputRow("Route Cost", route), 4, 1);
    layout->addWidget(createInputRow("MTU", mtu), 5, 0, 1, 2);
    layout->setRowStretch(6, 1);

    connect(selector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        if (idx < 0)
            return;
        QString text = selector->itemText(idx);
        if (items.contains(text)) {
            selectItem(text);
        }
    });

    connect(saveButton, &QPushButton::clicked, this, &DetailArea::save);
    connect(removeButton, &QPushButton::clicked, this, &DetailArea::remove);
    connect(cancelButton, &QPushButton::clicked, this, [this] {
        if (lastSelected.isEmpty()) {
            scrollArea->hide();
        } else {
            selectItem(lastSelected);
        }
    });

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
        cachedAddress[group] = "Connecting...";
    }

    if (!items.isEmpty()) {
        selectItem(items.firstKey());
    } else {
        reset(true);
        save();
    }

    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &DetailArea::pollStatus);
    statusTimer->start(1000);
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

    websocket->setPlaceholderText("");
    password->setPlaceholderText("");
    tun->setPlaceholderText("");
    stun->setPlaceholderText("");
    port->setPlaceholderText("");
    discovery->setPlaceholderText("");
    route->setPlaceholderText("");
    localhost->setPlaceholderText("");
    mtu->setPlaceholderText("");

    {
        auto status = candy::client::status(key.toStdString());
        if (status && status->has("address")) {
            QString addr = QString::fromStdString(status->getValue<std::string>("address"));
            if (!addr.isEmpty()) {
                if (settings.value("expected").toString() != addr) {
                    settings.setValue("expected", addr);
                    settings.sync();
                }
                cachedAddress[key] = addr;
                ipAddress->setText(addr);
                settings.endGroup();

                removeButton->setEnabled(true);
                cancelButton->hide();
                removeButton->show();
                scrollArea->show();

                networkStack->setCurrentWidget(selector);
                selector->blockSignals(true);
                selector->setCurrentText(key);
                selector->blockSignals(false);
                return;
            }
        }
        if (cachedAddress.contains(key)) {
            ipAddress->setText(cachedAddress[key]);
        } else {
            ipAddress->setText("Connecting...");
        }
    }
    settings.endGroup();

    removeButton->setEnabled(true);
    cancelButton->hide();
    removeButton->show();
    scrollArea->show();

    networkStack->setCurrentWidget(selector);
    selector->blockSignals(true);
    selector->setCurrentText(key);
    selector->blockSignals(false);
}

void DetailArea::reset(bool fillDefault)
{
    if (networkStack->currentWidget() == selector && !selector->currentText().isEmpty()) {
        lastSelected = selector->currentText();
    }

    websocket->setText("");
    password->setText("");
    tun->setText("");
    stun->setText("");
    port->setText("");
    discovery->setText("");
    route->setText("");
    localhost->setText("");
    mtu->setText("");

    websocket->setPlaceholderText("wss://server.example.com");
    password->setPlaceholderText("Enter password");
    tun->setPlaceholderText("10.0.0.1/24 (auto-assigned if empty)");
    stun->setPlaceholderText("stun://stun.example.com");
    port->setPlaceholderText("0");
    discovery->setPlaceholderText("300");
    route->setPlaceholderText("5");
    localhost->setPlaceholderText("Local peer address (auto-detect if empty)");
    mtu->setPlaceholderText("1400");

    networkStack->setCurrentWidget(newName);
    newName->setText("");

    if (fillDefault && items.isEmpty()) {
        websocket->setText("wss://canets.org");
        stun->setText("stun://stun.canets.org");
        discovery->setText("300");
        route->setText("5");
        mtu->setText("1400");
        newName->setText("candy");
    }

    ipAddress->clear();

    removeButton->setEnabled(false);
    removeButton->hide();
    cancelButton->setEnabled(!items.isEmpty());
    cancelButton->show();

    scrollArea->show();
}

void DetailArea::save()
{
    const bool isNew = networkStack->currentWidget() == newName;
    const QString key = (isNew ? newName->text() : selector->currentText()).trimmed();

    if (key.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setWindowTitle("Cake");
        msgBox.setText("Name cannot be empty");
        msgBox.exec();
        return;
    }
    if (websocket->text().isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setWindowTitle("Cake");
        msgBox.setText("Server cannot be empty");
        msgBox.exec();
        return;
    }

    if (isNew) {
        if (items.contains(key)) {
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::NoIcon);
            msgBox.setWindowTitle("Cake");
            msgBox.setText("Network name already exists");
            msgBox.exec();
            return;
        }
    }

    QString serverUrl = websocket->text().trimmed();
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it.key() == key)
            continue;
        settings.beginGroup(it.key());
        QString existing = settings.value("websocket").toString();
        settings.endGroup();
        if (existing == serverUrl) {
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::NoIcon);
            msgBox.setWindowTitle("Cake");
            msgBox.setText("Server already used by \"" + it.key() + "\"");
            msgBox.exec();
            return;
        }
    }

    if (isNew) {
        CandyItem *item = new CandyItem(key);
        items[key] = item;
        selector->blockSignals(true);
        selector->addItem(key);
        selector->blockSignals(false);
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
    cachedAddress.remove(key);
    ipAddress->setText("Connecting...");
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
    cachedAddress.remove(key);
    settings.remove(key);
    settings.sync();

    if (items.isEmpty()) {
        reset(true);
    } else {
        selectItem(items.firstKey());
    }
}

void DetailArea::pollStatus()
{
    bool connected = false;
    for (auto it = items.begin(); it != items.end(); ++it) {
        auto status = candy::client::status(it.value()->name().toStdString());
        if (status && status->has("address")) {
            QString addr = QString::fromStdString(status->getValue<std::string>("address"));
            if (addr.isEmpty())
                continue;
            cachedAddress[it.key()] = addr;
            settings.beginGroup(it.key());
            if (settings.value("expected").toString() != addr) {
                settings.setValue("expected", addr);
                settings.sync();
            }
            settings.endGroup();
            if (networkStack->currentWidget() == selector && selector->currentText() == it.key()) {
                ipAddress->setText(addr);
                connected = true;
            }
        }
    }
    statusTimer->setInterval(networkStack->currentWidget() == selector && connected ? 5000 : 1000);
}

QWidget *DetailArea::createInputRow(const QString &label, QWidget *input)
{
    QWidget *row = new QWidget;
    QVBoxLayout *vLayout = new QVBoxLayout(row);
    vLayout->setContentsMargins(0, 4, 0, 4);
    vLayout->setSpacing(4);
    QLabel *lbl = new QLabel(label);
    lbl->setIndent(4);
    lbl->setAlignment(Qt::AlignLeft);
    vLayout->addWidget(lbl);
    vLayout->addWidget(input);
    return row;
}

QWidget *DetailArea::createButtonRow()
{
    QWidget *row = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(row);
    hLayout->setContentsMargins(24, 12, 24, 12);
    hLayout->addStretch();
    hLayout->addWidget(cancelButton);
    hLayout->addWidget(removeButton);
    hLayout->addWidget(saveButton);
    return row;
}
