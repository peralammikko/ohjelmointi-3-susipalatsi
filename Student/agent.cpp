#include <QDebug>

#include "agent.hh"
#include "councilor.h"

namespace Interface {


Agent::Agent(QString name, std::weak_ptr<Player> owner) : name_(name), owner_(owner), placement_(std::weak_ptr<Location>())
{
    placement_.reset();
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

void Agent::addResource(std::shared_ptr<Interface::Location> agentAt, std::shared_ptr<CommonResource> res, int amount)
{
    AgentResourceMap::iterator iter = gatheredResources_.find(agentAt);
    for (int i = 0; i < amount; i++) {
        iter->second.push_back(res);
    }
}

void Agent::removeResource(std::shared_ptr<Location> &agentAt,  int amount)
{
    int resourcesAt = gatheredResources_.at(agentAt).size();
    if (resourcesAt >= amount) {
        for (int i = 0; i < amount; i++) {
            gatheredResources_.at(agentAt).pop_back();
        }
    }
}

bool Agent::addCouncilCard(std::shared_ptr<Councilor> card)
{
    if (councilCardHolder == nullptr) {
        councilCardHolder = card;
        qDebug() << this->name() << "is now holding " << card->name() << " card";
        return true;
    } else {
        return false;
    }
}

bool Agent::hasCouncilCard()
{
    if (councilCardHolder == nullptr) {
        return false;
    } else {
        return true;
    }
}

std::shared_ptr<Councilor> Agent::getCouncilCard()
{
    return councilCardHolder;
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
