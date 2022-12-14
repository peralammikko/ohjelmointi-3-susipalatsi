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

    courseRunner = std::make_shared<Interface::Runner>(game_);

    // Tell the game to start listening to the timer
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
    connect(logic_.get(), &Logic::onWinnersFound, this, &GameWindow::onWinnersFound);
    connect(game_.get(), &Interface::Game::playerChanged, this, &GameWindow::onPlayerChanged);
    connect(courseRunner.get(), &Interface::Runner::actionPerformed, this, &GameWindow::onActionPerformed);
    connect(this, &GameWindow::actionDeclared, logic_.get(), &Logic::onActionDeclared);
    connect(logic_.get(), &Logic::enteredEventPhase, this, &GameWindow::onEnteringEventPhase);
    connect(gameTime_.get(), SIGNAL(timeout()), gameScene_, SLOT(advance()));
    connect(logic_.get(), &Logic::requestInterphase, this, &GameWindow::onInterphaseRequested);
    GameSetup setup = GameSetup(gameScene_, game_, courseRunner,  logic_, playerNames_, gameSettings_, bots_, resDealer_);

    displayPlayerStats();
    startingDialog();
    /*
    std::shared_ptr<Interface::Player> p1 = std::make_shared<Interface::Player>(game_,8,"Makke");
    winners_.insert(p1);
    gameoverDialog();
    */
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
                gameui_->agentListWidget->addItem(agentCard->name() + " @ Mother Base");
            }
        }
    }
}

void GameWindow::listInfluence(std::shared_ptr<Interface::Player> &currentPlayer)
{
    gameui_->influenceList->clear();
    for (auto loc : game_->locations()) {
        int playerInf = loc->influence(currentPlayer);
        if (playerInf < 5) {
            gameui_->influenceList->addItem(loc->name() + ": " + QString::number(playerInf));
        } else {
            gameui_->influenceList->addItem(loc->name() + ": 5");
        }
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

void GameWindow::displayPlayerStats() {

    std::shared_ptr<Interface::Player> currentPlayer = game_->currentPlayer();

    gameui_->playerNameLabel->setText(currentPlayer->name());
    listAgents(currentPlayer);
    listInfluence(currentPlayer);
}

void GameWindow::on_passButton_clicked()
{
    if (!game_->active()){
        // prevents pressing pass during pauses (such as interphases)
        return;
    }
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
    Q_UNUSED(actingPlayer);
    displayPlayerStats();
    listCouncilCards();
}

void GameWindow::onEnteringEventPhase()
{
    gameui_->actionHistoryWidget->addItem(QString::number(gameui_->actionHistoryWidget->count()+1) + "==ROUND OVER==");
    gameui_->actionHistoryWidget->addItem("Councilors gather to discuss who is fit to rule Susipalatsi");
    if (gameOver_){
        gameui_->actionHistoryWidget->addItem("Councilors have now decided! Glory be to the new ruler of Susipalatsi!");
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

    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
    reader.setPath(":/settings/defaultsettings.dat");
    reader.readSettings();
    if (gameSettings.size() == 0) {
        winCondition = reader.getValue("WINCONDITION").toInt();
    } else {
        winCondition = gameSettings.at(2);
    }
}


void GameWindow::on_helpButton_clicked()
{

    QFile *textfile = new QFile(":/textfiles/helpText");
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

void GameWindow::startingDialog()
{
    QFile *textfile = new QFile(":/textfiles/startingText");
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

void GameWindow::gameoverDialog(std::set<std::shared_ptr<Interface::Player>> winners)
{
    infoBox_ = new QMessageBox();
    infoBox_->setWindowTitle("Game finished!");
    infoBox_->setStyleSheet("background-image: url(:/img/background/background.png); border: none; font-family: Console; color: white;");
    QString winnerNames = "";
    for (auto i : winners) {
        winnerNames += i->name() + " ";
    }
    infoBox_->setText("The council has finally come to a conclusion. \n"
                      "The next ruler(s) of our realm will be... \n"
                      "\n" +
                      winnerNames);
    infoBox_->exec();

    QFile *textfile = new QFile(":/textfiles/winningText");
    if (textfile->open(QIODevice::ReadOnly) == true) {
        infoBox_->setText(QString (textfile->readLine(0)));
        textfile->close();
    }
    QPixmap pic = QPixmap(":/img/img/some sprites/partygoon.png");
    infoBox_->setIconPixmap(pic);
    infoBox_->exec();

    infoBox_->setText("Or maybe the real SUSIPALATSI was the friends we made along the way... \n"
                          "\n"
                          "Thanks for playing!");
    infoBox_->setIconPixmap(pic);
    infoBox_->exec();
}

void GameWindow::onInterphaseTimeout()
{
    gameui_->graphicsView->setMouseTracking(true);
    gameui_->graphicsView->setEnabled(true);
}

void GameWindow::onInterphaseRequested(int time = 1300)
{
    if (gameOver_){
        return;
    }
    if (interphaseTimer_ != nullptr){
        delete interphaseTimer_;
    }
    interphaseTimer_ = new QTimer(this);
    interphaseTimer_->setSingleShot(true);
    interphaseTimer_->start(time);

    gameui_->graphicsView->setEnabled(false);
    connect(interphaseTimer_, &QTimer::timeout, this, &GameWindow::onInterphaseTimeout);
    connect(interphaseTimer_, &QTimer::timeout, logic_.get(), &Logic::onInterphaseTimeout);
}

void GameWindow::onWinnersFound(std::set<std::shared_ptr<Interface::Player>> winners)
{
    gameoverDialog(winners);
    gameui_->graphicsView->setEnabled(false);
    if (interphaseTimer_ != nullptr){
        disconnect(interphaseTimer_, &QTimer::timeout, this, &GameWindow::onInterphaseTimeout);
        disconnect(interphaseTimer_, &QTimer::timeout, logic_.get(), &Logic::onInterphaseTimeout);
        interphaseTimer_->stop();
    }
    game_->setActive(false);
    gameOver_ = true;
}
