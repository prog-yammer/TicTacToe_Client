#pragma once

#include "../../common/base_page.h"
#include "../../model/player.h"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

class GamePage : public BasePage
{
    Q_OBJECT

public:
    GamePage(QString&& gameId, std::optional<QString>&& opponentNickname, QWidget* parent = nullptr);

public Q_SLOTS:
    void receiveMessage(const InMessage& message) override;

private Q_SLOTS:
    void leave();
    void handleMove(int row, int col);

private:
    void setOpponent(const std::optional<QString>& playerNickaname);

    QString getTurnText() const;
    void changeTurn();

    bool playerTurn_;
    bool gameEnded_;
    bool hasOpponent_;
    QPushButton* board_[3][3];

    QLabel* opponentLabel_;
    QLabel* turnLabel_;
};
