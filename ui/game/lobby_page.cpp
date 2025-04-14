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
{
    auto* layout = new QVBoxLayout;
    this->setLayout(layout);

    auto nicknameLabel = new QLabel(this);
    nicknameLabel->setText(QLabel::tr("Your nickname: ") + player_.nickname);
    layout->addWidget(nicknameLabel);

    createGameButton_->setText(QLabel::tr("Create game"));
    connect(createGameButton_, &QPushButton::clicked, this, &LobbyPage::createGame);
    layout->addWidget(createGameButton_);

    updateGameListButton_->setText(QLabel::tr("Update"));
    connect(updateGameListButton_, &QPushButton::clicked, this, &LobbyPage::updateGameList);
    layout->addWidget(updateGameListButton_);

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
            QMessageBox::information(
                    this,
                    QMessageBox::tr("Notification"),
                    tr("This game is not available!"));
            updateGameList();
        }
        setEnabled(true);
        return;
    }

    switch (message.code) {
        case GAME_LIST: {
            gameListWidget_->clear();

            for (auto& gameInfo : message.message.split(' ')) {
                auto gameInfoParts = gameInfo.split('|');
                auto item = new QListWidgetItem(gameListWidget_);

                QVariant gameId = gameInfo.split('|')[0];
                item->setData(Qt::UserRole, gameId);

                auto GamePage = new GameItemWidget(std::move(gameInfo));
                item->setSizeHint(GamePage->sizeHint());
                gameListWidget_->setItemWidget(item, GamePage);
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

    emit sendMessage({.code = GET_GAMES});
}

void LobbyPage::joinGame(QListWidgetItem* gameItem)
{
    auto gameId = gameItem->data(Qt::UserRole).toString();

    setEnabled(false);
    emit sendMessage({.code = JOIN_GAME, .message = gameId});
}
