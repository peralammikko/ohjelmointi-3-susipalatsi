#ifndef COMMONRESOURCE_HH
#define COMMONRESOURCE_HH

#include <QMainWindow>
#include "location.h"
#include "cardinterface.h"

/*
enum CommonResource {
    NONE = 0,
    KULTA = 1,
    VOHVELI = 2,
    LAHNA = 3,
    OLVI = 4,
    OP = 5,
    BITCOIN = 6
};

using ResourceMap = std::map<CommonResource, int>;
using ResourceNameMap = std::map<CommonResource, QString>;
using AreaResources = std::map<std::shared_ptr<Interface::Location>, CommonResource>;

const ResourceNameMap RESOURCE_NAMES = {
    {CommonResource::KULTA, "Kulta"},
    {CommonResource::VOHVELI, "Vohveli"},
    {CommonResource::LAHNA, "Lahna"},
    {CommonResource::OLVI, "Olvi III"},
    {CommonResource::OP, "TUNI Op"},
    {CommonResource::BITCOIN, "Bitcoin"}
};

const ResourceMap PLAYER_STARTING_RESOURCES = {
    {CommonResource::KULTA, 0},
    {CommonResource::VOHVELI, 0},
    {CommonResource::LAHNA, 0},
    {CommonResource::OLVI, 0},
    {CommonResource::OP, 0},
    {CommonResource::BITCOIN, 0}
};
*/

namespace Interface {

class CommonResource : public Interface::CardInterface
{
public:
    explicit CommonResource(QString name, std::weak_ptr<Location> loc, int amount = 0);
    ~CommonResource();
    int amount();

    QString typeName() const override;
    QString title() const override;
    QString name() const override;
    std::weak_ptr<Location> location() const override;
    std::weak_ptr<Player> owner() const override;
    void setOwner(std::weak_ptr<Player> owner) override;

private:
    QString name_;
    QString typeName_;
    std::weak_ptr<Location> location_;
    int amount_;
};

}

using ResourceMap = std::map<std::shared_ptr<Interface::Location>, Interface::CommonResource>;

#endif // COMMONRESOURCE_HH
