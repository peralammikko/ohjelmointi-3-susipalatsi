#include "actioncard.hh"

using std::shared_ptr;

Interface::ActionCard s = Interface::ActionCard();

namespace Interface {

ActionCard::ActionCard(QString name, QString typeName, QString title){
    name_ = name;
    typeName_ = typeName;
    title_ = title;
    cardItem_ = nullptr;


}

ActionCard::~ActionCard(){}

QString ActionCard::typeName() const
{
    return typeName_;
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
