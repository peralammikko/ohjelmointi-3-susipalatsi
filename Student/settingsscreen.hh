#ifndef SETTINGSSCREEN_HH
#define SETTINGSSCREEN_HH

#include <QDialog>

/**
 * @file
 * @brief For user's custom settings
 */
namespace Ui {
class SettingsScreen;
}

class SettingsScreen : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = nullptr);
    ~SettingsScreen();


signals:
    /**
     * @brief Sends information about game's settings
     * @param agentCount: Number of starting agents for each player
     * @param locCount: Number of locations in game
     * @param winCount: Number of councilor cards needed for winning the game
     */
    void sendInfo(int agentCount, int locCount, int winCount);

private slots:
    /**
     * @brief Closes the settings window without saving and sending the settings info
     */
    void on_returnButton_clicked();

    /**
     * @brief Closes the settings window and sends settings information back to starting screen.
     */
    void on_saveButton_clicked();

private:
    Ui::SettingsScreen *ui;
    int agentNum_ = 3;
    int locNum_ = 6;
    int cardNum_ = 3;
};

#endif // SETTINGSSCREEN_HH
