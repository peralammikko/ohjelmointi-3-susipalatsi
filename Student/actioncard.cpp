#include "actioncard.hh"

namespace Interface {

ActionCard::ActionCard() :
    name_("Action Card"), title_("title"), description_("Olen kortti. Minut pelatessasi tapahtuu jotain (description)")
{

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
