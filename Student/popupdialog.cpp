#include "popupdialog.hh"
#include "ui_popupdialog.h"
#include "deck.h"

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
    location_ = loc->getObject();
    locationBV_ = loc->getBasevalue();
    localRes_ = loc->getLocalResource();
    neededRes_ = loc->getDemandedResource();
    std::shared_ptr<Interface::Location> demandLoc = neededRes_->location().lock();

    // Setting up labels

    // Resource information
    ui->areaResourceLabel->setText(localRes_->name());
    ui->deckSizeLabel->setText(QString::number(location_->deck()->size()));
    ui->BVlabel->setText(QString::number(locationBV_));
    ui->influenceNum->setText(QString::number(location_->influence(player_)));

    // Councillor & location information
    ui->councillorDemandsLabel->setText(neededRes_->name() + " x " + QString::number(neededRes_->amount()));
    ui->councillorNameLabel->setText(location_->councilor()->name());
    ui->demandLocLabel->setText("(in " + demandLoc->name() + ")");
    ui->councillorCardText->setText("Councillor card: \n"
                                    + location_->councilor()->name());
    auto satan = QPixmap(loc->getLocalResource()->getSpritePath());


    ui->localResLbl->setPixmap(QPixmap(loc->getLocalResource()->getSpritePath()).scaled(ui->localResLbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->demandSpriteLbl->setPixmap(QPixmap(loc->getDemandedResource()->getSpritePath()).scaled(ui->demandSpriteLbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->councilorLbl->setPixmap((*loc->governorPixmap()).scaled(ui->councilorLbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );

    //ui->deman->setPixmap(QPixmap(loc->getDemandedResource().getSpritePath()));
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
        checkAgentResources(listOfAgents);
    }
}

PopupDialog::~PopupDialog()
{

    delete ui;
}

void PopupDialog::checkAgentResources(std::set<std::shared_ptr<Interface::AgentInterface>> &agentsHere)
{
    // Getting the location's required resource
    int reqAmount = neededRes_->amount();
    int agentHas = 0;
    // Seek those agents (cards) from player that are in current location
    for (auto card : player_->cards()) {
        std::shared_ptr<Interface::Agent> agent = std::dynamic_pointer_cast<Interface::Agent>(card);

        // Potential agent found
        if (agent and agentsHere.find(agent) != agentsHere.end()) {
            std::shared_ptr<Interface::Location> demandLoc = neededRes_->location().lock();
            auto agentResources = agent->getAgentResources();

            if (!demandLoc) {
                qDebug() << "no go";
            } else {
                agentHas = agentResources.at(demandLoc).size();
            }

            // Agent has enough resources for the trade
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
    auto demandLoc = neededRes_->location().lock();
    int demandAmount = neededRes_->amount();
    if (location_->influence(player_) < 5) {
        ui->canGetCardLabel->setText("Not enough influence \n"
                                     "on councillor yet");
        return;
    } else if (potentialAgent_->getCouncilCard() == location_->councilor()) {
        ui->canGetCardLabel->setText("You are already endorsed by councilor");
    } else {
        if (demandLoc) {
            if (potentialAgent_->addCouncilCard(location_->councilor())) {
                potentialAgent_->removeResource(demandLoc, demandAmount);
                ui->councillorDemandsLabel->clear();
                ui->canGetCardLabel->setText("The councillor endorses you");
                locItem->generateNewDemand();
                ui->tradeButton->setDisabled(true);
            } else {
                ui->canGetCardLabel->setText("Agent is already carrying \n"
                                             "another councillor card");
            }

        } else {
            qDebug() << "Card not found";
        }
    }
}
