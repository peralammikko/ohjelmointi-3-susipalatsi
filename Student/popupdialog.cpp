#include "popupdialog.hh"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(LocationItem* &loc, std::shared_ptr<Interface::Player> &player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog),
    locItem(loc),
    player_(player)
{
    // Setting up the window UI and disabling the trade button
    ui->setupUi(this);
    ui->tradeButton->setDisabled(true);
    ui->canGetCardLabel->hide();

    // Setting up labels
    location_ = loc->getObject();
    locationBV_ = loc->getBasevalue();
    localRes_ = loc->getLocalResource();
    neededRes_ = loc->getDemandedResource();
    ui->locationNameLabel->setText(location_->name());
    ui->areaResourceLabel->setText(localRes_.name());
    ui->BVlabel->setText(QString::number(locationBV_));
    ui->councillorDemandsLabel->setText(neededRes_.name() + " x " + QString::number(neededRes_.amount()));

    // Calculating friendly and rival agents in location and sum of resource rewards
    std::vector<int> sumAndAgents = locItem->calculateRewards(player);
    int rewardSum = sumAndAgents.at(0);
    int ownAgents = sumAndAgents.at(1);
    int enemyAgents = sumAndAgents.at(2);
    ui->ownAgentsLabel->setText(QString::number(ownAgents));
    ui->enemyAgentsLabel->setText(QString::number(enemyAgents));
    ui->sumLabel->setText(QString::number(rewardSum));

    // Listing out all agents in location
    std::set<std::shared_ptr<Interface::AgentInterface>> listOfAgents = location_->agents();

    // Do these if there are any agents in area
    if (listOfAgents.size() > 0) {
        fillAreaAgentsList(listOfAgents);
        checkCouncilCard(listOfAgents);
    }
}

PopupDialog::~PopupDialog()
{
    delete ui;
}

void PopupDialog::checkCouncilCard(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere)
{
    // Getting the location's required resource
    int reqAmount = neededRes_.amount();
    int agentHas = 0;
    // Seek those agents (cards) from player that are in current location
    for (auto card : player_->cards()) {
        std::shared_ptr<Interface::Agent> agent = std::dynamic_pointer_cast<Interface::Agent>(card);

        // Potential agent found
        if (agent and agentsHere.find(agent) != agentsHere.end()) {
            std::shared_ptr<Interface::Location> demandLoc = neededRes_.location().lock();
            auto agentResources = agent->getAgentResources();

            if (!demandLoc) {
                qDebug() << "no go";
            } else {
                agentHas = agentResources.at(demandLoc).size();
            }
            if (agentHas >= reqAmount) {
                qDebug() << "enough stuff";
                ui->canGetCardLabel->setText("Trade available");
                ui->canGetCardLabel->setPalette(Qt::darkYellow);
                ui->canGetCardLabel->show();
                ui->tradeButton->setDisabled(false);

                // TO DO: Remove agent's resources and give councillor card
                // Also need to reroll another quest for location
                // signal to mainwindow?
                potentialAgent_ = agent;

                return;
            } else {
                qDebug() << "not enough stuff";
            }
        }
    }
}

void PopupDialog::fillAreaAgentsList(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere)
{
    for (auto agent : agentsHere) {
        std::shared_ptr<Interface::Player> agentOwner = agent->owner().lock();
        // WIP
        if (!agentOwner) {
            qDebug() << "owner not found";
        } else {
            ui->agentListWidget->addItem(agent->name());
        }
    }
}

void PopupDialog::on_tradeButton_clicked()
{
    // Work still in progress, probably gonna move to locationitem

    auto demandLoc = neededRes_.location().lock();
    int demandAmount = neededRes_.amount();
    if (demandLoc) {
        QString councName = "Mr. " + location_->name();
        std::shared_ptr<Interface::Councilor> counc = std::make_shared<Interface::Councilor>(councName, "Mestari", location_);

        if (potentialAgent_->addCouncilCard(counc)) {
            potentialAgent_->removeResource(demandLoc, demandAmount);
            ui->councillorDemandsLabel->clear();
            ui->canGetCardLabel->setText("Fame gained");
            locItem->generateNewDemand();
            ui->tradeButton->setDisabled(true);
        } else {
            ui->canGetCardLabel->setText("Card holder full!");
        }

    } else {
        qDebug() << "Card not found";
    }
}
