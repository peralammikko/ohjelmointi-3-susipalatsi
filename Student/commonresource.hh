#ifndef COMMONRESOURCE_HH
#define COMMONRESOURCE_HH

#include <QMainWindow>
#include <deque>
#include "location.h"
#include "cardinterface.h"

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
    void setAmountTo(int num);

private:
    QString name_;
    QString typeName_;
    std::weak_ptr<Location> location_;
    int amount_;
};

}

using ResourceMap = std::map<std::shared_ptr<Interface::Location>, Interface::CommonResource>;
using AgentResourceMap = std::map<std::shared_ptr<Interface::Location>, std::deque<Interface::CommonResource>>;

const Interface::CommonResource NULLRES("", std::weak_ptr<Interface::Location>());

#endif // COMMONRESOURCE_HH
