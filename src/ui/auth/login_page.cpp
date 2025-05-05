#include "login_page.h"

#include "../game/lobby_page.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

LoginPage::LoginPage(QWidget* parent)
    : BasePage(parent)
    , nicknameLineEdit_(new QLineEdit(this))
    , authButton_(new QPushButton(this))
{
    auto* mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    auto titleLabel = new QLabel(this);
    titleLabel->setText(QLabel::tr("Authorization"));
    titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLabel->setStyleSheet("QLabel {"
                              "font-size: 28px;"
                              "font-weight: bold;"
                              "border: 2px solid black;"
                              "}");
    mainLayout->addWidget(titleLabel);

    auto contentLayout = new QHBoxLayout;
    mainLayout->addLayout(contentLayout);

    auto formLayout = new QVBoxLayout;
    contentLayout->addLayout(formLayout, 2);

    formLayout->addStretch();

    auto validator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9]*"), nicknameLineEdit_);
    nicknameLineEdit_->setObjectName("nicknameLineEdit");
    nicknameLineEdit_->setValidator(validator);
    nicknameLineEdit_->setPlaceholderText(QPushButton::tr("Enter your nickname"));
    nicknameLineEdit_->setStyleSheet("QLineEdit {"
                                     "font-size: 16px;"
                                     "padding: 2px;"
                                     "}");
    formLayout->addWidget(nicknameLineEdit_);

    authButton_->setObjectName("authButton");
    authButton_->setText(QPushButton::tr("Auth"));
    authButton_->setStyleSheet("QPushButton {"
                               "font-size: 16px;"
                               "padding: 5px;"
                               "}");
    formLayout->addWidget(authButton_);

    connect(authButton_, &QPushButton::clicked, this, &LoginPage::authorize);

    formLayout->addStretch();

    auto welcomeLabel = new QLabel(this);
    welcomeLabel->setText(tr(
            "Welcome to the best version of TicTacToe! Before you begin play, enter your nickname.\nGood luck!"
    ));
    welcomeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    welcomeLabel->setWordWrap(true);
    welcomeLabel->setStyleSheet("QLabel {"
                              "font-size: 20px;"
                              "margin-left: 20px;"
                              "margin-right: 20px;"
                              "}");
    contentLayout->addWidget(welcomeLabel, 3);
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
