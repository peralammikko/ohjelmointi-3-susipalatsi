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
    explicit CommonResource(QString name, std::weak_ptr<Location> loc, QString spritePath,int amount = 0);
    ~CommonResource();
    int amount();

    QString typeName() const override;
    QString title() const override;
    QString name() const override;
    std::weak_ptr<Location> location() const override;
    std::weak_ptr<Player> owner() const override;
    void setOwner(std::weak_ptr<Player> owner) override;
    void setAmountTo(int num);
    const QString getSpritePath(){return spritePath_;}


private:
    QString name_;
    QString typeName_;
    std::weak_ptr<Location> location_;
    int amount_;
    QString spritePath_;

};

// const Interface::CommonResource NULLRES("", std::weak_ptr<Interface::Location>(), "");

}

using ResourceMap = std::map<std::shared_ptr<Interface::Location>, std::shared_ptr<Interface::CommonResource>>;
using AgentResourceMap = std::map<std::shared_ptr<Interface::Location>, std::deque<std::shared_ptr<Interface::CommonResource>>>;

#endif // COMMONRESOURCE_HH
