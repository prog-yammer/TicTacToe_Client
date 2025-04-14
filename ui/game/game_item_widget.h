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
        auto* layout = new QHBoxLayout;
        this->setLayout(layout);

        auto label = new QLabel(this);
        label->setText(gameInfo.replace('|', ' '));
        layout->addWidget(label);
    }
};
