#ifndef AGENTDIALOG_HH
#define AGENTDIALOG_HH

#include <QDialog>
#include "agent.hh"

/**
 * @file
 * @brief  a popup dialog which shows where agent is and what it is carrying
 */

namespace Ui {
class AgentDialog;
}

namespace Interface {
class Agent;
}

class AgentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgentDialog(std::shared_ptr<Interface::Agent> agentClicked, QWidget *parent = nullptr);
    ~AgentDialog();
    /**
     * @brief Lists resources carried by the agent hovered on
     */
    void listResources();

    /**
     * @brief Displays the council card an agent is carrying (if any)
     */
    void displayCouncilCard();

private:
    Ui::AgentDialog *ui;
    std::shared_ptr<Interface::Agent> agent_;
};

#endif // AGENTDIALOG_HH
