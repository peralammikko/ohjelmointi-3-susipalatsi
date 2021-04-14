#ifndef AGENT_HH
#define AGENT_HH
#include "../Course/agentinterface.h"

#include <memory>

namespace Interface {

class Agent : public AgentInterface
{
public:
    Agent(QString name, QString typeName);
    ~Agent();

    // AgentInterface overrides
    virtual bool isCommon() const override;
    virtual std::weak_ptr<Location> placement() const override;
    virtual void setPlacement(std::weak_ptr<Location> placement) override;
    virtual unsigned short connections() const  override;
    virtual void setConnections(unsigned short connections) override;
    virtual void modifyConnections(short change) override;

    // CardInterface overrides
    virtual QString typeName() const override;
    virtual QString name() const override;
    virtual QString title() const override;
    virtual std::weak_ptr<Player> owner() const override;
    virtual void setOwner(std::weak_ptr<Player> owner) override;

    // You probably should not use location(), or at least know that it is different to setPlacement and placement.
    virtual std::weak_ptr<Location> location() const override;

private:
    QString name_;
    QString typeName_;
    QString title_;
    std::weak_ptr<Player> owner_;
};


}
#endif // AGENT_HH