#include "simplemainwindow.hh"
#include "game.h"
#include <QtDebug>
#include <string>
#include "gamewindow.hh"

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

    return a.exec();
}
