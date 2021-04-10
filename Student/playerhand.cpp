#include "playerhand.hh"

PlayerHand::PlayerHand(QGraphicsScene* scene, std::shared_ptr<Interface::Player> player, int x, int y)
{
    scene_ = scene;
    xCenterCoord = x;
    yCenterCoord = y;
    player_ = player;
           
}

