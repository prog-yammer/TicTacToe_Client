#include "login_page.h"

#include "../game/lobby_page.h"

#include <QVBoxLayout>
#include <QPushButton>

LoginPage::LoginPage(QWidget* parent)
    : BasePage(parent)
    , nicknameLineEdit_(new QLineEdit(this))
    , authButton_(new QPushButton(this))
{
    auto* layout = new QHBoxLayout;
    this->setLayout(layout);

    auto validator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9]*"), nicknameLineEdit_);
    nicknameLineEdit_->setValidator(validator);
    nicknameLineEdit_->setPlaceholderText(QPushButton::tr("Enter your nickname"));
    layout->addWidget(nicknameLineEdit_);

    authButton_->setText(QPushButton::tr("Auth"));
    layout->addWidget(authButton_);

    connect(authButton_, &QPushButton::clicked, this, &LoginPage::authorize);
}

void LoginPage::receiveMessage(const InMessage& message)
{
    if (message.code != PLAYER_AUTHED) {
        nicknameLineEdit_->setEnabled(true);
        return;
    }

    Player p {
        .id = message.message,
        .nickname = nicknameLineEdit_->text().trimmed(),
    };

    auto lobbyPage = new LobbyPage(std::move(p), parentWidget());
    emit forward(lobbyPage, true);
}

void LoginPage::authorize()
{
    if (nicknameLineEdit_->text().trimmed().isEmpty())
        return;

    nicknameLineEdit_->setEnabled(false);
    auto nickname = nicknameLineEdit_->text().trimmed();

    emit sendMessage(OutMessage {
        .code = AUTH,
        .message = nickname,
    });
}

void LoginPage::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        authorize();
    }
}
