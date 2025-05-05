#include <QtTest>
#include "../src/ui/game/lobby_page.h"
#include "../src/ui/game/game_page.h"

class LobbyTest : public QObject
{
Q_OBJECT

private slots:

    void test_send_message()
    {
        LobbyPage lobbyPage((Player(player)));
        auto createGameButton = lobbyPage.findChild<QPushButton*>("createGameButton");
        auto updateGameListButton = lobbyPage.findChild<QPushButton*>("updateGameListButton");

        QSignalSpy sendMessageSpy(&lobbyPage, &BasePage::sendMessage);
        QTest::mouseClick(createGameButton, Qt::LeftButton);
        QCOMPARE(sendMessageSpy.count(), 1);

        QList<QVariant> arguments = sendMessageSpy.takeFirst();
        QCOMPARE(arguments.at(0).value<OutMessage>().code, OutCommandCode::CREATE_GAME);
        QCOMPARE(lobbyPage.isEnabled(), false);

        lobbyPage.setEnabled(true);
        QTest::mouseClick(updateGameListButton, Qt::LeftButton);
        QCOMPARE(sendMessageSpy.count(), 1);

        arguments = sendMessageSpy.takeFirst();
        QCOMPARE(arguments.at(0).value<OutMessage>().code, OutCommandCode::GET_GAMES);
    }

    void test_receive_message()
    {
        LobbyPage lobbyPage((Player(player)));
        auto gameListWidget = lobbyPage.findChild<QListWidget*>("gameListWidget");

        lobbyPage.receiveMessage({.code = GAME_LIST, .message = "0|p1 1|p2"});
        QCOMPARE(gameListWidget->count(), 2);

        QSignalSpy forwardSpy(&lobbyPage, &BasePage::forward);
        lobbyPage.receiveMessage({.code = GAME_CREATED, .message = "2"});
        QCOMPARE(forwardSpy.count(), 1);
        QList<QVariant> arguments = forwardSpy.takeFirst();

        auto gamePage = arguments.at(0).value<GamePage*>();
        QVERIFY(gamePage != nullptr);

        lobbyPage.receiveMessage({.code = JOINED_GAME, .message = "0 p1"});
        QCOMPARE(forwardSpy.count(), 1);
        arguments = forwardSpy.takeFirst();

        gamePage = arguments.at(0).value<GamePage*>();
        QVERIFY(gamePage != nullptr);
    }

    void test_join_game()
    {
        LobbyPage lobbyPage((Player(player)));
        auto gameListWidget = lobbyPage.findChild<QListWidget*>("gameListWidget");

        lobbyPage.receiveMessage({.code = GAME_LIST, .message = "0|p1 1|p2"});
        QCOMPARE(gameListWidget->count(), 2);

        QSignalSpy sendMessageSpy(&lobbyPage, &BasePage::sendMessage);
        gameListWidget->itemClicked(gameListWidget->item(0));

        QCOMPARE(sendMessageSpy.count(), 1);

        QList<QVariant> arguments = sendMessageSpy.takeFirst();
        QCOMPARE(arguments.at(0).value<OutMessage>().code, OutCommandCode::JOIN_GAME);
    };

private:
    Player player = {.id = "0", .nickname = "nickname"} ;
};

QTEST_MAIN(LobbyTest)
#include "lobby_test.moc"
