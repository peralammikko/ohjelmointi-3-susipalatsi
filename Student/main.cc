#include "simplemainwindow.hh"
#include "game.cpp"
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

    // Alustetaan peliluokka
    std::shared_ptr<Interface::Game> gameboard(nullptr);
    gameboard = make_shared<Interface::Game>();

    // luodaan demonstroinniksi 3 pelaajaa
    for (int i=0; i<3; i++){
        // annettu merkkijono on vain pelaajanimi, sen voisi syöttää myöhemmin jotenkin
        gameboard->addPlayer("a");
    }
    std::vector<QString> paikat = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};

    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat.at(i));
        gameboard->addLocation(location);
    }

    SimpleMainWindow w;

    w.drawLocations(2, 3, paikat);

    // tämä kohta piirtää kortteja ja slotteja pöydälle. Ei vielä tarpeellinen.
    // w.drawSlots();        i;
    // w.drawCardToSlot();
    // SimpleCardItem* card = w.drawDeck(0, 1);

    w.show();

    return a.exec();
}
