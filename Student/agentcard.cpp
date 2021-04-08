#include "agentcard.hh"

agentCard::agentCard(std::shared_ptr<Interface::AgentInterface>) : agentObject_(nullptr), agentName_(""), value_(2), negSkill_(1)
{
    agentObject_ = nullptr;
}
