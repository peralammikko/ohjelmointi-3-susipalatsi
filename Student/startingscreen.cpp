#include "startingscreen.hh"
#include "ui_startingscreen.h"
#include "settingsscreen.hh"
#include <QTimer>
#include <QDebug>
#include <QMessageBox>

StartingScreen::StartingScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartingScreen)
{
    settings = new SettingsScreen(this);
    ui->setupUi(this);
    const QPixmap logo = QPixmap(":/img/background/logo.png");

    ui->nameFrame->hide();
    ui->errorLabel->hide();
    ui->cpuFrame->hide();
    connect(settings, &SettingsScreen::sendInfo, this, &StartingScreen::getSettings);
    this->setWindowTitle("Susipalatsi");
    ui->logoLabel->setPixmap(logo.scaled(ui->logoLabel->width(),ui->logoLabel->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QString styleSheet("background-image:url(:/img/background/background.png);");
    this->setStyleSheet(styleSheet);

    ui->namesLayout->setAlignment(Qt::AlignTop);
    ui->cpuLayout->setAlignment(Qt::AlignTop);
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
    if (qlist.count()+botPlayers < 5) {
        QLineEdit *lineEd = new QLineEdit;
        lineEd->setStyleSheet("color: white");
        // lineEd->setFont(consoleFont);
        qlist.push_back(lineEd);
        ui->namesLayout->addWidget(lineEd);

    } else {
        ui->errorLabel->setText("Maximum player capacity reached (5)");
        ui->errorLabel->show();
        QTimer::singleShot(4000, ui->errorLabel, &QLabel::hide);

    }
}

void StartingScreen::on_removePlayerBtn_clicked()
{
    QWidget *child;
    if (qlist.count() > 1 and botPlayers > 0) {
        if ((child = ui->namesLayout->takeAt(qlist.count()-1)->widget()) != 0 ) {
            delete child;
            qlist.removeLast();
        }
    } else if (qlist.count() > 2 and botPlayers == 0) {
        if ((child = ui->namesLayout->takeAt(qlist.count()-1)->widget()) != 0 ) {
            delete child;
            qlist.removeLast();
        }
    } else {
        ui->errorLabel->setText("At least two players required");
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

void StartingScreen::on_startButton_clicked()
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
    emit sendStartingInfo(playerNames, customSettings, botPlayers);
    accept();
}

void StartingScreen::on_closeFrameButton_clicked()
{
    ui->nameFrame->close();
}

void StartingScreen::on_aiCheckbox_stateChanged(int arg1)
{
    if (arg1) {
        ui->cpuFrame->show();
    } else {
        int botCount = botPlayers;
        for (int i = 0; i < botCount; i++) {
            on_removeCPUbtn_clicked();
        }
        ui->cpuFrame->hide();
    }
}

void StartingScreen::on_addCPUbtn_clicked()
{
    if (botPlayers + qlist.count() < 5) {
        botPlayers++;
        QLineEdit *lineEd = new QLineEdit;
        lineEd->setStyleSheet("color: white");
        lineEd->setText("CPU"+QString::number(botPlayers));
        lineEd->setReadOnly(true);
        ui->cpuLayout->addWidget(lineEd);
    } else {
        ui->errorLabel->setText("Maximum player capacity reached (5)");
        ui->errorLabel->show();
        QTimer::singleShot(4000, ui->errorLabel, &QLabel::hide);
    }
}

void StartingScreen::on_removeCPUbtn_clicked()
{
    QWidget *child;
    if (botPlayers > 0 and qlist.count() > 1) {
        if ((child = ui->cpuLayout->takeAt(botPlayers-1)->widget()) != 0 ) {
            delete child;
            botPlayers--;
        }
    } else {
        ui->errorLabel->setText("At least two players required");
        ui->errorLabel->show();
        QTimer::singleShot(4000, ui->errorLabel, &QLabel::hide);
    }
}

void StartingScreen::on_helpButton_clicked()
{
    QMessageBox* mesBox = new QMessageBox();
    mesBox->setStyleSheet("background-image: url(:/img/background/background.png); border: none; font-family: Console; color: white;");
    mesBox->setText("Click the '?' icon in game window for a quick How to play reference! \n"
                    "\n"
                    "For full description of game rules and gameplay, please read the documentation.");
    mesBox->exec();

}
