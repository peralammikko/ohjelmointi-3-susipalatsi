#include <QtTest>
#include <memory>


#include "../Student/agent.hh"
#include "commonresource.hh"

#include "../Course/game.h"

#include "gamescene.hh"
#include "../Student/agentitem.hh"


class StudentTest : public QObject
{
    Q_OBJECT
public:
    StudentTest();
private:
    QString test();
    std::shared_ptr<Interface::Game> game_;
    std::vector<std::shared_ptr<Interface::Location>> testLocations_;

private slots:
    void test_case1();
    void test_resources();
    void test_resources_data();

    void test_game_end();
    void test_game_end_data();


};

StudentTest::StudentTest()
{
    game_ = std::make_shared<Interface::Game>();
}

QString StudentTest::test()
{
    return "name";
}

void StudentTest::test_case1()
{
    //Interface::Game game_;

    game_->setActive(true);


    auto pl = std::make_shared<Interface::Player>(game_, 0, "");
    qDebug() << "hey!";

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

}

void StudentTest::test_game_end_data()
{

}




QTEST_APPLESS_MAIN(StudentTest)
#include "tst_studenttest.moc"

