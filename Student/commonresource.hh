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
    explicit CommonResource(QString name, std::weak_ptr<Location> loc, QString spritePath ="", int amount = 0);
    ~CommonResource();
    /**
     * @brief Returns the set amount of a resource.
     * @return Amount of resource in question.
     */
    int amount();

    /**
     * @brief CardInterface overridden functions, most of which aren't used for this class.
     */
    QString typeName() const override;
    QString title() const override;
    QString name() const override;
    std::weak_ptr<Location> location() const override;
    std::weak_ptr<Player> owner() const override;
    void setOwner(std::weak_ptr<Player> owner) override;

    /**
     * @brief Sets the resources Amount variable to desired. Used for generating demands on locations.
     * @param num: Desired number for resource "value"
     * @pre Resource exists
     */
    void setAmountTo(int num);

    /**
     * @brief Used to get the file path for corresponding sprite to a resource
     * @return File path in QString form
     * @pre File exists in presumed location
     * @post Path is found and resource gets it's respective sprite
     */
    const QString getSpritePath(){return spritePath_;}


private:
    QString name_;
    QString typeName_;
    std::weak_ptr<Location> location_;
    int amount_;
    QString spritePath_;
    std::weak_ptr<Player> owner_;

};

}

/**
 * @brief Abbreviations to make life easier when managing location's resources.
 * Both used for initializing LocationItems.
 */
using ResourceMap = std::map<std::shared_ptr<Interface::Location>, std::shared_ptr<Interface::CommonResource>>;
using AgentResourceMap = std::map<std::shared_ptr<Interface::Location>, std::deque<std::shared_ptr<Interface::CommonResource>>>;

#endif // COMMONRESOURCE_HH
