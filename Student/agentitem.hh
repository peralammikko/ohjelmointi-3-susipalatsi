#ifndef AGENTITEM_HH
#define AGENTITEM_HH
#include <QDebug>
#include <QTimer>
#include <QApplication>

#include "mapitem.hh"
#include "agentdialog.hh"
#include "playerhand.hh"


class agentItem : public mapItem
{
public:
    agentItem(std::shared_ptr<Interface::Agent> &agentinterface);
    ~agentItem();

    bool isSelected = false;
    std::shared_ptr<Interface::Agent> getAgentClass();

    // MapItem overridet
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    const QString typeOf() override;
    void rearrange() override{};

    void displayResourceChange(int amount, QString path); // TODO: maybe add sprite too?


protected:
    // some cool hovering stuff if these are needed.
    // TODO: these do nothing now
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual std::shared_ptr<Interface::ActionInterface> getDragReleaseAction() override;

signals:
    void actionSent(std::shared_ptr<Interface::ActionInterface> action);
   // Interface::SendAgentAction(LocationItem *newLocItem, agentItem *aItem, LocationItem *oldLocItem=nullptr) asd;

private slots:
    void spawnDialogue();
    void clearResGainDisplay();

private:
    // The object from which this card takes its data (name, owner etc)
    std::shared_ptr<Interface::Agent> agentObject_;

    int agentConnections_;

    QPixmap *centerimage_;

    QColor playerColor_;

    // Empty dialog window to be called on mouse hover on/off
    AgentDialog* dialog_ = nullptr;

    // Timer used to spawn agent info dialog
    // TODO: apparently this does nothing
     QTimer* timer_;
    int dialogDelay_ = 1000;

    QString displayRes_ = "";
    QPixmap displayResSprite_;

};

#endif // AGENTITEM_HH
