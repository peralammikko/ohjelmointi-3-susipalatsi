#include <QDebug>

#include "agent.hh"

#include <QDebug>

namespace Interface {


Agent::Agent(QString name, std::weak_ptr<Player> owner) : name_(name), owner_(owner), placement_(std::weak_ptr<Location>())
{
    qDebug() << "agent";
    auto f =  owner_;
    auto s = f.lock();
    s->name();
    qDebug() << owner.lock()->name();
    placement_.reset(); // wtf is this for
}

Agent::~Agent()
{

}

void Agent::initAgentResources(AgentResourceMap resMap)
{
    gatheredResources_ = resMap;
}

AgentResourceMap Agent::getAgentResources()
{
    return gatheredResources_;
}
/*
ResourceMap Agent::getResources()
{
    return gatheredResources_;
}
*/
void Agent::addResource(std::shared_ptr<Interface::Location> agentAt, CommonResource res, int amount)
{
    AgentResourceMap::iterator iter = gatheredResources_.find(agentAt);
    for (int i = 0; i < amount; i++) {
        iter->second.push_back(res);
        qDebug() << i << " resources added to " << this->name();
    }
}

std::shared_ptr<Location> Agent::whereIsAgent()
{
    std::shared_ptr<Interface::Location> loc = this->placement().lock();
    if (!loc) {
        return nullptr;
    } else {
        return loc;
    }
}

bool Agent::isCommon() const
{
    return 1;
}

std::weak_ptr<Location> Agent::placement() const
{
    return placement_;
}

void Agent::setPlacement(std::weak_ptr<Location> placement)
{
    placement_ = placement;
}

unsigned short Agent::connections() const
{
    return 0;
}

void Agent::setConnections(unsigned short connections)
{

}

void Agent::modifyConnections(short change)
{

}


QString Agent::typeName() const
{
    return "agent";
}

QString Agent::name() const
{
    return name_;
}

QString Agent::title() const
{
    return "";
}

std::weak_ptr<Player> Agent::owner() const
{
    return owner_;
}

void Agent::setOwner(std::weak_ptr<Player> owner)
{

}


std::weak_ptr<Location> Agent::location() const
{
    return placement_;
}

} // Interface
