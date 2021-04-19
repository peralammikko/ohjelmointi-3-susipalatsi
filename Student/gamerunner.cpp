#include <QDebug>
#include "gamerunner.hh"

GameRunner::GameRunner(std::shared_ptr<Interface::Game> &game, GameScene* &scene, ResourceMap resMap) : Interface::Runner(game)
{
    game_ = game;
    scene_ = scene;
}

void GameRunner::testDebug()
{
    qDebug() << "running in the 90s";
}

std::shared_ptr<Interface::Game> GameRunner::getGameboard()
{
    return game_;
}

GameScene* GameRunner::getScene()
{
    return scene_;
}

/*
void GameRunner::setResourceMap(std::map<std::shared_ptr<Interface::Location>, Interface::CommonResource> resMap_)
{
    areaResources = resMap_;
}
*/


