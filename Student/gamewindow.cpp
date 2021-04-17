#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

// kokeilu käden asettelulle
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>

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

    courseRunner = std::make_shared<GameRunner>(game_);
    // logic class testing

    gameScene_ = new GameScene(gameui_->graphicsView, game_);

    gameui_->graphicsView->setScene(gameScene_);
    gameui_->graphicsView->setMouseTracking(true);


    // Tell the game to start listening to the timer
    // TODO: move this after settings are selected or something
    gameTime_ = new QTimer(this);

    connect(gameTime_, SIGNAL(timeout()), gameScene_, SLOT(advance()));

    gameTime_->start(100);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui_->graphicsView->setFixedSize(1400, 900);
    // gameScene_->setSceneRect(-600,600,-350,350);
    this->setFixedSize(1920, 1080);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    // Luodaan location-oliot ja resurssit
    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        game_->addLocation(location);
    }

    initAreaResources();
    gameScene_->resourceInfo(initResourceMap_);
    std::vector<std::shared_ptr<Interface::Location>> locvec = game_->locations();
    gameScene_->drawLocations(locvec);
    initCouncillorDemands();


    // Adding test players
    player1 = std::make_shared<Interface::Player>(game_, 1, "RED");
    player2 = std::make_shared<Interface::Player>(game_, 2, "BLUE");
    game_->addPlayer(player1);
    game_->addPlayer(player2);

    // Setting up game runner
    courseRunner = std::make_shared<GameRunner>(game_, gameScene_, initResourceMap_);
    initPlayerControls();

    setupPlayerStash();
    for (int i = 0 ; i < 3; i++) {
        spawnAgent(player1);
    }  
    for (int i = 0; i < 5; i++) {
        spawnAgent(player2);
    }

    if (current_round == 0) {
        drawPlayerAgents(player1);
    } else {
        drawPlayerAgents(player2);
    }

    // luodaan pelaajille käsialueen luokka
    for (unsigned int i=0; i<game_->players().size(); ++i) {
        std::shared_ptr<Interface::Player> pl = game_->players().at(i);

        // Luodaan pari korttia ja annetaan ne pelaajalle
        for (int j=0; j<4; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            pl->addCard(card);
        }
    }
    gameScene_->createHandCards(game_->players().at(0)->cards());

    playerInTurn = player1;
    displayPlayerStats();
}

GameWindow::~GameWindow()
{
    delete gameui_;
}

void GameWindow::drawPlayerAgents(std::shared_ptr<Interface::Player> &player)
 {
    std::vector<agentItem*> agents = playerAgentItems_.at(player);
    gameScene_->drawAgents(agents);
 }

void GameWindow::drawItem(mapItem *item)
{
    gameScene_->drawItem(item);
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return game_->locations();
}

void GameWindow::spawnAgent(std::shared_ptr<Interface::Player> &player)
{
    // Create agent interface, which holds all of the data of the card.
    // Ideally we want this to be handled by carddata class, which would use xml/JSON later on
    // For now we will just use some default generated stuff
    QString agname{"Perry"};
    std::shared_ptr<Interface::Agent> agentptr = std::make_shared<Interface::Agent>(agname + player->name(), player);
    agentptr->initAgentResources(initAgentBackpack_);

    agentItem* agenttiesine = new agentItem(agentptr);
    connect(agenttiesine, &agentItem::actionDeclared, gameScene_, &GameScene::onActionDeclared);

    gameScene_->addItem(agenttiesine);
    //agenttiesine->setParent(gameScene_);

    playerAgentItems_.at(player).push_back(agenttiesine);
}

std::shared_ptr<Interface::Player> GameWindow::getPlayerInTurn()
{
    return playerInTurn;
}

void GameWindow::changeTurn()
{
    if (current_round % 2 == 0) {
        playerInTurn = player1;
    } else {
        playerInTurn = player2;
    }

    displayPlayerStats();
    rewardResources();

    gameScene_->turnInfo(current_round, playerInTurn);

}

void GameWindow::listAgents(std::shared_ptr<Interface::Player> player)
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
    for (auto player : game_->players()) {

        // Map for agents each player has to use (empty on initialization)
        playerAgentItems_.insert({player, {}});

        // Map for players and their in-game currenty (2 coins on initialization)
        playerWallets_.insert({player, 2});

        // Map for players and their collected councilor cards (empty on initialization)
        councilorCards_.insert({player, {}});
    }
}

void GameWindow::displayPlayerStats()
{
    current_round++;
    gameui_->currentRoundLabel->setText("Current round: " + QString::number(current_round));
    gameui_->playerNameLabel->setText(playerInTurn->name());
    gameui_->playerCoinsLabel->setText(QString::number(playerWallets_.at(playerInTurn)));

    listAgents(playerInTurn);
}

void GameWindow::initAreaResources()
{
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

void GameWindow::rewardResources()
{
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
    ResourceMap::iterator it;
    for (auto pair : initResourceMap_) {
        auto location = pair.first;
        auto res = pair.second;

        while (true) {
            it = initResourceMap_.begin();
            int num = Interface::Random::RANDOM.uint(5);
            std::advance(it, num);
            if (it->first != location) {
                auto res = it->second;
                Interface::CommonResource demand(res.name(), location, num);
                councillorDemandsMap_.insert({location, demand});
                break;
            }
        }
    }
    for (auto i : councillorDemandsMap_) {
        qDebug() << i.first->name() << ": " << i.second.name() << " x" << i.second.amount();
    }
}

void GameWindow::calculateRewards()
{

}

void GameWindow::on_passButton_clicked()
{
    changeTurn();
}
