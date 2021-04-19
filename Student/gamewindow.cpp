#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

// Kokeilu gamesetup
#include "gamesetup.hh"

#include "actioncard.hh"
#include "playerhand.hh"
#include "../Course/manualcontrol.h"
#include "commonresource.hh"


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui_(new Ui::GameWindow)
{
    gameui_->setupUi(this);

    // Declare the game first before gameScene, so we can give game_ to gameScene's constructor
    game_ = std::make_shared<Interface::Game>();
    game_->setActive(true);



    gameScene_ = new GameScene(gameui_->graphicsView, game_);
    gameui_->graphicsView->setScene(gameScene_);
    gameui_->graphicsView->setMouseTracking(true);

    courseRunner = std::make_shared<GameRunner>(game_, gameScene_, initResourceMap_);

    // Tell the game to start listening to the timer
    // TODO: move this after settings are selected or something
    gameTime_ = new QTimer(this);
    connect(gameTime_, SIGNAL(timeout()), gameScene_, SLOT(advance()));
    gameTime_->start(50);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui_->graphicsView->setFixedSize(1400, 800);
    gameScene_->setSceneRect(0,0,1400,800);
    this->setFixedSize(1450, 950);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    // TODO - MERGE: Siirretään nää gamesetuppiin
    initAreaResources();
    initCouncillorDemands();
    gameScene_->resourceInfo(initResourceMap_, councillorDemandsMap_);
    std::vector<std::shared_ptr<Interface::Location>> locvec = game_->locations();

    // TODO: move logic and gamerunner init into gamesetup somehow
    logic_ = std::make_shared<Logic>(courseRunner, game_, gameScene_);
    // GameSetup is only called here, and should be cleared after getting out of context
    GameSetup* setup = new GameSetup(gameScene_, game_, courseRunner,  logic_);
    delete setup;

    // This is a hardcorded card generation and it does NOT draw from decks or anything.
    // It can be here until we get reward system in order
    for (unsigned int i=0; i<game_->players().size(); ++i) {
        std::shared_ptr<Interface::Player> pl = game_->players().at(i);
        for (int j=0; j<4; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            pl->addCard(card);

           // gameScene_->playerHands().at(pl)->addMapItem()
        }
    }
    // gameScene_->turnInfo(current_round, playerInTurn);
    displayPlayerStats();
}

GameWindow::~GameWindow()
{
    delete gameui_;
}

/*
void GameWindow::drawPlayerAgents(std::shared_ptr<Interface::Player> &player)
 {
    std::vector<agentItem*> agents = playerAgentItems_.at(player);
    gameScene_->drawAgents(agents);
 }*/

void GameWindow::drawItem(mapItem *item)
{
    gameScene_->drawItem(item);
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return game_->locations();
}

/*
std::shared_ptr<Interface::Player> GameWindow::getPlayerInTurn()
{
    return playerInTurn;
}*/


void GameWindow::changeTurn()
{
    // FIX THIS TO LOGIC
    qDebug() << "GAMEWINDOW CHANGETURN - NEEDS FIXING!!";
    /*
    game_->nextPlayer();
    displayPlayerStats();
    rewardResources();

    gameScene_->turnInfo(current_round, playerInTurn);

    //current_round += 1;
    //gameScene_->turnInfo(current_round, game_->currentPlayer());
    gameScene_->turnInfo(0, game_->currentPlayer());*/

}

void GameWindow::listAgents(std::shared_ptr<Interface::Player> &player)
{
    gameui_->agentListWidget->clear();
    auto listOfAgents = playerAgentItems_.at(player);
    for (auto agent : listOfAgents) {
        std::shared_ptr<Interface::Agent> agentPtr = agent->getAgentClass();
        std::shared_ptr<Interface::Location> loc = agent->getObject()->location().lock();
        if (!loc) {
            gameui_->agentListWidget->addItem(agentPtr->name() + " @ Home");
        } else {
            QString agentAt = loc->name();
            gameui_->agentListWidget->addItem(agentPtr->name() + " @ " + agentAt);
        }
    }
}

void GameWindow::setupPlayerStash()
{
    qDebug() << "Setup Player stash: Needs to be moved to gamesetup";
    for (auto player : game_->players()) {

        // Map for agents each player has to use (empty on initialization)
        playerAgentItems_.insert({player, {}});

        // Map for players and their in-game currenty (2 coins on initialization)
        playerWallets_.insert({player, 2});
    }
}

void GameWindow::displayPlayerStats()
{
     // This is temporarily broken

    //<<<<<<< Student/gamewindow.cpp
    /* Mikko's part
    current_round++;
    gameui_->currentRoundLabel->setText("Current round: " + QString::number(current_round));
    gameui_->playerNameLabel->setText(playerInTurn->name());
    gameui_->playerCoinsLabel->setText(QString::number(playerWallets_.at(playerInTurn)));

    listAgents(playerInTurn);
    */
    //=======
   
    //current_round++;
    //gameui_->currentRoundLabel->setText("Current round: " + QString::number(game_));
    gameui_->playerNameLabel->setText(game_->currentPlayer()->name());
    //gameui_->playerCoinsLabel->setText(QString::number(playerWallets_.at(game_->currentPlayer())));
    //gameui_->councillorNumberLabel->setText(QString::number(councilorCards_.at(game_->currentPlayer()).size()) + " / 6");
    //listAgents(game_->currentPlayer());
}

void GameWindow::initAreaResources()
{  
    qDebug() << "initAreaResources - TODO move to gamesetup";
    for (auto loc : game_->locations()) {
        QString resName = loc->name() + " item";
        Interface::CommonResource res(resName, loc, 0);
        // Resource map for locations & runners
        std::pair<std::shared_ptr<Interface::Location>, Interface::CommonResource> pair(loc, res);
        initResourceMap_.insert(pair);

        // Resource map for agents
        std::pair<std::shared_ptr<Interface::Location>, std::deque<Interface::CommonResource>> pair2;
        pair2.first = loc;
        // pair2.second.push_back(res);
        initAgentBackpack_.insert(pair2);
    }
}

/*
void GameWindow::initPlayerControls()
{
    std::shared_ptr<Interface::ManualControl> mancontrol = std::make_shared<Interface::ManualControl>();
    courseRunner->setPlayerControl(player1, mancontrol);
    courseRunner->setPlayerControl(player2, mancontrol);

    // LOGIC SIGNALING TESTING
    // You need to use get() to makes shared_ptr to a regular ptr
    // Connect Logic class with gamescene in order to do any actions
    logic_ = std::make_shared<Logic>(courseRunner);
    connect(gameScene_, &GameScene::actionDeclared, logic_.get(), &Logic::actionSelected);
    logic_->doTheRunning();
    courseRunner->testDebug();
}
*/

void GameWindow::rewardResources()
{
    qDebug() << "Reward Resources  in gamewindow - TODO: maybe move to logic";
    for (auto pair : playerAgentItems_) {
        for (auto agent : pair.second) {
            auto agentPtr = agent->getAgentClass();
            std::shared_ptr<Interface::Location> agentAt = agentPtr->placement().lock();
            if (!agentAt) {
                qDebug() << agentPtr->name() << " not in any location";
            } else {
                Interface::CommonResource res = initResourceMap_.at(agentAt);
                if (agentAt != nullptr) {
                    agentPtr->addResource(agentAt, res, 1);
                } else {
                    qDebug() << "who am I where am I";
                }
            }
        }
    }
}

void GameWindow::initCouncillorDemands()
{
    qDebug() << "initCouncillorDemands in gamewindow: TODO - Move to gamesetup";
    ResourceMap::iterator it;
    for (auto pair : initResourceMap_) {
        auto location = pair.first;
        auto res = pair.second;

        // Make it so that location's demands can not be it's own resource
        while (true) {
            it = initResourceMap_.begin();
            int num = Interface::Random::RANDOM.uint(5);
            std::advance(it, num);
            if (it->first != location) {
                res = it->second;
                int amount = 2 + Interface::Random::RANDOM.uint(3);
                Interface::CommonResource demand(res.name(), it->first, amount);
                councillorDemandsMap_.insert({location, demand});
                break;
            }
        }
    }
    for (auto i : councillorDemandsMap_) {
        qDebug() << i.first->name() << ": " << i.second.name() << " x" << i.second.amount();
    }
}

void GameWindow::on_passButton_clicked()
{
    // TODO: move to logic where player hand is emptied of all action cards
    qDebug() << "Pass button was clicked. TODO: inform logic";
    //changeTurn();
}
