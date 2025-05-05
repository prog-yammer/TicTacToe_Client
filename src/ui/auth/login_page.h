#pragma once

#include "../../common/base_page.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>

class LoginPage : public BasePage
{
    Q_OBJECT

public:
    LoginPage(QWidget* parent = nullptr);

public Q_SLOTS:
    void receiveMessage(const InMessage& message) override;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private Q_SLOTS:
    void authorize();

private:
    QLineEdit* nicknameLineEdit_;
    QPushButton* authButton_;
};
