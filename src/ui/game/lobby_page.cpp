#include "lobby_page.h"
#include "game_item_widget.h"
#include "game_page.h"

#include <QLabel>
#include <QStringListModel>
#include <QListWidgetItem>

LobbyPage::LobbyPage(Player&& player, QWidget *parent)
    : BasePage(parent)
    , player_(std::move(player))
    , createGameButton_(new QPushButton(this))
    , updateGameListButton_(new QPushButton(this))
    , gameListWidget_(new QListWidget(this))
    , infoLabel_(new QLabel(this))
{
    auto* layout = new QVBoxLayout;
    this->setLayout(layout);

    auto nicknameLabel = new QLabel(this);
    nicknameLabel->setObjectName("nicknameLabel");
    nicknameLabel->setText(QLabel::tr("Your nickname: %1").arg(player_.nickname));
    nicknameLabel->setStyleSheet("QLabel {"
                              "font-size: 20px;"
                              "font-weight: bold;"
                              "}");

    layout->addWidget(nicknameLabel);

    auto buttonsLayout = new QHBoxLayout;
    layout->addLayout(buttonsLayout);

    createGameButton_->setObjectName("createGameButton");
    createGameButton_->setText(QLabel::tr("Create game"));
    createGameButton_->setStyleSheet("QPushButton {"
                                     "font-size: 16px;"
                                     "padding: 10px;"
                                     "}");
    connect(createGameButton_, &QPushButton::clicked, this, &LobbyPage::createGame);
    buttonsLayout->addWidget(createGameButton_, 1);

    updateGameListButton_->setObjectName("updateGameListButton");
    updateGameListButton_->setText(QLabel::tr("Update"));
    updateGameListButton_->setStyleSheet("QPushButton {"
                                     "font-size: 16px;"
                                     "padding: 10px;"
                                     "}");
    connect(updateGameListButton_, &QPushButton::clicked, this, &LobbyPage::updateGameList);
    buttonsLayout->addWidget(updateGameListButton_, 1);

    layout->addWidget(infoLabel_);

    gameListWidget_->setObjectName("gameListWidget");
    gameListWidget_->setStyleSheet(
            "QListWidget::item {"
            "   border: 1px solid #999;"
            "   border-radius: 3px;"
            "   padding: 2px;"
            "   margin: 2px;"
            "}"
            "QListWidget::item::hover {"
            "   border: 3px solid #999;"
            "}");
    connect(gameListWidget_, &QListWidget::itemClicked, this, &LobbyPage::joinGame);
    layout->addWidget(gameListWidget_);
}

void LobbyPage::onStart()
{
    updateGameList();
}

void LobbyPage::receiveMessage(const InMessage& message)
{
    if (message.errorCode) {
        if (message.errorCode == ErrorCode::ERROR_JOIN) {
            updateGameList();
            infoLabel_->setText(tr("This game is not available!"));
        }
        setEnabled(true);
        return;
    }

    switch (message.code) {
        case GAME_LIST: {
            gameListWidget_->clear();

            if (!message.message.isEmpty()) {
                for (auto& gameInfo : message.message.split(' ')) {
                    auto item = new QListWidgetItem(gameListWidget_);

                    QVariant gameId = gameInfo.split('|')[0];
                    item->setData(Qt::UserRole, gameId);

                    auto gameItemWidget = new GameItemWidget(std::move(gameInfo));
                    item->setSizeHint(gameItemWidget->sizeHint());
                    gameListWidget_->setItemWidget(item, gameItemWidget);
                }
            }

            updateGameListButton_->setEnabled(true);
            gameListWidget_->setEnabled(true);
            break;
        }
        case GAME_CREATED: {
            auto gamePage = new GamePage(QString(message.message), std::nullopt, parentWidget());
            emit forward(gamePage);
            setEnabled(true);
            break;
        }
        case JOINED_GAME: {
            auto messageParts = message.message.split(' ');
            auto gamePage = new GamePage(std::move(messageParts[0]), std::move(messageParts[1]), parentWidget());
            emit forward(gamePage);
            setEnabled(true);
            break;
        }
        default:
            setEnabled(true);
    }
}

void LobbyPage::createGame()
{
    setEnabled(false);
    emit sendMessage({.code = CREATE_GAME});
}

void LobbyPage::updateGameList()
{
    updateGameListButton_->setEnabled(false);
    gameListWidget_->setEnabled(false);
    infoLabel_->setText("");

    emit sendMessage({.code = GET_GAMES});
}

void LobbyPage::joinGame(QListWidgetItem* gameItem)
{
    auto gameId = gameItem->data(Qt::UserRole).toString();

    setEnabled(false);
    emit sendMessage({.code = JOIN_GAME, .message = gameId});
}
