#ifndef ACTIONCARD_HH
#define ACTIONCARD_HH
#include "../Course/cardinterface.h"
#include "carditem.hh"

#include <memory>

namespace Interface {

class ActionCard : public CardInterface
{
public:
    explicit ActionCard(QString name="NONAME", QString typeName="action", QString title ="");
    ~ActionCard();

    QString typeName() const override;
    QString title() const override;
    QString name() const override;
    std::weak_ptr<Location> location() const override;
    std::weak_ptr<Player> owner() const override;
    void setOwner(std::weak_ptr<Player> owner) override;

    // This is probably useless
    std::shared_ptr<CardItem> getCardItem();

private:
    QString name_;
    QString typeName_;
    QString title_;

    QString description_;

    std::shared_ptr<CardItem> cardItem_;

    std::weak_ptr<Player> owner_;
    std::weak_ptr<Location> location_;


};

#endif // ACTIONCARD_HH
} // Interface
