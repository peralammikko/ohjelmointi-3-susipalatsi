#include "actioncard.hh"

using std::shared_ptr;

namespace Interface {

ActionCard::ActionCard(QString name, QString title) :
    name_(name), title_(title)
{
    cardItem_ = nullptr;
    description_ = "Olen kortti. Minut pelatessasi tapahtuu jotain (description)";
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
