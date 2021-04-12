#ifndef AGENTITEM_HH
#define AGENTITEM_HH
#include <QDebug>

#include "mapitem.hh"
#include "agentinterface.h"

class agentItem : public mapItem, public Interface::AgentInterface
{
public:
    explicit agentItem(std::shared_ptr<Interface::AgentInterface> &obj);
    bool isSelected = false;
    std::shared_ptr<Interface::AgentInterface> getObject();

    // MapItem overridet
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void setCoords(int x, int y); //override;
    const std::pair<int, int> getCoords(); // override;

    // AgentInterface overridet
    bool isCommon() const override;
    unsigned short connections() const override;
    void modifyConnections(short change) override;
    std::weak_ptr<Interface::Location> placement() const override;
    void setConnections(unsigned short connections) override;
    void setPlacement(std::weak_ptr<Interface::Location> placement) override;

    // CardInterface overridet
    QString typeName() const override;
    QString name() const override;
    QString title() const override;
    std::weak_ptr<Interface::Location> location() const override;
    std::weak_ptr<Interface::Player> owner() const override;
    void setOwner(std::weak_ptr<Interface::Player>) override;

    void testPrint() {qDebug() << "Tämä on agentti";}

    const QString typeOf() override;

private:
    int itemx, itemy;
    std::shared_ptr<Interface::AgentInterface> agentObject_;
    std::weak_ptr<Interface::Location> locationAt_;
    const std::weak_ptr<Interface::Player> agentOwner_;
    QString agentName_;
    int value_;
    int negSkill_;
    int agentConnections_;
};

#endif // AGENTITEM_HH
