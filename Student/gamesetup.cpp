#include <QDirIterator>
#include "gamesetup.hh"
#include "gamescene.hh"
#include "startingscreen.hh"
#include "settingsscreen.hh"

GameSetup::GameSetup(GameScene* gameScene, std::shared_ptr<Interface::Game> game, std::shared_ptr<GameRunner> courseRunner, std::shared_ptr<Logic> logic, std::vector<QString> playerNames, std::vector<int> customSettings)
    : gameScene_(gameScene), game_(game), courseRunner_(courseRunner), logic_(logic)
{
    Interface::SettingsReader& reader = Interface::SettingsReader::READER;
    reader.readSettings();
    PLAYERCOUNT = reader.getValue("PLAYERS").toInt();

    game_.get()->connect(game_.get(), &Interface::Game::playerChanged, logic_.get(), &Logic::onPlayerChanged);
    courseRunner_->connect(courseRunner_.get(), &Interface::Runner::actionPerformed, logic_.get(), &Logic::onActionPerformed);

    checkStartingInfo(playerNames, customSettings);

    initLocations();
    initResourceMaps();
    initDemandMaps();
    initLocItems();
    initLocDecks();

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

void GameSetup::checkStartingInfo(std::vector<QString> names, std::vector<int> settings)
{
    std::vector<QString> some_names = {"RED", "BLUE", "KALJAMIES", "KURKI", "NAPANUORA", "VAIKKU", "LASKIJA"};

    for (unsigned int i = 0; i < names.size(); i++ ) {
        if (names.at(i).trimmed().isEmpty()) {
            playerNames_.push_back(some_names.at(i));
        } else {
            playerNames_.push_back(names.at(i));
        }
    }
    PLAYERCOUNT = playerNames_.size();

    if (settings.size() != 0) {
        useCustomSettings = true;
        AGENTCOUNT = settings.at(0);
        LOCATIONS = settings.at(1);
        WINCONDITION = settings.at(2);
    } else {
        useCustomSettings = false;
    }

}

void GameSetup::initLocations()
{
    if (useCustomSettings == false) {
        Interface::SettingsReader& reader = Interface::SettingsReader::READER;
        LOCATIONS = std::min(reader.getValue("LOCATIONS").toInt(), reader.getValue("MAX_LOCATIONS").toInt());
    }

    // TODO: move names to settingsreader file maybe
    const std::vector<QString> paikat_ = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};
    const std::vector<QString> councillors = {"KKK Kauppias", "Paavi", "Baarimikko", "Aallon kylteri", "Shaq O'Neil", "Muumipappa"};

    // Luodaan location-oliot
    for (unsigned int i = 0; i < LOCATIONS; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        std::shared_ptr<Interface::Councilor> areaCouncillor = std::make_shared<Interface::Councilor>(councillors.at(i), "Councillor", location);
        location->initialize();

        location->setCouncilor(areaCouncillor);

        game_->addLocation(location);
    }
}

void GameSetup::initResourceMaps()
{
    int i = 0;
    QDirIterator it(":/img/res/img/resources/");
    std::vector<std::pair<QString,QString>> resources;
    while (it.hasNext()) {
        QString path = it.next();
        QString name = (path.split("/").end()-1)->split(".").at(0);
        resources.push_back({name, path});
    }
    for (auto loc : game_->locations()) {
        //QString resName = paikkaresut_.at(i);
        Interface::CommonResource res(resources.at(i).first, loc, resources.at(i).second,0);

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
                Interface::CommonResource demand(res.name(), it->first, res.getSpritePath(), amount);
                councillorDemandsMap_.insert({location, demand});
                break;
            }
        }
    }
}

void GameSetup::initLocItems()
{
    std::vector<std::shared_ptr<Interface::Location>> locvec = game_->locations();
    std::vector<std::pair<std::shared_ptr<Interface::Location>, std::vector<std::pair<QString, QString>>>> locationInformation;
    std::unique_ptr<QDirIterator> it;

    std::vector<std::pair<QString, QString>> types;
    types = {{"governorLbl",":/img/governors/img/governors/"},
            {"planet",":/img/planets/img/some sprites"},
            {"resourceLbl",":/img/res/img/resources/"}};

    std::map<QString, std::vector<QString>> typedata;
    std::vector<std::pair<QString, QString>>::iterator type = types.begin();
    while (type != types.end()){
        std::vector<QString> paths = {};
        it.reset(new QDirIterator(type->second, QDirIterator::Subdirectories));
        while (it->hasNext()) {
            paths.push_back(it->next());
        }
        Q_ASSERT(paths.size() >= locvec.size());
        typedata[type->first] = paths;
        ++type;
    }

    int num = 0;
    QString spritePath = "";
    QString key = "";
    for (unsigned int i = 0; i < locvec.size(); ++i){
        auto loc = locvec.at(i);
        std::vector<std::pair<QString, QString>> pathData;
        for (unsigned int j = 0; j < types.size(); ++j){
            key = types.at(j).first;
            std::vector<QString>* spritePaths = &typedata.at(key);
            num = Interface::Random::RANDOM.uint(spritePaths->size()-1);
            spritePath = spritePaths->at(num);
            pathData.push_back({key, spritePath});
            spritePaths->erase(spritePaths->begin() + num);
            spritePath = "";
            key = "";
        }
        locationInformation.push_back({loc, pathData});
    }

    gameScene_->resourceInfo(initResourceMap_, councillorDemandsMap_);

    gameScene_->drawLocations(locationInformation);
}

void GameSetup::initLocDecks()
{
    auto locations = gameScene_->GetLocItems();
    for (unsigned int i=0; i < locations.size(); ++i){
        auto location = locations.at(i)->getObject();
        auto resource = locations.at(i)->getLocalResource();

            for  (int j = 0; j < 10; j++) {
                location->deck()->addCard(std::make_shared<Interface::CommonResource>(resource.name(), location, resource.getSpritePath(), 1));
                location->deck()->addCard(std::make_shared<Interface::ActionCard>());
            }
            location->deck()->shuffle();
        }

}

void GameSetup::initLogic()
{
    if (useCustomSettings == false) {
        WINCONDITION = 3;
    }
    logic_->infoResourceMaps(initResourceMap_, councillorDemandsMap_, WINCONDITION);
}

void GameSetup::initPlayers()
{
    for (unsigned int i = 0; i < PLAYERCOUNT; ++i)
    {
        game_->addPlayer(std::make_shared<Interface::Player>(game_, i, playerNames_.at(i)));
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
    int cards = AGENTCOUNT;
    if (useCustomSettings == false) {
        cards = Interface::SettingsReader::READER.getValue("STARTING_AGENTS").toInt();
    }

    for (unsigned int i=0; i<game_->players().size(); ++i) {
        std::shared_ptr<Interface::Player> player = game_->players().at(i);

        for (int j=0; j < cards; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            player->addCard(card);
            gameScene_->addActionCardForPlayer(player, card);
        }
    }

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
    if (useCustomSettings == false) {
        Interface::SettingsReader& reader = Interface::SettingsReader::READER;
        AGENTCOUNT = reader.getValue("STARTING_AGENTS").toInt();
    }

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
