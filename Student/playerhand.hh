#ifndef PLAYERHAND_HH
#define PLAYERHAND_HH

#include <QObject>
#include <QGraphicsScene>


#include "../Course/player.h"
// For now we use just simple card items
#include "../Course/simplecarditem.hh"

class PlayerHand
{
public:
    PlayerHand(QGraphicsScene* scene, std::shared_ptr<Interface::Player> player, int x=0, int y=0);
    void updateHand();
private:
    QGraphicsScene* scene_;
    std::shared_ptr<Interface::Player> player_;
    
    int xCenterCoord;
    int yCenterCoord;
    float scale = 1;

    QColor outline = QColor(200, 0, 200);

    // returns combined card width
    int getWidth();
    // returns card height
    int getHeight();

    
};

#endif // PLAYERHAND_HH
