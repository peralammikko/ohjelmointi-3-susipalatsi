#ifndef COMMONRESOURCE_HH
#define COMMONRESOURCE_HH

#include <QMainWindow>
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

private:
    QString name_;
    QString typeName_;
    std::weak_ptr<Location> location_;
    int amount_;
};

}

using ResourceMap = std::map<std::shared_ptr<Interface::Location>, Interface::CommonResource>;

#endif // COMMONRESOURCE_HH
