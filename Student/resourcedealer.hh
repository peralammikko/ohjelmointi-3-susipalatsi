#ifndef RESOURCEDEALER_HH
#define RESOURCEDEALER_HH

#include <QObject>
#include "gamescene.hh"
#include "commonresource.hh"
#include "../Course/location.h"

class ResourceDealer : public QObject
{
    Q_OBJECT
public:
    explicit ResourceDealer(GameScene* gameScene, std::shared_ptr<Interface::Game> game);
    void infoResourceMaps(ResourceMap &rmap, ResourceMap &dmap);

public slots:
    void onReadyForResources();
private:
    void rewardResources();
    GameScene* gameScene_;
    std::shared_ptr<Interface::Game> game_;

    ResourceMap resMap_;
    ResourceMap demandsMap_;


signals:

};

#endif // RESOURCEDEALER_HH
