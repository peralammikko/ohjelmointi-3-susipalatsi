#include "popupdialog.hh"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(std::shared_ptr<Interface::Location> loc, int BV, std::shared_ptr<Interface::Player> player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog),
    location_(loc),
    player_(player),
    locationBV_(BV)
{
    ui->setupUi(this);
    ui->locationNameLabel->setText(location_->name());

    fillAreaAgentsList();
}

PopupDialog::~PopupDialog()
{
    delete ui;
}

void PopupDialog::fillAreaAgentsList()
{
    std::set<std::shared_ptr<Interface::AgentInterface>> listOfAgents = location_->agents();
    std::shared_ptr<Interface::AgentInterface> AgentOwner = nullptr;
    for (auto agent : listOfAgents) {
        ui->agentListWidget->addItem(agent->name());
    }
}
