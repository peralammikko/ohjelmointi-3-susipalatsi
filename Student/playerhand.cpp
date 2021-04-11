#include "playerhand.hh"


PlayerHand::PlayerHand(QGraphicsScene* scene, std::shared_ptr<Interface::Player> player, QGraphicsItem *parent, int x, int y)
{
    scene_ = scene;
    xCenterCoord = x;
    yCenterCoord = y;
    player_ = player;
           
}

void PlayerHand::updateHand()
{
    std::vector<std::shared_ptr<Interface::CardInterface>> cards = player_->cards();
    for (unsigned int i = 0; i < cards.size(); ++i){
        
    }
}

void PlayerHand::renderHand()
{

}
