#ifndef SETTINGSSCREEN_HH
#define SETTINGSSCREEN_HH

#include <QDialog>

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
    void sendInfo(int agentCount, int locCount, int winCount);

private slots:
    void on_returnButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::SettingsScreen *ui;
    int agentNum_ = 3;
    int locNum_ = 6;
    int cardNum_ = 3;
};

#endif // SETTINGSSCREEN_HH
