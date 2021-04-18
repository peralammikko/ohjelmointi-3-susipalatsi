#include "popupdialog.hh"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(LocationItem* &loc, std::shared_ptr<Interface::Player> &player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog),
    locItem(loc),
    player_(player)
{
    ui->setupUi(this);
    qDebug() << "popup created";
    location_ = loc->getObject();
    locationBV_ = loc->getBasevalue();
    localRes_ = loc->getLocalResource();
    neededRes_ = loc->getDemandedResource();
    ui->locationNameLabel->setText(location_->name());
    ui->areaResourceLabel->setText(localRes_.name());
    ui->BVlabel->setText(QString::number(locationBV_));
    ui->councillorDemandsLabel->setText(neededRes_.name() + " x " + QString::number(neededRes_.amount()));

    std::vector<int> sumAndAgents = locItem->calculateRewards(player);
    int rewardSum = sumAndAgents.at(0);
    int ownAgents = sumAndAgents.at(1);
    int enemyAgents = sumAndAgents.at(2);
    ui->ownAgentsLabel->setText(QString::number(ownAgents));
    ui->enemyAgentsLabel->setText(QString::number(enemyAgents));
    ui->sumLabel->setText(QString::number(rewardSum));

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
        // std::shared_ptr<Interface::Player> agentOwner = agent->owner().lock();
        ui->agentListWidget->addItem(agent->name());
    }
}
