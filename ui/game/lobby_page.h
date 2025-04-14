#pragma once

#include "../../common/base_page.h"
#include "../../model/player.h"

#include <QPushButton>
#include <QListView>
#include <QVBoxLayout>
#include <QListWidget>

class LobbyPage : public BasePage {
    Q_OBJECT

public:
    LobbyPage(Player&& player, QWidget* parent = nullptr);

    void onStart() override;

public Q_SLOTS:
    void receiveMessage(const InMessage& message) override;

private Q_SLOTS:
    void createGame();
    void updateGameList();
    void joinGame(QListWidgetItem* gameItem);

private:
    Player player_;

    QPushButton* createGameButton_;

    QPushButton* updateGameListButton_;
    QListWidget* gameListWidget_;
};
