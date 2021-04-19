#ifndef GAMERUNNER_HH
#define GAMERUNNER_HH

#include <QObject>

#include "gamescene.hh"
#include "../Course/runner.h"
#include "commonresource.hh"

class GameRunner : public Interface::Runner
{
public:
    GameRunner(std::shared_ptr<Interface::Game> game);
    void testDebug();
    std::shared_ptr<Interface::Game> getGameboard();

private:
    std::shared_ptr<Interface::Game> game_ = nullptr;
    std::map<std::shared_ptr<const Interface::Player>, std::shared_ptr<Interface::ControlInterface> > playerControls_;
};

#endif // GAMERUNNER_HH
