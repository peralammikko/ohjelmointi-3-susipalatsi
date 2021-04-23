#include "startingscreen.hh"
#include "ui_startingscreen.h"
#include "settingsscreen.hh"
#include <QTimer>

StartingScreen::StartingScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartingScreen)
{
    settings = new SettingsScreen(this);
    ui->setupUi(this);
    ui->nameFrame->hide();
    ui->errorLabel->hide();
    connect(settings, &SettingsScreen::sendInfo, this, &StartingScreen::getSettings);

    ui->namesLayout->setAlignment(Qt::AlignTop);

    qlist.push_back(ui->nameLine1);
    qlist.push_back(ui->nameLine2);
}

StartingScreen::~StartingScreen()
{
    delete ui;
}

void StartingScreen::on_newGameButton_clicked()
{
    ui->nameFrame->show();
}

void StartingScreen::on_settingsButton_clicked()
{
    settings->exec();
}

void StartingScreen::on_quitButton_clicked()
{
    reject();
}

void StartingScreen::on_addPlayerBtn_clicked()
{
    if (qlist.count() < 5) {
        QLineEdit *lineEd = new QLineEdit;
        qlist.push_back(lineEd);
        ui->namesLayout->addWidget(lineEd);

        // lineEd->setObjectName("nameLine"+QString::number(qlist.count()));
    } else {
        ui->errorLabel->setText("Maximum player capacity reached");
        ui->errorLabel->show();
        QTimer::singleShot(4000, ui->errorLabel, &QLabel::hide);

    }
}

void StartingScreen::on_removePlayerBtn_clicked()
{
    QWidget *child;
    if (qlist.count() > 2) {
        if ((child = ui->namesLayout->takeAt(qlist.count()-1)->widget()) != 0 ) {
            delete child;
            qlist.removeLast();
        }
    } else {
        ui->errorLabel->setText("Two players required");
        ui->errorLabel->show();
        QTimer::singleShot(4000, ui->errorLabel, &QLabel::hide);
    }
}

void StartingScreen::getSettings(int agentCount, int locCount, int winCardCount)
{
    customSettings.push_back(agentCount);
    customSettings.push_back(locCount);
    customSettings.push_back(winCardCount);

}

void StartingScreen::on_pushButton_clicked()
{
    for (auto line : qlist) {
        QString name = line->text().trimmed();
        if (name.length() > 10) {
            ui->errorLabel->setText("Name(s) must contain less than 10 characters");
            ui->errorLabel->show();
            QTimer::singleShot(4000, ui->errorLabel, &QLabel::hide);
            playerNames.clear();
        } else {
            playerNames.push_back(name);
        }
    }
    emit sendStartingInfo(playerNames, customSettings);
    accept();
}
