#ifndef GAMERUNNER_HH
#define GAMERUNNER_HH

#include <QObject>

#include "gamescene.hh"
#include "../Course/runner.h"
#include "commonresource.hh"

class GameRunner : public Interface::Runner
{
public:
    GameRunner(std::shared_ptr<Interface::Game> &game, GameScene *&scene, ResourceMap resMap);
    void testDebug();
    std::shared_ptr<Interface::Game> getGameboard();
    GameScene* getScene();
    void getResourceMap();

private:
    std::shared_ptr<Interface::Game> game_ = nullptr;
    std::map<std::shared_ptr<const Interface::Player>, std::shared_ptr<Interface::ControlInterface> > playerControls_;
    GameScene* scene_ = nullptr;
    const std::map<std::shared_ptr<Interface::Location>, Interface::CommonResource> resourceMap_;

    
};

#endif // GAMERUNNER_HH
