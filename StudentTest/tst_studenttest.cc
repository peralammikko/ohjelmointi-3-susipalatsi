#include <QtTest>
#include <memory>
#include <algorithm>

#include "../Student/agent.hh"
#include "commonresource.hh"
#include "../Course/game.h"
#include "../Student/agentitem.hh"
#include "settingsreader.h"
#include "../course/Course/runner.h"
#include "../Student/logic.hh"
#include "../course/Course/councilor.h"


class StudentTest : public QObject
{
    Q_OBJECT
public:
    StudentTest();
private:

private slots:
    void test_resources();
    void test_resources_data();

    void test_game_end();
    void test_game_end_data();


};

StudentTest::StudentTest()
{

}

void StudentTest::test_resources()
{
    ResourceMap resMap = {};
    auto location1 = std::make_shared<Interface::Location>(0, "");
    auto location2 = std::make_shared<Interface::Location>(1, "");
    auto agent1 = std::make_shared<Interface::Agent>("", std::shared_ptr<Interface::Player>());

    QFETCH(unsigned int, addedResource);
    QFETCH(unsigned int, removedResource);
    QFETCH(unsigned int, expectedResource);

    // According to our contract, the agent must first have initialized resource map before we can add resources to it
    AgentResourceMap agResMap;
    for (auto loc : {location1, location2}){
        auto resource = std::make_shared<Interface::CommonResource>(QString::number(loc->id()), loc, QString::number(loc->id()));
        resMap.insert(std::make_pair(loc, resource));
        std::pair<std::shared_ptr<Interface::Location>, std::deque<std::shared_ptr<Interface::CommonResource>>> pair2;
        pair2.first = loc;
        agResMap.insert(pair2);
    }
    agent1->initAgentResources(agResMap);

    // Preforming the test
    agent1->addResource(location1, resMap.at(location1), addedResource);
    QVERIFY2(agent1->getAgentResources().at(location1).size() == addedResource, "Incorrect resource amount");
    QVERIFY2(agent1->getAgentResources().at(location2).size() == 0, "Wrong location had its resources changed");

    agent1->removeResource(location1, removedResource);
    QVERIFY2(agent1->getAgentResources().at(location1).size() == expectedResource, "Resource removal failure");
}

void StudentTest::test_resources_data()
{
    QTest::addColumn<unsigned int>("addedResource");
    QTest::addColumn<unsigned int>("removedResource");
    QTest::addColumn<unsigned int>("expectedResource");

    QTest::newRow("4-0 = 4")    << 4u << 0u << 4u;
    QTest::newRow("1-1 = 0")    << 1u << 1u << 0u;
    QTest::newRow("2-1 = 1")    << 2u << 1u << 1u;
    QTest::newRow("7-5 = 2")    << 7u << 5u << 2u;
    // QTest::newRow("2-5 = 0")    << 2u << 5u << 0u;
}

void StudentTest::test_game_end()
{
    auto game = std::make_shared<Interface::Game>();
    auto runner = std::make_shared<Interface::Runner>(game);
    auto logic = std::make_shared<Logic>(runner, game);

    QFETCH(unsigned int, numberOfCouncilorCards0);
    QFETCH(unsigned int, numberOfCouncilorCards1);
    QFETCH(unsigned int, requiredForVictory);
    QFETCH(bool, player0ExpectedToWin);
    QFETCH(bool, player1ExpectedToWin);

    auto player0 = std::make_shared<Interface::Player>(game, 0, "0");
    game->addPlayer(player0);
    auto player1 = std::make_shared<Interface::Player>(game, 1, "1");
    game->addPlayer(player1);

    for (unsigned int i = 0; i < numberOfCouncilorCards0; ++i){
        player0->addCard(std::make_shared<Interface::Councilor>("", "", std::shared_ptr<Interface::Location>()));
    }
    for (unsigned int i = 0; i < numberOfCouncilorCards1; ++i){
        player1->addCard(std::make_shared<Interface::Councilor>("", "", std::shared_ptr<Interface::Location>()));
    }

    std::set<std::shared_ptr<Interface::Player>> winners = logic->checkWin({player0, player1});
    QVERIFY2(player0ExpectedToWin == (std::find(winners.begin(), winners.end(), player0) != winners.end()), "Wrong win state for player0");
    QVERIFY2(player1ExpectedToWin == (std::find(winners.begin(), winners.end(), player1) != winners.end()), "Wrong win state for player1");

}

void StudentTest::test_game_end_data()
{
    QTest::addColumn<unsigned int>("numberOfCouncilorCards0");
    QTest::addColumn<unsigned int>("numberOfCouncilorCards1");
    QTest::addColumn<unsigned int>("requiredForVictory");
    QTest::addColumn<bool>("player0ExpectedToWin");
    QTest::addColumn<bool>("player1ExpectedToWin");

    QTest::newRow("3/3, 0/3") << 3u << 0u << 3u << true << false;
    QTest::newRow("2/3, 2/3") << 2u << 2u << 3u << false << false;
    QTest::newRow("4/4, 4/4") << 4u << 4u << 4u << true << true;
    QTest::newRow("2/5, 8/5") << 2u << 8u << 5u << false << true;
    QTest::newRow("0/0, 0/0") << 0u << 0u << 0u << false << false;
}




QTEST_APPLESS_MAIN(StudentTest)
#include "tst_studenttest.moc"

