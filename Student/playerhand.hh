#ifndef PLAYERHAND_HH
#define PLAYERHAND_HH

#include <QObject>
#include <QGraphicsScene>


#include "../Course/player.h"
// For now we use just simple card items
#include "carditem.hh"


class PlayerHand
{
public:
    PlayerHand(QGraphicsScene* scene, std::shared_ptr<Interface::Player> player, QGraphicsItem *parent = nullptr, int x=0, int y=0);

    void updateHand();
    void renderHand();
    void changeCoords(int x, int y);


private:
    QGraphicsScene* scene_;
    std::shared_ptr<Interface::Player> player_;

    int xCenterCoord;
    int yCenterCoord;

    float scale = 1;

    std::vector<CardItem> cards_;

    // QColor outline = QColor(200, 0, 200);

    // returns combined card width
    int getWidth();
    // returns card height
    int getHeight();


    
};

#endif // PLAYERHAND_HH
