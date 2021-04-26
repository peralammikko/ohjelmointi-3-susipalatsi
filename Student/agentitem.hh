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
    /**
     * @brief Get the object the AgentItem is connected to for methods
     * @return Returns the Agent object
     * @pre AgentObject exists
     */
    std::shared_ptr<Interface::Agent> getAgentClass();

    /**
     * @brief MapItem class overrides
     */
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    /**
     * @brief Returns the name of the type of this object
     * @return Return a string of this objects type
     */
    const QString typeOf() override;

    /**
     * @brief rearrange
     */
    void rearrange() override{};

    /**
     * @brief Displays the resources an AgentItem on scene may have received during reward distribution (Event phase).
     * @param amount: How much resource an agent received
     * @param path: Sprite for corresponding resource
     * @pre AgentItem is on a location and not in hand
     */
    void displayResourceChange(int amount, QString path); // TODO: maybe add sprite too?

protected:
    // some cool hovering stuff if these are needed.
    // TODO: these do nothing now
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual std::shared_ptr<Interface::ActionInterface> getDragReleaseAction() override;

signals:
    /**
     * @brief actionSent
     * @param action
     */
    void actionSent(std::shared_ptr<Interface::ActionInterface> action);

private slots:
    /**
     * @brief Creates a small popup dialog used for checking agent's resources
     */
    void spawnDialogue();

    /**
     * @brief Clears the small display of how much an agent received resources during Event phase
     */
    void clearResGainDisplay();

private:

    /**
     * @brief The base class from which this card takes its data (name, owner etc)
     */
    std::shared_ptr<Interface::Agent> agentObject_;

    int agentConnections_;

    QPixmap *centerimage_;

    QColor playerColor_;

    /**
     * @brief Empty dialog window to be called on mouse hover on/off
     */
    AgentDialog* dialog_ = nullptr;

    /**
     * @brief Delay used for spawning the agent info dialog
     */
    int dialogDelay_ = 1000;

    QString displayRes_ = "";
    QPixmap displayResSprite_;

};

#endif // AGENTITEM_HH
