#include "about.h"
#include "define.h"
#include <QLabel>
#include <QVBoxLayout>

About::About(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("关于");
    setFixedSize(500, 200);

    QLabel *label = new QLabel(
        "<a href=\"https://github.com/lanthora/cake\">Cake</a> "
        "是使用 Qt 并基于 "
        "<a href=\"https://github.com/lanthora/candy\">Candy</a> "
        "开发的桌面应用程序<br>"
        "当前版本: " CAKE_VERSION "<br>",
        this);

    label->setAlignment(Qt::AlignCenter);
    label->setOpenExternalLinks(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);
}
