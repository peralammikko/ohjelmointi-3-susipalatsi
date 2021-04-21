#include "commonresource.hh"

using std::shared_ptr;

namespace Interface {

CommonResource::CommonResource(QString name, std::weak_ptr<Location> loc, int amount) : amount_(amount) {
    name_ = name;
    location_ = loc;
}

CommonResource::~CommonResource(){}

int CommonResource::amount()
{
    return amount_;
}

QString CommonResource::typeName() const
{
    return typeName_;
}

QString CommonResource::title() const
{
    return "Resource";
}

QString CommonResource::name() const
{
    return name_;
}

std::weak_ptr<Location> CommonResource::location() const
{
    return location_;
}

std::weak_ptr<Player> CommonResource::owner() const
{

}

void CommonResource::setOwner(std::weak_ptr<Player> owner)
{

}

void CommonResource::setAmountTo(int num)
{
    amount_ = num;
}

} // Interface

