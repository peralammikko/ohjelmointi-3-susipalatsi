#ifndef LOCATIONITEM_HH
#define LOCATIONITEM_HH

#include <QMainWindow>
#include <QObject>

#include "mapitem.hh"
#include "popupdialog.hh"

class LocationItem : public mapItem
{
    Q_OBJECT
public:
    LocationItem(const std::shared_ptr<Interface::Location> location);

    // MapItem overridet

    // Asetetaan koordinaatit itemille
    // tarpeeton, sillä QGraphicsItemillä on jo oma metodi tälle
    void setCoords(int x, int y); // override;

    // Haetaan itemin koordinaatit
    // tarpeeton, sillä QGraphicsItemillä on jo oma metodi tälle
    const std::pair<int, int> getCoords(); // override;

    // Luodaan itemille muoto (neliö)
    QRectF boundingRect() const override;

    // Piirretään neliöt ja nimet + maalataan jos valittuna
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // Klikkauksesta tapahtuva metodi
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    // Haetaan itemin kantaluokka (Location)
    const std::shared_ptr<Interface::Location> getObject();

    int getBasevalue();

    const QString typeOf() override;

signals:
    void locationItemPressed(LocationItem*);

private:
    int itemx, itemy;
    const std::shared_ptr<Interface::Location> locationObject_;
    int basevalue_;
    bool isSelected = false;
    bool isHovered_ = false;
};

#endif // LOCATIONITEM_HH
