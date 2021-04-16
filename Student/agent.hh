#ifndef AGENT_HH
#define AGENT_HH
#include "../Course/agentinterface.h"
#include "commonresource.hh"

#include <memory>
#include <map>
#include <deque>

namespace Interface {

class Agent : public AgentInterface
{
public:
    Agent(QString name, std::weak_ptr<Player> owner);
    ~Agent();

    // Initialize agent's backpack for resources to gather
    void initAgentResources(ResourceMap resMap);

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
    // for our sakes and purposes this method is the same as placement()
    virtual std::weak_ptr<Location> location() const override;

private:
    QString name_;
    QString typeName_;
    std::weak_ptr<Player> owner_;
    std::weak_ptr<Location> placement_;
    QString title_;

    ResourceMap gatheredResources_;

};


}
#endif // AGENT_HH
