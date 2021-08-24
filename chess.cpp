#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());

  connect(ui.actionCreate_the_connection, &QAction::triggered, this, &Chess::setServer);
  connect(ui.actionConnect_to_server, &QAction::triggered, this, &Chess::setClient);
  connect(ui.actionDisconnect, &QAction::triggered, this, &Chess::disconnect);
  connect(ui.actionStart, &QAction::triggered, this, &Chess::gameStart);
}

void Chess::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.drawPixmap(QRect(0, 21, 441, 614), QPixmap(":/Chess/Background"));
}

void Chess::connecting(bool x) {
  ui.actionCreate_the_connection->setEnabled(!x);
  ui.actionConnect_to_server->setEnabled(!x);
  ui.actionDisconnect->setEnabled(x);
  ui.actionStart->setEnabled(x);
}

void Chess::gameInit() {
  game = new Game(centralWidget());
  game->setGeometry(0, -100, 441, 614);
  game->show();
  connect(ui.actionAdmit_defeat, &QAction::triggered, game, &Game::lose);
  connect(game, &Game::over, this, &Chess::gameOver);
  connect(game, &Game::enableSurrender, ui.actionAdmit_defeat, &QAction::setEnabled);
}

void Chess::setServer() {
  tcpServer = new QTcpServer();
  connect(tcpServer, &QTcpServer::newConnection, this, &Chess::onConnectedToClient);
  if (!tcpServer->listen(QHostAddress::LocalHost, 8080)) {
    qDebug() << "Failed to listen";
    return;
  }
  qDebug() << "Listening";
  gameInit();
  connecting(true);
}

void Chess::setClient() {
  tcpSocketClient = new QTcpSocket();
  connect(tcpSocketClient, &QTcpSocket::connected, this, &Chess::onConnectedToServer);
  connect(tcpSocketClient, &QTcpSocket::disconnected, this, &Chess::onDisconnectedFromServer);
  tcpSocketClient->connectToHost(QHostAddress::LocalHost, 8080);
  qDebug() << "Connecting";
  gameInit();
  connecting(true);
  ui.actionStart->setEnabled(false);
}

void Chess::disconnect() {
  if (game) {
    delete game;
    game = nullptr;
  }
  if (tcpServer) {
    if (tcpSocketServer) {
      tcpSocketServer->disconnectFromHost();
      delete tcpSocketServer;
      tcpSocketServer = nullptr;
    }
    tcpServer->close();
    delete tcpServer;
    tcpServer = nullptr;
  } else if (tcpSocketClient) {
    tcpSocketClient->disconnectFromHost();
    delete tcpSocketClient;
    tcpSocketClient = nullptr;
  }
  connecting(false);
}

void Chess::gameStart() {
  ready = true;
  ui.actionStart->setEnabled(false);
  ui.actionAdmit_defeat->setEnabled(false);
  qDebug() << "Game start";
  game->start();
}

void Chess::gameOver() {
  ready = false;
  ui.actionStart->setEnabled(true);
  ui.actionAdmit_defeat->setEnabled(false);
  qDebug() << "Game over";
}

void Chess::onConnectedToClient() {
  qDebug() << "Connected to client";
  tcpSocketServer = tcpServer->nextPendingConnection();
  connect(tcpSocketServer, &QTcpSocket::disconnected, this, &Chess::onDisconnectedFromClient);
}

void Chess::onConnectedToServer() {
  ui.actionStart->setEnabled(true);
  qDebug() << "Connected to server";
}

void Chess::onDisconnectedFromClient() {
  qDebug() << "Disconnected from client";
}

void Chess::onDisconnectedFromServer() {
  qDebug() << "Disconnected from server";
}