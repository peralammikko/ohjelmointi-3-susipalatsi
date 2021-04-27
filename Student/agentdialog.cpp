#include <QDebug>

#include "agentdialog.hh"
#include "ui_agentdialog.h"

AgentDialog::AgentDialog(std::shared_ptr<Interface::Agent> agentClicked, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentDialog),
    agent_(agentClicked)
{
    ui->setupUi(this);

    // Spawn a frameless window
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->agentNameLabel->setText(agent_->name());
    std::shared_ptr<Interface::Location> agentLocation = agent_->placement().lock();
    if (!agentLocation) {
        ui->locationBox->setText("Mothership");
    } else {
        ui->locationBox->setText(agentLocation->name());
    }
    ui->locationBox->setReadOnly(true);
    listResources();
}

AgentDialog::~AgentDialog()
{
    delete ui;
}

void AgentDialog::listResources()
{
    // Lists out all resources an agent is carrying
    for (auto res : agent_->getAgentResources()) {
        int resAmount = res.second.size();
        if (resAmount > 0) {
            std::shared_ptr<Interface::CommonResource> resCard = res.second.at(0);
            QString resName = resCard->name();
            ui->resourceList->addItem(resName + ": " + QString::number(resAmount));
        }
    }
}

void AgentDialog::displayCouncilCard()
{
    // Display a council card if an agent has one
    if (agent_->hasCouncilCard()) {
        ui->cardBox->setText(agent_->getCouncilCard()->name());
    }
}
