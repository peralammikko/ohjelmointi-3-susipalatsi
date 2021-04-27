#ifndef ACTIONCARD_HH
#define ACTIONCARD_HH
#include "cardinterface.h"



/**
 * @file
 * @brief Visual representation of an action card
 */

namespace Interface {

class ActionCard : public Interface::CardInterface
{
public:
    /**
     * @brief ActionCard is a type of a card which is used for paying agent actions
     */
    explicit ActionCard();
    ~ActionCard();

    QString typeName() const override;
    QString title() const override;
    QString name() const override;
    std::weak_ptr<Location> location() const override;
    std::weak_ptr<Player> owner() const override;
    void setOwner(std::weak_ptr<Player> owner) override;

private:
    QString name_;
    QString title_;
    QString description_;
    std::weak_ptr<Player> owner_;
    std::weak_ptr<Location> location_;
};

} // Interface
#endif // ACTIONCARD_HH
