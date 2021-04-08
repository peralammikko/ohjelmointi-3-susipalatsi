#include "simplemainwindow.hh"
#include "game.h"
#include <QtDebug>
#include <string>
#include "gamewindow.hh"
#include "agentcard.hh"

#include <QApplication>

enum PAIKKANIMET {
    Marketti,
    Kirkko,
    Taverna,
    Kauppiaiden_kilta,
    Menomesta,
    Salapaikka
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(default_assets);

    GameWindow win;
    win.show();
    /*
    std::shared_ptr<agentCard> testiagentti = nullptr;
    std::shared_ptr<agentCard> makke(testiagentti);

    mapItem* clicked = mapItem::clickedLocation;
    std::shared_ptr<Interface::Location> selectedLoc = clicked->getObject();
    selectedLoc->sendAgent(ageMake);
    win.drawAgents(clicked);
    */

    return a.exec();
}
