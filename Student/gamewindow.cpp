#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

#include "gamesetup.hh"

#include "actioncard.hh"
#include "playerhand.hh"
#include "../Course/manualcontrol.h"
#include "commonresource.hh"
#include "startingscreen.hh"


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui_(new Ui::GameWindow)
{
    gameui_->setupUi(this);
    StartingScreen* start = new StartingScreen(this);
    connect(start, &StartingScreen::sendStartingInfo, this, &GameWindow::getStartingInfo);
    int ret = start->exec();
    if (ret == QDialog::Rejected) {
        this->~GameWindow();
        close();
    }

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
    gameui_->graphicsView->setFixedSize(1410, 810);
    gameScene_->setSceneRect(0,0,1400,800);
    auto background = QPixmap(":/img/background/background.png");
    QBrush brushBackground = QBrush(background);
    gameui_->graphicsView->setBackgroundBrush(brushBackground);

    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    logic_ = std::make_shared<Logic>(courseRunner, game_);

    resDealer_ = std::make_shared<ResourceDealer>(gameScene_, game_);
    connect(logic_.get(), &Logic::readyToRewardResources, resDealer_.get(), &ResourceDealer::onReadyForResources);

    connect(game_.get(), &Interface::Game::playerChanged, this, &GameWindow::onPlayerChanged);
    connect(courseRunner.get(), &Interface::Runner::actionPerformed, this, &GameWindow::onActionPerformed);
    connect(this, &GameWindow::actionDeclared, logic_.get(), &Logic::onActionDeclared);
    connect(logic_.get(), &Logic::enteredEventPhase, this, &GameWindow::onEnteringEventPhase);
    connect(gameTime_.get(), SIGNAL(timeout()), gameScene_, SLOT(advance()));
    connect(logic_.get(), &Logic::requestInterphase, this, &GameWindow::onInterphaseRequested);
    GameSetup setup = GameSetup(gameScene_, game_, courseRunner,  logic_, playerNames_, gameSettings_, bots_, resDealer_);

    displayPlayerStats();
    startingDialog();
}

GameWindow::~GameWindow()
{
    delete gameui_;
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

void GameWindow::listInfluence(std::shared_ptr<Interface::Player> &currentPlayer)
{
    // int id = currentPlayer->id();
    gameui_->influenceList->clear();
    for (auto loc : game_->locations()) {
        int playerInf = loc->influence(currentPlayer);
        gameui_->influenceList->addItem(loc->name() + ": " + QString::number(playerInf));
    }
}

void GameWindow::listCouncilCards()
{
    gameui_->councilCardBoard->clear();
    std::set<std::shared_ptr<Interface::Councilor>> cardVector;
    for (auto pl : game_->players()) {
        cardVector = {};
        for (auto card : pl->cards()) {
            std::shared_ptr<Interface::Councilor> councilCard = std::dynamic_pointer_cast<Interface::Councilor>(card);
            if (councilCard) {
                cardVector.insert(councilCard);
            }
        }
        gameui_->councilCardBoard->addItem(pl->name() + ": " + QString::number(cardVector.size()) + " / " + QString::number(winCondition));
    }
}

void GameWindow::startingDialog()
{
    QFile *textfile = new QFile("");
    infoBox_ = new QMessageBox();
    if (textfile->open(QIODevice::ReadOnly) == true)
    {
        infoBox_->setText(QString (textfile->readAll()));
        textfile->close();
    }
    QPixmap pic = QPixmap(":/img/img/some sprites/spacegoon.png");
    infoBox_->setWindowTitle("Introduction");
    infoBox_->setIconPixmap(pic);
    infoBox_->setStyleSheet("background-image: url(:/img/background/background.png); border: none; font-family: Console; color: white;");
    infoBox_->show();
}

void GameWindow::displayPlayerStats() {

    std::shared_ptr<Interface::Player> currentPlayer = game_->currentPlayer();
    gameui_->currentRoundLabel->setText("Current round: " + QString::number(current_round));

    gameui_->playerNameLabel->setText(currentPlayer->name());
    listAgents(currentPlayer);
    listInfluence(currentPlayer);
}

void GameWindow::on_passButton_clicked()
{
    if (!game_->active()){
        return;
    }
    // TODO: move to logic where player hand is emptied of all action cards
    qDebug() << "Pass button was clicked. TODO: inform logic";
    auto hand = gameScene_->playerHands().at(game_->currentPlayer());
    gameScene_->resetAction();

    emit actionDeclared(std::make_shared<PassAction>(hand));

    auto player = game_->currentPlayer();
}

void GameWindow::onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action)
{
    QString history = "";
    auto agentaction = std::dynamic_pointer_cast<AgentActionInterface>(action);

    history += QString::number(gameui_->actionHistoryWidget->count()+1) + " " + player->name() + " " +agentaction->pastTenseDescription();
    gameui_->actionHistoryWidget->addItem(history);
    gameui_->actionHistoryWidget->scrollToBottom();
}

void GameWindow::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{
    displayPlayerStats();
    listCouncilCards();
}

void GameWindow::onEnteringEventPhase()
{
    gameui_->actionHistoryWidget->addItem(QString::number(gameui_->actionHistoryWidget->count()+1) + "==ROUND OVER==");
    gameui_->actionHistoryWidget->addItem("Councilors gather to discuss who is fit to rule Susipalatsi");
    // TODO: Fix the logic victory check
    if (false){
        gameui_->actionHistoryWidget->addItem("Councilors have now decided!");
    } else {
        gameui_->actionHistoryWidget->addItem("Councilors did not decide the winner yet");
        gameui_->actionHistoryWidget->addItem("Cosmic Forces change the order of planets!");
    }
    gameui_->actionHistoryWidget->scrollToBottom();
}

void GameWindow::getStartingInfo(std::vector<QString> playerNames, std::vector<int> gameSettings, int bots)
{
    playerNames_ = playerNames;
    gameSettings_ = gameSettings;
    bots_ = bots;

    winCondition = 3;
    /*
    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
<<<<<<< Student/gamewindow.cpp
    reader.setPath(":/settings/defaultsettings.dat");
=======
    reader.setPath("");  // ???
>>>>>>> Student/gamewindow.cpp
    reader.readSettings();
    qDebug() << "settings read";
    if (gameSettings.size() == 0) {
        winCondition = reader.getValue("WINCONDITION").toInt();
    } else {
        winCondition = gameSettings.at(2);
    }
    */
}


void GameWindow::on_helpButton_clicked()
{

    QFile *textfile = new QFile("");
    infoBox_ = new QMessageBox();
    if (textfile->open(QIODevice::ReadOnly) == true)
    {
        infoBox_->setText(QString (textfile->readAll()));
        textfile->close();
    }
    QPixmap pic = QPixmap(":/img/img/some sprites/spacegoon.png");
    infoBox_->setWindowTitle("How to play");
    infoBox_->setIconPixmap(pic);
    infoBox_->setStyleSheet("background-image: url(:/img/background/background.png); border: none; font-family: Console; color: white;");
    infoBox_->exec();
}

void GameWindow::onInterphaseTimeout()
{
    gameui_->graphicsView->setMouseTracking(true);
    gameui_->graphicsView->setEnabled(true);
}

void GameWindow::onInterphaseRequested(int time = 1500)
{
    if (interphaseTimer_ != nullptr){
        delete interphaseTimer_;
    }
    interphaseTimer_ = new QTimer(this);
    interphaseTimer_->setSingleShot(true);
    interphaseTimer_->start(time);
   // gameui_->graphicsView->setMouseTracking(false);
    gameui_->graphicsView->setEnabled(false);
    connect(interphaseTimer_, &QTimer::timeout, this, &GameWindow::onInterphaseTimeout);
    connect(interphaseTimer_, &QTimer::timeout, logic_.get(), &Logic::onInterphaseTimeout);
}
