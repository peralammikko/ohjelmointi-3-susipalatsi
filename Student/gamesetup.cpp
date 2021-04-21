#include "gamesetup.hh"
#include "gamescene.hh"


GameSetup::GameSetup(GameScene* gameScene, std::shared_ptr<Interface::Game> game, std::shared_ptr<GameRunner> courseRunner, std::shared_ptr<Logic> logic)
    : gameScene_(gameScene), game_(game), courseRunner_(courseRunner), logic_(logic)
{
    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
    reader.readSettings();

    game_.get()->connect(game_.get(), &Interface::Game::playerChanged, logic_.get(), &Logic::onPlayerChanged);
    courseRunner_->connect(courseRunner_.get(), &Interface::Runner::actionPerformed, logic_.get(), &Logic::onActionPerformed);

    initLocations();
    initResourceMaps();
    initDemandMaps();
    initLocItems();

    initLogic();

    initPlayers();
    initPlayerHands();
    addPlayerSetupCards();
    initPlayerControls();

    initAgentInterfaces();
    logic_->doTheRunning();

    // Need to tell gamescene who goes first so first player is not nullptr
    std::shared_ptr<Interface::Player> firstPlayer = game_->currentPlayer();
    gameScene_->turnInfo(firstPlayer);

}

void GameSetup::initLocations()
{
    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
    const int LOCATIONS = std::min(reader.getValue("LOCATIONS").toInt(), reader.getValue("MAX_LOCATIONS").toInt());

    // TODO: move names to settingsreader file maybe
    const std::vector<QString> paikat_ = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};
    // TODO: väärät resunimet
    const std::vector<QString> paikkaresut_ = {"Makkara", "pettuleipä", "absinttisnifferi", "kaks euroo", "tasan gramma", "krapulaa"};

    const std::vector<QString> councillors = {"KKK Kauppias", "Paavi", "Baarimikko", "Aallon kylteri", "Shaq O'Neil", "Muumipappa"};
    // Luodaan location-oliot
    for (int i = 0; i < LOCATIONS; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        std::shared_ptr<Interface::Councilor> areaCouncillor = std::make_shared<Interface::Councilor>(councillors.at(i), "Councillor", location);
        location->initialize();


        for  (int j = 0; j < 10; j++) {
            location->deck()->addCard(std::make_shared<Interface::CommonResource>(
                                          paikat_.at(i)+paikkaresut_.at(i), location, 1));
            location->deck()->addCard(std::make_shared<Interface::ActionCard>());
        }

        location->setCouncilor(areaCouncillor);
        location->deck()->shuffle();
        game_->addLocation(location);
    }
}

void GameSetup::initResourceMaps()
{
    int i = 0;
    const std::vector<QString> paikkaresut_ = {"Makkara", "pettuleipä", "absinttisnifferi", "kaks euroo", "tasan gramma", "krapulaa"};
    for (auto loc : game_->locations()) {
        QString resName = paikkaresut_.at(i);
        Interface::CommonResource res(resName, loc, 0);

        // Resource map for locations & runners
        std::pair<std::shared_ptr<Interface::Location>, Interface::CommonResource> pair(loc, res);
        initResourceMap_.insert(pair);

        // Resource map for agents
        std::pair<std::shared_ptr<Interface::Location>, std::deque<Interface::CommonResource>> pair2;
        pair2.first = loc;
        initAgentBackpack_.insert(pair2);

        i++;
    }
}

void GameSetup::initDemandMaps()
{
    ResourceMap::iterator it;
    for (auto pair : initResourceMap_) {
        auto location = pair.first;
        auto res = pair.second;

        // Make it so that location's demands can not be it's own resource
        while (true) {
            it = initResourceMap_.begin();
            int num = Interface::Random::RANDOM.uint(5);
            std::advance(it, num);
            if (it->first != location) {
                res = it->second;
                int amount = 2 + Interface::Random::RANDOM.uint(3);
                Interface::CommonResource demand(res.name(), it->first, amount);
                councillorDemandsMap_.insert({location, demand});
                break;
            }
        }
    }
}

void GameSetup::initLocItems()
{
    std::vector<std::shared_ptr<Interface::Location>> locvec = game_->locations();
    gameScene_->resourceInfo(initResourceMap_, councillorDemandsMap_);
    gameScene_->drawLocations(locvec);
}

void GameSetup::initLogic()
{
    logic_->infoResourceMaps(initResourceMap_, councillorDemandsMap_);
}

void GameSetup::initPlayers()
{
    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
    unsigned int playerCount = reader.getValue("PLAYERS").toInt();

    // TODO: Make names not hard coded maybe
    std::vector<QString> some_names = {"RED", "BLUE", "KALJAMIES", "KURKI", "NAPANUORA", "VAIKKU", "LASKIJA"};

    for (unsigned int i = 0; i < playerCount; ++i)
    {
        game_->addPlayer(std::make_shared<Interface::Player>(game_, i, some_names.at(i)));
    }
}

void GameSetup::initPlayerHands()
{
    auto players = game_->players();
    for (unsigned int i = 0; i < players.size(); ++i)
    {
        auto player = players.at(i);
        gameScene_->initHands(player);
        if ( i != 0)//player != game_->currentPlayer())
        {
            // Player who is not in turn has their hand hidden
            gameScene_->playerHands().at(player)->setEnabled(false);
            gameScene_->playerHands().at(player)->setY(0);
            gameScene_->playerHands().at(player)->setScale(0.5);

        } else {
            gameScene_->playerHands().at(player)->setY(650);
            gameScene_->playerHands().at(player)->setX(150);
        }
    }
}

void GameSetup::addPlayerSetupCards()
{
    CardItem* cardItem;
    for (unsigned int i=0; i<game_->players().size(); ++i) {
        std::shared_ptr<Interface::Player> player = game_->players().at(i);

        int cards = Interface::SettingsReader::READER.getValue("STARTING_AGENTS").toInt();

        for (int j=0; j < cards; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            player->addCard(card);

            cardItem = new CardItem(card, gameScene_);
            gameScene_->connect(cardItem, &CardItem::actionDeclared, gameScene_, &GameScene::onActionDeclared);
            gameScene_->addItem(cardItem);

            gameScene_->playerHands().at(player)->addMapItem(cardItem);
            cardItem = nullptr;
        }
    }
    delete cardItem;
}

void GameSetup::initPlayerControls()
{
    //std::shared_ptr<Interface::ManualControl> mancontrol = std::make_shared<Interface::ManualControl>();

    // TODO: maybe other than manual controls too like the AI Stuff? doubt we have time for that
    auto players = game_->players();
    for (unsigned int i = 0; i < players.size(); ++i)
    {
        // Does every player need its own control class?
        courseRunner_->setPlayerControl(players.at(i), std::make_shared<Interface::ManualControl>());
    }
}

void GameSetup::initAgentInterfaces()
{
    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
    const int AGENTCOUNT = reader.getValue("STARTING_AGENTS").toInt();

    // TODO: Make names not hard coded maybe
    std::vector<QString> some_names = {"PERRY", "KARHU", "VALDEMAR", "PONTIKKA", "KUMi",
                                       "KAHLAAJA", "VEITSI", "SAHANISKA", "KRAPULA", "VAHTIMESTARI", "LAKRITSIPORTTERI"};
    auto players = game_->players();
    for (unsigned int i = 0; i < players.size(); ++i)
    {
        auto player = players.at(i);
        auto hand = gameScene_->playerHands().at(player);
        for (unsigned int j = 0; j < AGENTCOUNT; ++j)
        {
            // Sort of randomize a name
            unsigned int pseudorandomIndex = pow((i+1)*j, 3);
            pseudorandomIndex = pseudorandomIndex % (some_names.size()-1);
            // create the interface
            std::shared_ptr<Interface::Agent> agentInter = std::make_shared<Interface::Agent>(some_names.at(pseudorandomIndex) + player->name(), player);
            agentInter->initAgentResources(initAgentBackpack_);
            player->addCard(agentInter);

            // create the agent item that is movable on the scene and add it to the scene
            agentItem* agenttiesine = new agentItem(agentInter); // should this be a shared_ptr instead?
            gameScene_->connect(agenttiesine, &agentItem::actionDeclared, gameScene_, &GameScene::onActionDeclared);
            gameScene_->addItem(agenttiesine);
            hand->addMapItem(agenttiesine);
        }
    }
}
