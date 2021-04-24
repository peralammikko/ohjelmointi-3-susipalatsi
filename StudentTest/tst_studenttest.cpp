#include <QtTest>
#include <memory>
#include "../Student/agent.hh"
#include "../Student/agent.cpp"
#include "../Course/game.h"

class StudentTest : public QObject
{
    Q_OBJECT

public:
    StudentTest();
    ~StudentTest();

private slots:
    void test_case1();

};

StudentTest::StudentTest()
{

}

StudentTest::~StudentTest()
{

}

void StudentTest::test_case1()
{
   QFETCH(QString, locname);
   QFETCH(QString, plname);
   QFETCH(QString, agname);
   QFETCH(unsigned short, id);

   std::shared_ptr<Interface::Game> game = std::make_shared<Interface::Game>();
  // std::shared_ptr<Interface::Location> loc = std::make_shared<Interface::Location>(id, locname);
   std::shared_ptr<Interface::Player> pl = std::make_shared<Interface::Player>(game, id, plname);
   std::weak_ptr<Interface::Player> pl2 = pl;
  // std::shared_ptr<Interface::Agent> ag = std::make_shared<Interface::Agent>(agname, pl2);


//   Agent::Agent(QString name, std::weak_ptr<Player> owner)

   QCOMPARE(pl->name(), plname);

}

QTEST_APPLESS_MAIN(StudentTest)

#include "tst_studenttest.moc"
