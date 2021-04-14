#include "agent.hh"

namespace Interface {


Agent::Agent(QString name, std::weak_ptr<Player> owner, QString typeName) : name_(name), owner_(owner), typeName_(typeName)
{
    name_ = name;
    typeName_ = typeName;
}

Agent::~Agent()
{

}

bool Agent::isCommon() const
{
    return 1;
}

std::weak_ptr<Location> Agent::placement() const
{

}

void Agent::setPlacement(std::weak_ptr<Location> placement)
{

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
    return typeName_;
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

}

void Agent::setOwner(std::weak_ptr<Player> owner)
{

}

std::weak_ptr<Location> Agent::location() const
{

}

} // Interface
