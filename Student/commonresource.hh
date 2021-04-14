#ifndef COMMONRESOURCE_HH
#define COMMONRESOURCE_HH

#include <QMainWindow>
#include "location.h"

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



#endif // COMMONRESOURCE_HH
