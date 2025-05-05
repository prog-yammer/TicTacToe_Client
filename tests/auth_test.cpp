#include <QtTest>
#include "../src/ui/auth/login_page.h"
#include "../src/ui/game/lobby_page.h"

class AuthTest : public QObject
{
    Q_OBJECT

private slots:
    void test_empty_nickname()
    {
        LoginPage loginPage;
        auto authButton = loginPage.findChild<QPushButton*>("authButton");
        auto nicknameLineEdit = loginPage.findChild<QLineEdit*>("nicknameLineEdit");

        QSignalSpy sendMessageSpy(&loginPage, &BasePage::sendMessage);
        QTest::mouseClick(authButton, Qt::LeftButton);
        QCOMPARE(sendMessageSpy.count(), 0);
    }

    void test_authorize()
    {
        LoginPage loginPage;
        auto authButton = loginPage.findChild<QPushButton*>("authButton");
        auto nicknameLineEdit = loginPage.findChild<QLineEdit*>("nicknameLineEdit");

        QString nickname = "nickname";
        nicknameLineEdit->setText(nickname);

        QSignalSpy sendMessageSpy(&loginPage, &BasePage::sendMessage);
        QTest::mouseClick(authButton, Qt::LeftButton);
        QCOMPARE(sendMessageSpy.count(), 1);

        QTest::keyClick(&loginPage, Qt::Key_Return);
        QCOMPARE(sendMessageSpy.count(), 2);

        QSignalSpy forwardSpy(&loginPage, &BasePage::forward);
        loginPage.receiveMessage({
            .code = InCommandCode::PLAYER_AUTHED,
            .message = "0",
         });
        QCOMPARE(forwardSpy.count(), 1);
        QList<QVariant> arguments = forwardSpy.takeFirst();

        auto lobbyPage = arguments.at(0).value<LobbyPage*>();
        QVERIFY(lobbyPage != nullptr);
        QCOMPARE(arguments.at(1).toBool(), true);
    }
};

QTEST_MAIN(AuthTest)
#include "auth_test.moc"