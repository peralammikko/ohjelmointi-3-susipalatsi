#include "settingsscreen.hh"
#include "ui_settingsscreen.h"

SettingsScreen::SettingsScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsScreen)
{
    ui->setupUi(this);

    ui->agentNumBox->setRange(1,4);
    ui->agentNumBox->setValue(3);

    ui->locNumBox->setRange(4,6);
    ui->locNumBox->setValue(6);

    ui->cardNumBox->setRange(3,5);
    ui->cardNumBox->setValue(3);

}

SettingsScreen::~SettingsScreen()
{
    delete ui;
}

void SettingsScreen::on_returnButton_clicked()
{
    this->close();
}

void SettingsScreen::on_saveButton_clicked()
{
    agentNum_ = ui->agentNumBox->value();
    locNum_ = ui->locNumBox->value();
    cardNum_ = ui->cardNumBox->value();

    emit sendInfo(agentNum_, locNum_, cardNum_);
    close();
}
