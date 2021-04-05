#ifndef ACTIONCARD_HH
#define ACTIONCARD_HH
#include "../Course/deckinterface.h"

#include <QObject>
#include <QGraphicsItem>
#include <memory>

namespace Interface {
    class CardInterface;
}


class ActionCard :public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ActionCard(int width, int height, int padding_x, int padding_y, QString name, std::shared_ptr<Interface::CardInterface> card,  QString description = "", QColor color = Qt::white, QObject *parent = nullptr);

};

#endif // ACTIONCARD_HH
