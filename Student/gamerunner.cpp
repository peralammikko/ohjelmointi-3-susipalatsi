#include <QDebug>
#include "gamerunner.hh"

GameRunner::GameRunner(std::shared_ptr<Interface::Game> &game, GameScene* &scene) : Interface::Runner(game)
{
    game_ = game;
    scene_ = scene;
}

void GameRunner::testDebug()
{
    qDebug() << "hello it's yo boy runner";
}

std::shared_ptr<Interface::Game> GameRunner::getGameboard()
{
    return game_;
}

GameScene* GameRunner::getScene()
{
    return scene_;
}
