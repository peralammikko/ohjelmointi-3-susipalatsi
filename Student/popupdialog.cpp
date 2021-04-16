#include "popupdialog.hh"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(std::shared_ptr<Interface::Location> loc, int BV, CommonResource res, std::shared_ptr<Interface::Player> player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog),
    location_(loc),
    player_(player),
    locationBV_(BV),
    areaRes(res)
{
    ui->setupUi(this);
    ui->locationNameLabel->setText(location_->name());
    QString areaResName = RESOURCE_NAMES.at(areaRes);
    ui->areaResourceLabel->setText("2x " + areaResName);

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
