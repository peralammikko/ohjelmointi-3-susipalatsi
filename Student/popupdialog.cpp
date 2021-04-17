#include "popupdialog.hh"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(std::shared_ptr<Interface::Location> loc, int BV, Interface::CommonResource res, std::shared_ptr<Interface::Player> player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog),
    location_(loc),
    player_(player),
    locationBV_(BV),
    areaRes(res)
{
    ui->setupUi(this);
    ui->locationNameLabel->setText(location_->name());
    areaResName = res.name();
    ui->areaResourceLabel->setText(QString::number(locationBV_) + "x " + areaResName);

    fillAreaAgentsList();
}

PopupDialog::~PopupDialog()
{
    delete ui;
}

void PopupDialog::fillAreaAgentsList()
{
    std::set<std::shared_ptr<Interface::AgentInterface>> listOfAgents = location_->agents();
    for (auto agent : listOfAgents) {
        ui->agentListWidget->addItem(agent->name());
    }
}
