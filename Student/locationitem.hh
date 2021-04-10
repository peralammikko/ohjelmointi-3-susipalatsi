#ifndef LOCATIONITEM_HH
#define LOCATIONITEM_HH

#include <QMainWindow>
#include <QObject>

#include "mapitem.hh"

class LocationItem : public mapItem
{
public:
    LocationItem(const std::shared_ptr<Interface::Location> &location);

    // MapItem overridet

    // Asetetaan koordinaatit itemille
    void setCoords(int x, int y) override;

    // Haetaan itemin koordinaatit
    const std::pair<int, int> getCoords() override;

    // Luodaan itemille muoto (neliö)
    QRectF boundingRect() const override;

    // Piirretään neliöt ja nimet + maalataan jos valittuna
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // Klikkauksesta tapahtuva metodi
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


    bool isSelected = false;

    // Haetaan itemin kantaluokka (Location)
    const std::shared_ptr<Interface::Location> getObject();

private:
    int itemx, itemy;
    const std::shared_ptr<Interface::Location> locationObject_;
};

#endif // LOCATIONITEM_HH
