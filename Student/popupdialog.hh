#ifndef POPUPDIALOG_HH
#define POPUPDIALOG_HH

#include <QDialog>
#include <QDebug>

#include "locationitem.hh"
#include "player.h"

namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(LocationItem* &loc, std::shared_ptr<Interface::Player> &player, QWidget *parent = 0);
    ~PopupDialog();
    /**
     * @brief fills dialog's agent list with agents currently residing there
     * @param agentsHere: List of agents in a locations
     * @pre Location has agents in it
     * @post All present agents and their respective owners are listed in the popup dialog
     */
    void fillAreaAgentsList(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere);
    /**
     * @brief Checks if any of current player's agents have enough resources to make trade for councilor card
     * @param agentsHere: List of agents in location
     * @pre Player in turn has agents in clicked locations
     * @post Trade-button is enabled and highlighted for active player if agents has enough demanded resources
     */
    void checkAgentResources(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere);

private slots:
    /**
     * @brief Attempts to trade location's councilor card for agent's resources. Checks first if
     * - player has enough influence on locations
     * - agent is carrying present councilor's card
     * - agent is carrying another councilor card at the moment
     * - player already has councilor's card
     * @pre Agent has enough resources, player has enough influence, player doesn't own said councilor's card and agent has space to carry card.
     * @post Agent is given a councilor card and agent's resources are deleted. A new resource demand is generated.
     */
    void on_tradeButton_clicked();

private:
    Ui::PopupDialog *ui;
    LocationItem* locItem = nullptr;
    std::shared_ptr<Interface::Location> location_ = nullptr;
    std::shared_ptr<Interface::Player> player_ = nullptr;
    int locationBV_ = 0;
    std::shared_ptr<Interface::CommonResource> localRes_ = nullptr;
    std::shared_ptr<Interface::CommonResource> neededRes_ = nullptr;
    std::shared_ptr<Interface::Agent> potentialAgent_ = nullptr;
};

#endif // POPUPDIALOG_HH
