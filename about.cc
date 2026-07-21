#include "about.h"
#include "define.h"
#include <QLabel>
#include <QVBoxLayout>

About::About(QWidget *parent)
    : FramelessDialog("About", parent)
{
    resize(400, 220);
    setMinimumSize(360, 200);

    QLabel *title = new QLabel("Cake", this);
    title->setObjectName("aboutTitle");
    title->setAlignment(Qt::AlignCenter);

    QLabel *version = new QLabel("Version " CAKE_VERSION, this);
    version->setObjectName("aboutVersion");
    version->setAlignment(Qt::AlignCenter);

    QLabel *desc = new QLabel(
        "<a href=\"https://github.com/lanthora/cake\" style=\"color: #4a90d9; text-decoration: none;\">Cake</a> "
        "is a desktop application<br>built with Qt based on "
        "<a href=\"https://github.com/lanthora/candy\" style=\"color: #4a90d9; text-decoration: none;\">Candy</a>",
        this);
    desc->setObjectName("aboutDesc");
    desc->setAlignment(Qt::AlignCenter);
    desc->setOpenExternalLinks(true);
    desc->setWordWrap(true);

    QVBoxLayout *layout = contentLayout();
    layout->setContentsMargins(32, 24, 32, 24);
    layout->setSpacing(4);
    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(version);
    layout->addSpacing(16);
    layout->addWidget(desc);
    layout->addStretch();
}
