#include "mapitem.hh"
#include "location.h"
#include "gamewindow.hh"

#include <QDebug>

void mapItem::goHome(int time)
{
    setPos(homeCoordinatesOnScene_);
}
