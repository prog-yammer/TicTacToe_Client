#pragma once

#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebSocket>
#include <QUrl>
#include <QThread>

class WebSocketWorker : public QObject
{
Q_OBJECT

public:
    explicit WebSocketWorker(const QUrl& url, QObject* parent = nullptr)
        : QObject(parent), url_(url), webSocket_(new QWebSocket)
    {
        webSocket_->setParent(this);

        connect(webSocket_, &QWebSocket::connected, this, &WebSocketWorker::onConnected);
        connect(webSocket_, &QWebSocket::disconnected, this, &WebSocketWorker::onDisconnected);
        connect(webSocket_, &QWebSocket::textMessageReceived, this, &WebSocketWorker::onTextMessageReceived);
    }

    ~WebSocketWorker()
    {
        webSocket_->close();
    }

public Q_SLOTS:
    void start() 
    {
        webSocket_->open(url_);
    }

    void sendMessage(const QString &message) 
    {
        webSocket_->sendTextMessage(message);
    }

Q_SIGNALS:
    void messageReceived(const QString& message);
    void statusUpdate(bool connected);

private Q_SLOTS:
    void onConnected()
    {
        emit statusUpdate(true);
    }

    void onDisconnected()
    {
        emit statusUpdate(false);
    }

    void onTextMessageReceived(const QString& message)
    {
        emit messageReceived(message);
    }

private:
    QUrl url_;
    QWebSocket* webSocket_;
};
