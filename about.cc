#include "about.h"
#include "define.h"
#include <QLabel>
#include <QVBoxLayout>

About::About(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("About");
    resize(500, 200);
    setMinimumSize(400, 160);

    QLabel *label = new QLabel(
        "<a href=\"https://github.com/lanthora/cake\">Cake</a> "
        "is a desktop application built with Qt based on "
        "<a href=\"https://github.com/lanthora/candy\">Candy</a>"
        "<br>Version: " CAKE_VERSION "<br>",
        this);

    label->setAlignment(Qt::AlignCenter);
    label->setOpenExternalLinks(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);
}
