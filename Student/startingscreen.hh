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
    void sendStartingInfo(std::vector<QString> playerNames, std::vector<int> customSettings, int botPlayers);

private slots:
    void on_newGameButton_clicked();

    void on_settingsButton_clicked();

    void on_quitButton_clicked();

    void on_addPlayerBtn_clicked();

    void on_removePlayerBtn_clicked();

    void on_startButton_clicked();

    void on_closeFrameButton_clicked();

    void on_aiCheckbox_stateChanged(int arg1);

    void on_addCPUbtn_clicked();

    void on_removeCPUbtn_clicked();

private:
    Ui::StartingScreen *ui;
    SettingsScreen* settings = nullptr;

    std::vector<QString> playerNames = {};
    QList<QLineEdit*> qlist;

    std::vector<int> customSettings = {};
    int botPlayers = 0;
};

#endif // STARTINGSCREEN_HH
