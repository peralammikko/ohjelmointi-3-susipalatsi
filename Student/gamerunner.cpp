#include <QDebug>
#include "gamerunner.hh"

GameRunner::GameRunner(std::shared_ptr<Interface::Game> &game) : Interface::Runner(game)
{
    game_ = game;
}

void GameRunner::testDebug()
{
    qDebug() << "hello it's yo boy runner";
}

std::shared_ptr<Interface::Game> GameRunner::getGameboard()
{
    return game_;
}
