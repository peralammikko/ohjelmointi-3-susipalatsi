#include "simplemainwindow.hh"
#include "game.h"
#include <QtDebug>
#include <string>

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

    SimpleMainWindow w;

    // tämä kohta piirtää kortteja ja slotteja pöydälle. Ei vielä tarpeellinen.
    // w.drawSlots();        i;
    // w.drawCardToSlot();
    // SimpleCardItem* card = w.drawDeck(0, 1);

    w.show();

    return a.exec();
}
