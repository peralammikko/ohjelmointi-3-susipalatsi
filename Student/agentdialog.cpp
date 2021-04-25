#include <QDebug>

#include "agentdialog.hh"
#include "ui_agentdialog.h"

AgentDialog::AgentDialog(std::shared_ptr<Interface::Agent> agentClicked, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentDialog),
    agent_(agentClicked)
{
    ui->setupUi(this);

    // Frameless windows cool as HECK
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->agentNameLabel->setText(agent_->name());
    std::shared_ptr<Interface::Location> agentLocation = agent_->placement().lock();
    if (!agentLocation) {
        ui->locationBox->setText("Mother Ship");
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
    if (agent_->hasCouncilCard()) {
        ui->cardBox->setText(agent_->getCouncilCard()->name());
    }
}
