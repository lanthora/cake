#include "candyitem.h"
#include "candy.h"

CandyItem::CandyItem()
{
    setTextAlignment(Qt::AlignCenter);
    setSizeHint(QSize(0, 40));

    candy = candy_client_create();
}

CandyItem::~CandyItem()
{
    candy_client_release(candy);
}
