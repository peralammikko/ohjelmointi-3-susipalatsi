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
    void fillAreaAgentsList(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere);
    void checkAgentResources(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere);

private slots:
    void on_tradeButton_clicked();

private:
    Ui::PopupDialog *ui;
    LocationItem* locItem = nullptr;
    std::shared_ptr<Interface::Location> location_ = nullptr;
    std::shared_ptr<Interface::Player> player_ = nullptr;
    int locationBV_ = 0;
    Interface::CommonResource localRes_ = NULLRES;
    Interface::CommonResource neededRes_ = NULLRES;
    std::shared_ptr<Interface::Agent> potentialAgent_ = nullptr;
};

#endif // POPUPDIALOG_HH
