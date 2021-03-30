#include "simplemainwindow.hh"
#include "game.cpp"
#include <QtDebug>
#include <string>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(default_assets);

    // Alustetaan peliluokka
    std::shared_ptr<Interface::Game> game(nullptr);
    game = make_shared<Interface::Game>();

    // luodaan demonstroinniksi 3 pelaajaa
    for (int i=0; i<3; i++){
        // annettu merkkijono on vain pelaajanimi, sen voisi syöttää myöhemmin jotenkin
        game->addPlayer("a");
    }

    SimpleMainWindow w;

    // tämä kohta piirtää kortteja ja slotteja pöydälle. Ei vielä tarpeellinen.
    // w.drawSlots();        i;
    // w.drawCardToSlot();
    // SimpleCardItem* card = w.drawDeck(0, 1);

    w.show();

    return a.exec();
}
