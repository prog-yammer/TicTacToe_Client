#include <QtTest>
#include "../src/ui/game/game_page.h"

class GameTest : public QObject
{
Q_OBJECT

private slots:
    void test_send_message()
    {
        GamePage gamePage(QString(gameId), std::nullopt);
        auto leaveButton = gamePage.findChild<QPushButton*>("leaveButton");

        QSignalSpy sendMessageSpy(&gamePage, &BasePage::sendMessage);
        QTest::mouseClick(leaveButton, Qt::LeftButton);
        QCOMPARE(sendMessageSpy.count(), 1);

        QList<QVariant> arguments = sendMessageSpy.takeFirst();
        QCOMPARE(arguments.at(0).value<OutMessage>().code, OutCommandCode::LEAVE_GAME);

        gamePage.receiveMessage({.code = OPPONENT_JOINED, .message = player.nickname});
        auto moveButton00 = gamePage.findChild<QPushButton*>("moveButton 0 0");
        QVERIFY(moveButton00 != nullptr);
        QTest::mouseClick(moveButton00, Qt::LeftButton);
        QCOMPARE(sendMessageSpy.count(), 1);

        auto message = sendMessageSpy.takeFirst().at(0).value<OutMessage>();
        QCOMPARE(message.code, OutCommandCode::MOVE);
        QCOMPARE(message.message, "0 0");
    }

    void test_receive_message()
    {
        GamePage gamePage(QString(gameId), std::nullopt);
        auto moveButton00 = gamePage.findChild<QPushButton*>("moveButton 0 0");

        QSignalSpy backSpy(&gamePage, &BasePage::back);
        gamePage.receiveMessage({.code = LEFT_GAME});
        QCOMPARE(backSpy.count(), 1);

        QCOMPARE(moveButton00->text(), "");
        gamePage.receiveMessage({.code = MOVED, .message = "0 0 X"});
        QCOMPARE(moveButton00->text(), "X");

        gamePage.receiveMessage({.code = MOVED, .message = "0 0 X"});

    }

private:
    QString gameId = "0";
    Player player = {.id = "0", .nickname = "nickname"} ;
};

QTEST_MAIN(GameTest)
#include "game_test.moc"