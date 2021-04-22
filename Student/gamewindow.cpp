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

    gameTime_->start(50);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui_->graphicsView->setFixedSize(1400, 800);
    gameScene_->setSceneRect(0,0,1400,800);
   // this->setFixedSize(1450, 950);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    logic_ = std::make_shared<Logic>(courseRunner, game_, gameScene_);
    GameSetup setup = GameSetup(gameScene_, game_, courseRunner,  logic_);
    connect(game_.get(), &Interface::Game::playerChanged, this, &GameWindow::onPlayerChanged);
    connect(courseRunner.get(), &Interface::Runner::actionPerformed, this, &GameWindow::onActionPerformed);
    connect(logic_.get(), &Logic::enteredEventPhase, this, &GameWindow::onEnteringEventPhase);
    connect(gameTime_.get(), SIGNAL(timeout()), gameScene_, SLOT(advance()));

    displayPlayerStats();
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

void GameWindow::listAgents(std::shared_ptr<Interface::Player> &currentPlayer)
{
    gameui_->agentListWidget->clear();
    for (auto card : currentPlayer->cards()) {
        std::shared_ptr<Interface::Agent> agentCard = std::dynamic_pointer_cast<Interface::Agent>(card);
        if (agentCard) {
            std::shared_ptr<Interface::Location> agentAt = agentCard->placement().lock();
            if (agentAt != nullptr) {
                gameui_->agentListWidget->addItem(agentCard->name() + " @ " + agentAt->name());
            } else {
                gameui_->agentListWidget->addItem(agentCard->name() + " @ Base");
            }
        }
    }
}

void GameWindow::displayPlayerStats() {

    // This is temporarily broken
    std::shared_ptr<Interface::Player> currentPlayer = game_->currentPlayer();
    gameui_->currentRoundLabel->setText("Current round: " + QString::number(current_round));

    gameui_->playerNameLabel->setText(currentPlayer->name());
    listAgents(currentPlayer);
}

void GameWindow::on_passButton_clicked()
{
    // TODO: move to logic where player hand is emptied of all action cards
    qDebug() << "Pass button was clicked. TODO: inform logic";
    auto player = game_->currentPlayer();
    //logic_->rewardResources();
    //changeTurn();
}

void GameWindow::onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action)
{
    QString history = "";
    auto agentaction = std::dynamic_pointer_cast<AgentActionInterface>(action);

    history += QString::number(gameui_->actionHistoryWidget->count()+1) + " " + player->name() + " " +agentaction->pastTenseDescription();
    gameui_->actionHistoryWidget->addItem(history);
}

void GameWindow::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{
    displayPlayerStats();
}

void GameWindow::onEnteringEventPhase()
{
    gameui_->actionHistoryWidget->addItem(QString::number(gameui_->actionHistoryWidget->count()+1) + "==EVENT PHASE==");
}
