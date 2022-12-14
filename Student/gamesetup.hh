#ifndef GAMESETUP_HH
#define GAMESETUP_HH


#include "gamewindow.hh"
#include "ui_gamewindow.h"

// location item
#include "locationitem.hh"

// creating decks
#include "../Course/deck.h"
#include "actioncard.hh"


// Agent interface and its class
#include "agent.hh"
#include "agentitem.hh"

// in constructor
#include "gamescene.hh"
#include "../Course/game.h"
#include "../Course/runner.h"

// reads settings

#include <cmath>
#include "resourcedealer.hh"
// Maybe do not need these?

#include "aicontrol.hh"
#include "gamescene.hh"
#include "startingscreen.hh"
#include "../Course/settingsreader.h"

/**
 * @file
 * @brief Sets up most of the stuff for the game
 */

class GameSetup
{
public:

GameSetup(GameScene* gameScene, std::shared_ptr<Interface::Game> game, std::shared_ptr<Interface::Runner> courseRunner, std::shared_ptr<Logic> logic,
	      std::vector<QString> playerNames, std::vector<int> customSettings,int bots,  std::shared_ptr<ResourceDealer> resDealer);

private:
    void checkStartingInfo(std::vector<QString> playerNames, std::vector<int> customSettings);

    /**
     * @brief initLocations
     */
    void initLocations();

    /**
     * @brief initLocationDecks
     */
    void initLocationDecks();

    /**
     * @brief initResourceMaps
     */
    void initResourceMaps();

    /**
     * @brief initDemandMaps
     */
    void initDemandMaps();

    /**
     * @brief initLocItems
     */
    void initLocItems();

    /**
     * @brief initLocDecks
     */
    void initLocDecks();

    /**
     * @brief initSceneArrows
     */
    void initSceneArrows();

    void initResDealer();

    void initPlayers();
    void initPlayerHands();
    void addPlayerSetupCards();
    void initPlayerControls();

    void initAgentInterfaces();
    void initAgentItems();
    void sendAgentsToStartLocations();



    GameScene* gameScene_;
    std::shared_ptr<Interface::Game> game_;
    std::shared_ptr<Interface::Runner> courseRunner_;
    std::shared_ptr<Logic> logic_;

    ResourceMap initResourceMap_;
    ResourceMap councillorDemandsMap_;
    // std::map<std::shared_ptr<Interface::Location>, std::unique_ptr<Interface::CommonResource>> councillorDemandsMap_;
    AgentResourceMap initAgentBackpack_;

    std::shared_ptr<ResourceDealer> resDealer_;

    std::vector<QString> playerNames_ = {};

    unsigned int PLAYERCOUNT;
    unsigned int AGENTCOUNT;
    unsigned int LOCATIONS;
    unsigned int WINCONDITION;
    unsigned int BOTCOUNT;

};

#endif // GAMESETUP_HH
