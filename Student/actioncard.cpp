#include "actioncard.hh"
#include "location.h"

namespace Interface {

ActionCard::ActionCard(std::weak_ptr<Location> location) :
    name_("Action Card"), title_("title"), description_(""), location_(location)
{
    if (location.lock()){
        auto locatio = location.lock();
        description_ = "Drawn in " + location.lock()->name();
    } else {
        description_ = "Regular action card";
    }
}

ActionCard::~ActionCard(){}

QString ActionCard::typeName() const
{
    return "actioncard";
}

QString ActionCard::title() const
{
    return title_;
}

QString ActionCard::name() const
{
    return name_;
}

std::weak_ptr<Location> ActionCard::location() const
{
    return location_;
}

std::weak_ptr<Player> ActionCard::owner() const
{
    return owner_;
}

void ActionCard::setOwner(std::weak_ptr<Player> owner)
{
    owner_ = owner;
}


} // Interface
