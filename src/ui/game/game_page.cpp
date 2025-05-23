#include "game_page.h"

GamePage::GamePage(QString&& gameId, std::optional<QString>&& opponentNickname, QWidget* parent)
    : BasePage(parent)
    , opponentLabel_(new QLabel(this))
    , turnLabel_(new QLabel(this))
    , playerTurn_(!opponentNickname.has_value())
{
    auto layout = new QVBoxLayout;
    this->setLayout(layout);

    auto topLayout = new QHBoxLayout;
    layout->addLayout(topLayout);

    auto gameIdLabel = new QLabel(this);
    gameIdLabel->setText(QPushButton::tr("Game id: %1").arg(gameId));
    gameIdLabel->setStyleSheet("QLabel {"
                               "font-size: 20px;"
                               "font-weight: bold;"
                               "}");
    topLayout->addWidget(gameIdLabel);

    auto leaveButton = new QPushButton(this);
    leaveButton->setObjectName("leaveButton");
    leaveButton->setText(QPushButton::tr("Leave game"));
    leaveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leaveButton->setStyleSheet("QPushButton {"
                               "padding: 5px;"
                               "border: 2px solid red;"
                               "}");
    connect(leaveButton, &QPushButton::clicked, this, &GamePage::leave);
    topLayout->addWidget(leaveButton);

    auto infoLayout = new QHBoxLayout;
    layout->addLayout(infoLayout);

    infoLayout->setContentsMargins(0, 15, 0, 2);

    QFont infoFont;
    infoFont.setPointSize(16);

    setOpponent(opponentNickname);
    opponentLabel_->setFont(infoFont);
    opponentLabel_->setAlignment(Qt::AlignLeft);
    infoLayout->addWidget(opponentLabel_);

    turnLabel_->setText(getTurnText());
    turnLabel_->setAlignment(Qt::AlignRight);
    turnLabel_->setFont(infoFont);
    infoLayout->addWidget(turnLabel_);

    auto boardLayout = new QHBoxLayout;
    layout->addLayout(boardLayout);
    boardLayout->addStretch();

    auto boardWidget = new QWidget(this);
    boardLayout->addWidget(boardWidget);

    boardWidget->setStyleSheet("QPushButton, QPushButton:pressed { "
                               "margin: 0;"
                               "padding: 0;"
                               "border: 1px solid black;"
                               "background: gray;"
                               "}");

    auto gridLayout = new QGridLayout(boardWidget);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    int buttonSize = 150;

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            auto moveButton = new QPushButton(this);
            moveButton->setObjectName(QString("moveButton %1 %2").arg(row).arg(col));
            moveButton->setFixedSize(buttonSize, buttonSize);
            moveButton->setFont(QFont("Arial", 64));
            moveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            connect(moveButton, &QPushButton::clicked, this, [=]() {
                handleMove(row, col);
            });

            gridLayout->addWidget(moveButton, row, col);
            board_[row][col] = moveButton;
            gridLayout->setColumnMinimumWidth(col, buttonSize);
        }
        gridLayout->setRowMinimumHeight(row, buttonSize);
    }

    boardWidget->setFixedSize(buttonSize * 3, buttonSize * 3);
    boardLayout->addStretch();
}

void GamePage::receiveMessage(const InMessage& message)
{
    switch (message.code) {
        case LEFT_GAME: {
            emit back();
            break;
        }
        case OPPONENT_JOINED: {
            setOpponent(message.message);
            break;
        }
        case MOVED: {
            auto messageParts = message.message.split(' ');
            auto x = messageParts[0].toInt();
            auto y = messageParts[1].toInt();
            auto symbol = messageParts[2];

            board_[x][y]->setText(symbol);
            QString colorStyle = "QWidget { color: %1; }";
            board_[x][y]->setStyleSheet(colorStyle.arg(symbol == "X" ? "blue" : "red"));
            changeTurn();

            break;
        }
        case GAME_ENDED: {
            auto messageParts = message.message.split(' ');
            auto code = static_cast<GameEndedCode>(messageParts[0].toInt());

            QString text;

            switch (code) {
                case DRAW:
                    text = tr("Draw! Nobody wins(");
                    break;
                case OPPONENT_LEFT:
                    text = tr("Opponent left! You win");
                    break;
                case WIN:
                    text = tr("Finish! %1 wins").arg(messageParts[1]);
                    break;
            }

            turnLabel_->setText(text);
            gameEnded_ = true;

            break;
        }
    }
}

void GamePage::leave()
{
    if (gameEnded_) {
        emit back();
        return;
    }
    emit sendMessage({ .code = LEAVE_GAME });
}

void GamePage::handleMove(int row, int col) {
    if (!playerTurn_ || !hasOpponent_)
        return;

    QString message;
    QTextStream stream(&message);
    stream << row << ' ' << col;

    emit sendMessage({ .code = MOVE, message = message });
}

void GamePage::setOpponent(const std::optional<QString>& playerNickaname)
{
    hasOpponent_ = playerNickaname.has_value();
    auto text = playerNickaname
                ? QLabel::tr("Your opponent: %1").arg(*playerNickaname)
                : QLabel::tr("Waiting opponent...");

    opponentLabel_->setText(text);
}

QString GamePage::getTurnText() const
{
    return playerTurn_
           ? QLabel::tr("Your turn...")
           : QLabel::tr("Opponent's turn...");
}

void GamePage::changeTurn()
{
    playerTurn_ = !playerTurn_;
    turnLabel_->setText(getTurnText());
}
