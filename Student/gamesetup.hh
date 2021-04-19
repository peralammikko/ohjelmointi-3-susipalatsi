#ifndef GAMESETUP_HH
#define GAMESETUP_HH


#include "gamewindow.hh"
#include "ui_gamewindow.h"

// location item
#include "locationitem.hh"

// Agent interface and its class
#include "agent.hh"
#include "agentitem.hh"

// in constructor
#include "gamescene.hh"
#include "../Course/game.h"
#include "../Course/runner.h"

// reads settings
#include "../Course/settingsreader.h"

#include <cmath>

// Maybe do not need these?

#include "../Course/player.h"
#include "../Course/actioninterface.h"
#include "../Course/controlinterface.h"


class GameSetup
{
public:
    GameSetup(GameScene* gameScene, std::shared_ptr<Interface::Game> game, std::shared_ptr<GameRunner> courseRunner, std::shared_ptr<Logic> logic);
private:
    void initLocations();
    void initLocationDecks();
    void initResourceMaps();
    void initDemandMaps();
    void initLocItems();

    // TODO: logic needed in constructor? Logic needed to return?
    void initLogic();


    void initPlayers();
    void initPlayerHands();
    void initPlayerControls();

    void initAgentInterfaces();
    void initAgentItems();
    void sendAgentsToStartLocations();


    GameScene* gameScene_;
    std::shared_ptr<Interface::Game> game_;
    std::shared_ptr<GameRunner> courseRunner_;
    std::shared_ptr<Logic> logic_;

    ResourceMap initResourceMap_;
    ResourceMap councillorDemandsMap_;
    AgentResourceMap initAgentBackpack_;


};

#endif // GAMESETUP_HH
