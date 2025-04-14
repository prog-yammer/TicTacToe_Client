#pragma once

#include "../common/base_page.h"
#include "../common/websocket_worker.h"

#include <QWidget>
#include <QStackedWidget>
#include <QList>
#include <QThread>
#include <QWebSocket>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void messageReceive(const InMessage& message);
    void sendTextMessage(const QString& message);

private Q_SLOTS:
    void forward(BasePage* widget, bool removeLast = false);
    void back();

    void connectionStatusUpdated(bool connected);
    void onSendMessage(const OutMessage& message);
    void onTextMessageReceived(const QString& message);

private:
    void connectBasePage(BasePage* widget);
    void disconnectBasePage(BasePage* widget);

    QStackedWidget* container_;
    QList<BasePage*> stack_;

    QThread* netThread_;
    WebSocketWorker* webSocket_;
};
