#ifndef POPUPDIALOG_HH
#define POPUPDIALOG_HH

#include <QDialog>

#include "commonresource.hh"
#include "gamescene.hh"

namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(std::shared_ptr<Interface::Location> loc, int BV, Interface::CommonResource res, std::shared_ptr<Interface::Player> player, QWidget *parent = 0);
    ~PopupDialog();
    void fillAreaAgentsList();

private:
    Ui::PopupDialog *ui;
    std::shared_ptr<Interface::Location> location_ = nullptr;
    std::shared_ptr<Interface::Player> player_ = nullptr;
    int locationBV_ = 0;
    Interface::CommonResource areaRes;
    QString areaResName = "";
};

#endif // POPUPDIALOG_HH
