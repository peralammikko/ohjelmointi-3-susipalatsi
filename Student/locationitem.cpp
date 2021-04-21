#include <QDebug>

#include "locationitem.hh"
#include "gamescene.hh"

LocationItem::LocationItem(const std::shared_ptr<Interface::Location> location, int mapIndex) : locationObject_(location), mapIndex_(mapIndex), basevalue_(1), isSelected(false), isHovered_(false)
{
    setAcceptHoverEvents(true);
}

LocationItem::~LocationItem()
{

}

QRectF LocationItem::boundingRect() const
{
    return QRectF(0, 0, 150,150);
}

void LocationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Piirretään paikat ja niiden nimet.
    // Maalataan punaiseksi jos "valittu"

    QRectF rect = boundingRect();
    painter->fillRect(rect, Qt::gray);

    QPoint upperpos(boundingRect().x()+5, 10);
    QPoint lowerpos(0, boundingRect().height()-10);
    QString placeName = this->getObject()->name();
    QPen pen;
    painter->drawText(upperpos, placeName);
    painter->drawText(lowerpos, "Base value: " + QString::number(this->getBasevalue()));

    if (isHovered_) {
        pen.setColor(Qt::red);
    } else {
        pen.setColor(Qt::black);
    }
    painter->setPen(pen);
    painter->drawRect(rect);
  ;
}

const std::shared_ptr<Interface::Location> LocationItem::getObject()
{
    return locationObject_;
}

int LocationItem::getBasevalue()
{
    return basevalue_;
}

int LocationItem::mapIndex()
{
    return mapIndex_;
}

void LocationItem::setMapIndex(int newIndex)
{
    mapIndex_ = newIndex;
}

void LocationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit locationItemPressed(this);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void LocationItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = true;
    if (childItems().size())
    {
      //  setRotation(rotation()+45);
    }
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void LocationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setRotation(0);
    isHovered_ = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}



const QString LocationItem::typeOf()
{
    return "locationitem";
}

std::vector<int> LocationItem::calculateRewards(std::shared_ptr<Interface::Player> &player)
{

    // WORK IN PROGRESS
    std::vector<int> numbers(3);
    int sum = basevalue_;
    int ownAgents = 0;
    int enemyAgents = 0;
    std::shared_ptr<Interface::Location> locPtr = this->getObject();
    for (auto agent : locPtr->agents()) {
        std::shared_ptr<Interface::Player> agentOwner = agent->owner().lock();
        std::shared_ptr<Interface::Location>agentPlacement = agent->placement().lock();
        if (!agentPlacement) {
            qDebug() << "owner not found";
        } else {
            if (agentOwner == player) {
                ownAgents += 1;
            } else {
                enemyAgents += 1;
            }
        }
    }
    sum = (basevalue_ + ownAgents) - enemyAgents;
    if (sum <= 0) {
        sum = 1;
    }
    numbers[0] = sum;
    numbers[1] = ownAgents;
    numbers[2] = enemyAgents;
    return numbers;

}

void LocationItem::setDemandedResource(Interface::CommonResource &res)
{
    demandRes_ = res;
}

Interface::CommonResource LocationItem::getDemandedResource()
{
    return demandRes_;
}

void LocationItem::generateNewDemand()
{
    GameScene* scene = dynamic_cast<GameScene*>(this->scene());
    if (scene) {
        ResourceMap rmap = scene->getResMap();
        ResourceMap::iterator it;
        while (true) {
            it = rmap.begin();
            int rndm = Interface::Random::RANDOM.uint(5);
            std::advance(it, rndm);
            if (it->first != locationObject_) {
                demandRes_ = it->second;
                int amount = 2+ Interface::Random::RANDOM.uint(3);
                demandRes_.setAmountTo(amount);
                break;
            }
        }
    }
}

void LocationItem::addInfluence(std::shared_ptr<Interface::Player> &player)
{
    playerInfluence_.at(player) += 1;
}
/*
bool LocationItem::giveCouncilCard(std::shared_ptr<Interface::Agent> &agent)
{
    QString councName = "Mr. " + locationObject_->name();
    std::shared_ptr<Interface::Councilor> counc = std::make_shared<Interface::Councilor>(councName, "Mestari", locationObject_);
    agent->addCouncilCard(counc);
}
*/

void LocationItem::setLocalResource(Interface::CommonResource &res)
{
    localRes_ = res;
}

Interface::CommonResource LocationItem::getLocalResource()
{
    return localRes_;
}

void LocationItem::advance(int phase)
{
    // qDebug() << "Tick";
    // Proof of concept here
    //setRotation(rotation()+1);
}

