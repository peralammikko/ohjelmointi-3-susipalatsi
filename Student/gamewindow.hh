#ifndef GAMEWINDOW_HH
#define GAMEWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "gamescene.hh"
#include "game.h"
#include "playerhand.hh"
#include "councilor.h"
#include "controlinterface.h"
#include "agentactioninterface.hh"
#include "passaction.hh"

// Logic testing
#include "logic.hh"

#include "../Course/game.h"
#include "../Course/runner.h"

#include "commonresource.hh"
#include "gamerunner.hh"
#include "random.h"

#include <QTimer>

namespace Ui {
    class GameWindow;

}

class Logic;
class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    /**
     * @brief getLocations: palauttaa pelin sijainnit pointtereina
     * @return vektori jaetuista pointereista pelin sijainteihin
     */
    const std::vector<std::shared_ptr<Interface::Location>> getLocations();

    /**
     * @brief setSize:
     * @param width:
     * @param height:
     */
    void setSize(int width, int height); // Poistoon?

    /**
     * @brief drawItem
     * @param item
     */
    void drawItem(mapItem* item); // Poistoon?
    /**
     * @brief showHand
     */
    void showHand(); // Poistoon?

    /**
     * @brief Päivittää ruudulla olevat tiedot vuorossa olevalle pelaajalle
     * @pre Pelissä on pelaajia ja vuoro vaihtuu seuraavalle
     */
    void displayPlayerStats();

    /**
     * @brief Listaa nykyisen pelaajan agentit sijainteineen sivupaneeliin
     * @param currentPlayer: vuorossa oleva pelaaja
     * @pre
     */
    void listAgents(std::shared_ptr<Interface::Player> &currentPlayer);

    /**
     * @brief Listaa nykyisen pelaaja vaikutusvallan sijainteihin sivupaneeliin
     * @param currentPlayer: vuorossa oleva pelaaja
     */
    void listInfluence(std::shared_ptr<Interface::Player> &currentPlayer);

    /**
     * @brief Listaa kaikkien pelaajien "pisteet" neuvoston jäsenten korttien lukumäärän mukaan
     */
    void listCouncilCards();

    /**
     * @brief Antaa 1. pelaajalle lyhyen tietoiskun pelin päämäärästä ja tavoitteista
     */
    void startingDialog();


private slots:
    /**
     * @brief Päättää pelaajan kierroksen
     * @pre Klikkaava pelaaja on vuorossa
     * @post Pelaajan kierros päättyy ja käsikortit hävitetään
     */
    void on_passButton_clicked();


    /**
     * @brief Esittää vuorossa olleen pelaajan viimeisimmän tapahtuman sivupaneelin History-ikkunaan
     * @param player: vuorossa ollut pelaaja
     * @param action: pelaajan toteuttama toiminto
     */
    void onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action);

    /**
     * @brief Ottaa vastaan tiedon siitä, että vuoro on vaihtunut
     * @param actingPlayer: edellisen kierroksen pelaaja
     */
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);


    /**
     * @brief Esittää History-ikkunaan tiedon siitä, että event-vaihe on tapahtunut kierroksen päätteeksi
     */
    void onEnteringEventPhase();

    /**
     * @brief Ottaa vastaan aloitusruudusta tulevat asetukset ja tiedot
     * @param playerNames: pelaajien nimet
     * @param gameSettings: pelaajan valitsemat asetukset peliin
     * @param bots: pelaajan asettama tekoälypelaajien määrä
     * @pre Aloitusikkuna on avattu ja peli käynnistetty sieltä
     * @post Alustaa vektorit pelin pystyttämistä varten
     */
    void getStartingInfo(std::vector<QString> playerNames, std::vector<int> gameSettings, int bots);

signals:
    /**
     * @brief Lähettää tiedon pelaajan tekemästä toiminnosta
     * @param action: pelaajan tekemä toiminto (tässä luokassa kierroksen ohitus eli Pass)
     */
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);
private:
    Ui::GameWindow *gameui_;
    GameScene *gameScene_;
    std::shared_ptr<Interface::Game> game_ = nullptr;
    std::shared_ptr<GameRunner> courseRunner = nullptr;

    // Logic testing
    std::shared_ptr<Logic> logic_;

    std::unique_ptr<QTimer> gameTime_;

    // Usefulness to be decided.
    std::map<std::shared_ptr<Interface::Player>, std::vector<agentItem*>> playerAgentItems_;

    // this variable stores drag and drop targe, ie. what is "under" a draggable card
    mapItem* targetedMapItem_;

    int current_round = 1;

    std::vector<QString> playerNames_ = {};
    std::vector<int> gameSettings_ = {};
    int bots_ = 0;
    int winCondition = 3;

    QDialog* helpDialog_;

};

#endif // GAMEWINDOW_HH
