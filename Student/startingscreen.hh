#ifndef STARTINGSCREEN_HH
#define STARTINGSCREEN_HH

#include <QDialog>
#include <QLineEdit>
#include <QApplication>
#include "settingsscreen.hh"

namespace Ui {
class StartingScreen;
}

class StartingScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartingScreen(QWidget *parent = nullptr);
    ~StartingScreen();

public slots:
    void getSettings(int agentCount, int locCount, int winCardCount);

signals:
    void sendStartingInfo(std::vector<QString> playerNames, std::vector<int> customSettings);

private slots:
    void on_newGameButton_clicked();

    void on_settingsButton_clicked();

    void on_quitButton_clicked();

    void on_addPlayerBtn_clicked();

    void on_removePlayerBtn_clicked();

    void on_pushButton_clicked();

    void on_closeFrameButton_clicked();

private:
    Ui::StartingScreen *ui;
    SettingsScreen* settings = nullptr;

    std::vector<QString> playerNames = {};
    QList<QLineEdit*> qlist;

    std::vector<int> customSettings = {};

    const QFont consoleFont = QFont("Console Regular", 11);
};

#endif // STARTINGSCREEN_HH
