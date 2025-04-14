#include "main_window.h"

#include "../common/message/tools.h"

#include "auth/login_page.h"

#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , container_(new QStackedWidget)
    , netThread_(new QThread(this))
    , webSocket_(new WebSocketWorker(QUrl("ws://localhost:8080")))
{
    setFixedSize(800, 600);

    auto* layout = new QVBoxLayout;
    this->setLayout(layout);

    layout->addWidget(container_);

    webSocket_->moveToThread(netThread_);
    connect(this, &MainWindow::sendTextMessage, webSocket_, &WebSocketWorker::sendMessage);
    connect(webSocket_, &WebSocketWorker::messageReceived, this, &MainWindow::onTextMessageReceived);
    connect(webSocket_, &WebSocketWorker::statusUpdate, this, &MainWindow::connectionStatusUpdated);

    connect(netThread_, &QThread::started, webSocket_, &WebSocketWorker::start);
    connect(netThread_, &QThread::finished, webSocket_, &QObject::deleteLater);

    netThread_->start();
}

MainWindow::~MainWindow()
{
    netThread_->quit();
    netThread_->wait();
}

void MainWindow::forward(BasePage* widget, bool removeLast)
{
    if (removeLast && !stack_.empty()) {
        auto* last = stack_.last();
        container_->removeWidget(last);
        stack_.removeLast();
        delete last;
    } else {
        disconnectBasePage(stack_.last());
    }

    stack_.push_back(widget);
    connectBasePage(stack_.last());
    container_->addWidget(stack_.last());
    container_->setCurrentWidget(stack_.last());
}

void MainWindow::back()
{
    if (stack_.size() < 2) return;

    auto* last = stack_.last();
    container_->removeWidget(last);
    stack_.removeLast();
    disconnectBasePage(last);
    delete last;

    connectBasePage(stack_.last());
    container_->setCurrentWidget(stack_.last());
}

void MainWindow::connectionStatusUpdated(bool connected)
{
    if (connected) {
        auto* loginPage = new LoginPage(this);
        connectBasePage(loginPage);
        container_->addWidget(loginPage);
        container_->setCurrentIndex(0);
        stack_.push_back(loginPage);
        return;
    }

    QMessageBox::information(
            this,
            QMessageBox::tr("Notification"),
            tr("Connection lost!"));
    close();
}

void MainWindow::onSendMessage(const OutMessage& message)
{
    emit sendTextMessage(convertOutMessage(message));
}

void MainWindow::onTextMessageReceived(const QString& message)
{
    emit messageReceive(getInMessage(message));
}

void MainWindow::connectBasePage(BasePage* widget) {
    connect(widget, &BasePage::forward, this, &MainWindow::forward);
    connect(widget, &BasePage::back, this, &MainWindow::back);

    connect(widget, &BasePage::sendMessage, this, &MainWindow::onSendMessage);
    connect(this, &MainWindow::messageReceive, widget, &BasePage::receiveMessage);

    widget->onStart();
}

void MainWindow::disconnectBasePage(BasePage* widget) {
    disconnect(widget, &BasePage::forward, this, &MainWindow::forward);
    disconnect(widget, &BasePage::back, this, &MainWindow::back);

    disconnect(widget, &BasePage::sendMessage, this, &MainWindow::onSendMessage);
    disconnect(this, &MainWindow::messageReceive, widget, &BasePage::receiveMessage);
}
