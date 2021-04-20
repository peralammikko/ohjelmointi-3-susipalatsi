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

    courseRunner = std::make_shared<GameRunner>(game_);

    // Tell the game to start listening to the timer
    // TODO: move this after settings are selected or something
    gameTime_ =  std::make_unique<QTimer>(this);
    connect(gameTime_.get(), SIGNAL(timeout()), gameScene_, SLOT(advance()));
    gameTime_->start(50);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui_->graphicsView->setFixedSize(1400, 800);
    gameScene_->setSceneRect(0,0,1400,800);
    this->setFixedSize(1450, 950);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");


    logic_ = std::make_shared<Logic>(courseRunner, game_, gameScene_);
    GameSetup setup = GameSetup(gameScene_, game_, courseRunner,  logic_);

    // displayPlayerStats();
}

GameWindow::~GameWindow()
{
    delete gameui_;
}


void GameWindow::drawItem(mapItem *item)
{
    gameScene_->drawItem(item);
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return game_->locations();
}

void GameWindow::listAgents(std::shared_ptr<Interface::Player> &player)
{
    gameui_->agentListWidget->clear();
    auto listOfAgents = playerAgentItems_.at(player);
    for (auto agent : listOfAgents) {
        std::shared_ptr<Interface::Agent> agentPtr = agent->getAgentClass();
        std::shared_ptr<Interface::Location> loc = agent->getAgentClass()->location().lock();
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

/*
void GameWindow::displayPlayerStats()

    // This is temporarily broken
    Mikko's part
    current_round++;
    gameui_->currentRoundLabel->setText("Current round: " + QString::number(current_round));
    gameui_->playerNameLabel->setText(playerInTurn->name());
    gameui_->playerCoinsLabel->setText(QString::number(playerWallets_.at(playerInTurn)));

    listAgents(playerInTurn);
}
*/

/*
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
*/

void GameWindow::on_passButton_clicked()
{
    // TODO: move to logic where player hand is emptied of all action cards
    qDebug() << "Pass button was clicked. TODO: inform logic";
    logic_->rewardResources();
    //changeTurn();
}
