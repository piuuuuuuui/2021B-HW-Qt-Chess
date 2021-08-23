#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());

  tcpServer = new QTcpServer();
  tcpSocketServer = nullptr;
  tcpSocketClient = new QTcpSocket();
  connect(tcpServer, &QTcpServer::newConnection, this, &Chess::onNewConnection);
  connect(tcpSocketClient, &QTcpSocket::connected, this, &Chess::onConnectedToServer);
  connect(tcpSocketClient, &QTcpSocket::disconnected, this, &Chess::onDisconnectedFromhost);

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
  if (!tcpServer->listen(QHostAddress::LocalHost, 8080)) {
    qDebug() << "Failed to listen";
    return;
  }
  qDebug() << "Listening";
  connecting(true);
  gameInit();
}

void Chess::setClient() {
  tcpSocketClient->connectToHost(QHostAddress::LocalHost, 8080);
  qDebug() << "Connecting";
  connecting(true);
}

void Chess::disconnect() {
  if (game) {
    delete game;
    game = nullptr;
  }
  tcpServer->close();
  tcpSocketClient->disconnectFromHost();
  connecting(false);
}

void Chess::gameStart() {
  ui.actionStart->setEnabled(false);
  ui.actionAdmit_defeat->setEnabled(false);
  qDebug() << "Game start";
  game->start();
}

void Chess::gameOver() {
  ui.actionStart->setEnabled(true);
  ui.actionAdmit_defeat->setEnabled(false);
  qDebug() << "Game over";
}

void Chess::onNewConnection() {
  tcpSocketServer = tcpServer->nextPendingConnection();
}

void Chess::onConnectedToServer() {
  qDebug() << "Connected to server";
}

void Chess::onDisconnectedFromhost() {
  qDebug() << "Disconnected from host";
}