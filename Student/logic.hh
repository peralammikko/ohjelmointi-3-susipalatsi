#ifndef LOGIC_HH
#define LOGIC_HH
#include <QObject>
#include <memory>

#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

#include "../Course/runner.h"
#include "../Course/actioninterface.h"
#include "../Course/controlinterface.h"


class Logic : public QObject
{
    Q_OBJECT
public :
    Logic(std::shared_ptr<Interface::Runner> runner);
    ~Logic();
    void doTheRunning();

public slots:
    void actionSelected(std::shared_ptr<Interface::ActionInterface> action);
private:
    void setNextAction();

    std::shared_ptr<Interface::Runner> runner_;
    std::shared_ptr<Interface::ControlInterface> ctrl_;

    std::shared_ptr<Interface::ManualControl> manualCtrl_;

    std::shared_ptr<Interface::ActionInterface> action_;

};

#endif // LOGIC_HH
