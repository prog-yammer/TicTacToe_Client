#pragma once

#include "message/message.h"

#include <QWidget>

class BasePage : public QWidget
{
    Q_OBJECT
public:
    BasePage(QWidget* parent = nullptr)
        : QWidget(parent)
    {}

    virtual void onStart() {};

Q_SIGNALS:
    void forward(BasePage* widget, bool removeThis = false);
    void back();

    void sendMessage(const OutMessage& message);

public Q_SLOTS:
    virtual void receiveMessage(const InMessage& message) {};
};
