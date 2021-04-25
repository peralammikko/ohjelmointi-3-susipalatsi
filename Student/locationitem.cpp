#include <QDebug>

#include "locationitem.hh"
#include "gamescene.hh"

LocationItem::LocationItem(const std::shared_ptr<Interface::Location> location, std::vector<std::pair<QString, QString> > spritePaths, std::shared_ptr<Interface::CommonResource> localRes, std::shared_ptr<Interface::CommonResource> demandRes) :
    locationObject_(location),
    basevalue_(1),
    localRes_(localRes),
    demandRes_(demandRes),
    isSelected(false),
    isHovered_(false)
{
    // Default sprites
    planetImage_ = new QPixmap(":/img/planets/img/some sprites/planet iridium.png");
    governorImage_ = new QPixmap(":/img/governors/img/governors/2.png");
    setAcceptHoverEvents(true);
    for (int i = 0; i < spritePaths.size(); ++i){
        QString spritePath = spritePaths.at(i).second;
        if (spritePaths.at(i).first == "planet"){
            delete planetImage_;
            planetImage_ = new QPixmap(spritePath);
        }
        if (spritePaths.at(i).first == "governorLbl") {
            delete governorImage_;
            governorImage_ = new QPixmap(spritePath);
        }
    }
}

LocationItem::~LocationItem()
{
    delete governorImage_;
    delete planetImage_;
}

QRectF LocationItem::boundingRect() const
{
    return QRectF(0, 0, 180,180);
}

void LocationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(20, 20, boundingRect().width()-70, boundingRect().height()-70,  *planetImage_);

    QRectF govrect = QRectF(30, 30, 30, 30);
    painter->setBrush(QColor(145, 145, 145, 125));
    painter->drawEllipse(govrect);
    painter->drawPixmap(govrect.x()+5, govrect.y()+5, govrect.width()-10, govrect.height()-10, localRes_.getSpritePath());

    
    int xPos = boundingRect().x();
    int yPos = boundingRect().y();
    int yPadding = 70;
    QString placeName = this->getObject()->name();
    QRect nameRect(xPos, yPos-yPadding, boundingRect().width(),boundingRect().height());

    painter->drawPixmap(0, 0, boundingRect().width(), boundingRect().height(),  *planetImage_);
    painter->drawText(nameRect, Qt::AlignCenter, placeName);

}

const std::shared_ptr<Interface::Location> LocationItem::getObject()
{
    return locationObject_;
}

int LocationItem::getBasevalue()
{
    return basevalue_;
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

void LocationItem::setDemandedResource(std::shared_ptr<Interface::CommonResource> &res)
{
    demandRes_ = res;
}

std::shared_ptr<Interface::CommonResource> LocationItem::getDemandedResource()
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
                demandRes_->setAmountTo(amount);
                break;
            }
        }
    }
}

void LocationItem::rearrange()
{
    QPointF const center = QPointF(boundingRect().width()/2 -50, boundingRect().height()/2-50);

    std::vector<mapItem*> aItems;

    QList<QGraphicsItem *> const items = childItems();
    int count = items.size();
    if (count) {

        // TODO: Separate agents between players
        for (int i = 0; i < count; ++i) {
            auto mItem = dynamic_cast<mapItem*>(items.at(i));
            if (mItem){
                aItems.push_back(mItem);
            }
        }
    }
    int radius = boundingRect().width()/2 -30;
    int agentCount = aItems.size();
    if (!agentCount)
    {
        return;
    } else if (agentCount==1){
        radius = 0;
    }
    const int degree = 360 / agentCount;

    for (int i = 0; i < agentCount; i++) {
        auto currentAgent = aItems.at(i);
        // Geometrinen sijainti kehällä
        float angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;

        float x = center.x() + radius * std::cos(angleRad);
        float y = center.y() + radius * std::sin(angleRad);

        currentAgent->setHome(QPointF(x,y));
        currentAgent->goHome();
    }

}

agentItem *LocationItem::getAgentItemFor(std::shared_ptr<Interface::AgentInterface> agent)
{
    auto childItems = this->childItems();
    for (int i = 0; i < childItems.size(); ++i){
        agentItem* aItem = dynamic_cast<agentItem*>(childItems.at(i));
        if (aItem){
            if (aItem->getAgentClass() == agent){
                return aItem;
            }
        }
    }
}


void LocationItem::setLocalResource(std::shared_ptr<Interface::CommonResource> &res)
{
    localRes_ = res;
}

std::shared_ptr<Interface::CommonResource> LocationItem::getLocalResource()
{
    return localRes_;
}
