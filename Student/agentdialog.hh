#ifndef AGENTDIALOG_HH
#define AGENTDIALOG_HH

#include <QDialog>
#include "agent.hh"

namespace Ui {
class AgentDialog;
}

class AgentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgentDialog(std::shared_ptr<Interface::Agent> agentClicked, QWidget *parent = nullptr);
    ~AgentDialog();
    void listResources();

private:
    Ui::AgentDialog *ui;
    std::shared_ptr<Interface::Agent> agent_;
};

#endif // AGENTDIALOG_HH
