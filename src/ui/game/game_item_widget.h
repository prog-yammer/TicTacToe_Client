#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class GameItemWidget : public QWidget {
    Q_OBJECT

public:
    GameItemWidget(QString&& gameInfo, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto gameInfoParts = gameInfo.split('|');

        setCursor(Qt::PointingHandCursor);

        auto* layout = new QVBoxLayout;
        this->setLayout(layout);

        layout->setSpacing(0);

        auto gameIdlabel = new QLabel(this);
        gameIdlabel->setText(QLabel::tr("Game id: %1").arg(gameInfoParts[0]));
        gameIdlabel->setStyleSheet("QLabel {"
                                   "margin: 0;"
                                   "}");
        layout->addWidget(gameIdlabel);

        auto nicknamelabel = new QLabel(this);
        nicknamelabel->setText(QLabel::tr("Player nickname: %1").arg(gameInfoParts[1]));
        layout->addWidget(nicknamelabel);
    }
};
