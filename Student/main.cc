#include "simplemainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(default_assets);
    SimpleMainWindow w;
    w.show();

    return a.exec();
}
