#include <QDebug>

#include "agentdialog.hh"
#include "ui_agentdialog.h"

AgentDialog::AgentDialog(std::shared_ptr<Interface::Agent> agentClicked, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentDialog),
    agent_(agentClicked)
{
    ui->setupUi(this);
    ui->agentNameLabel->setText(agent_->name());
    std::shared_ptr<Interface::Location> agentLocation = agent_->placement().lock();
    if (!agentLocation) {
        if (agentLocation != nullptr) {
            ui->locationBox->setText("Consumed by the void...");
        } else {
            ui->locationBox->setText("Home");
        }
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
    for (auto res : agent_->getResources()) {
        QString resName = res.second.name();
        int resAmount = res.second.amount();
        ui->resourceList->addItem(resName + ": " + QString::number(resAmount));
    }
}
