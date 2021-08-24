#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());
  update();

  connect(ui.actionCreate_the_connection, &QAction::triggered, this, &Chess::setServer);
  connect(ui.actionConnect_to_server, &QAction::triggered, this, &Chess::setClient);
  connect(ui.actionDisconnect, &QAction::triggered, this, &Chess::disconnect);
  connect(ui.actionStart, &QAction::triggered, this, &Chess::gameStart);
}

void Chess::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.drawPixmap(QRect(0, 21, 441, 614), QPixmap(":/Chess/Background"));
}

void Chess::setMenubar(bool connecting) {
  ui.actionCreate_the_connection->setEnabled(!connecting);
  ui.actionConnect_to_server->setEnabled(!connecting);
  ui.actionDisconnect->setEnabled(connecting);
  ui.actionStart->setEnabled(connecting);
}

void Chess::gameInit() {
  game = new Game(centralWidget());
  game->setGeometry(0, -100, 441, 614);
  game->show();
  connect(ui.actionAdmit_defeat, &QAction::triggered, game, &Game::lose);
  connect(game, &Game::clicked, this, &Chess::writeClick);
  connect(game, &Game::over, this, &Chess::gameOver);
  connect(game, &Game::enableSurrender, ui.actionAdmit_defeat, &QAction::setEnabled);
}

void Chess::setServer() {
  // init server
  tcpServer = new QTcpServer();
  connect(tcpServer, &QTcpServer::newConnection, this, [this] {
            if (tcpSocket) return; // connection exists

            qDebug() << "Connected to client";
            tcpSocket = tcpServer->nextPendingConnection();
            connect(tcpSocket, &QTcpSocket::disconnected, this, [this] {
                      qDebug() << "Disconnected from client";
                    });
            connect(tcpSocket, &QTcpSocket::readyRead, this, &Chess::read);
          });

  // start listening
  if (!tcpServer->listen(QHostAddress::LocalHost, 8080)) {
    qDebug() << "Failed to listen";
    return;
  }
  qDebug() << "Listening";
  setMenubar(true);
  gameInit();
}

void Chess::setClient() {
  // init client
  tcpSocket = new QTcpSocket();
  connect(tcpSocket, &QTcpSocket::connected, this, [this] {
            qDebug() << "Connected to server";
            ui.actionStart->setEnabled(true);
            gameInit();
          });
  connect(tcpSocket, &QTcpSocket::disconnected, this, [this] {
            qDebug() << "Disconnected from server";
          });
  connect(tcpSocket, &QTcpSocket::readyRead, this, &Chess::read);
  tcpSocket->connectToHost(QHostAddress::LocalHost, 8080);
  qDebug() << "Connecting";
  setMenubar(true);
  ui.actionStart->setEnabled(false);
}

void Chess::disconnect() {
  if (game) {
    delete game;
    game = nullptr;
  }
  if (tcpSocket) {
    tcpSocket->disconnectFromHost();
    delete tcpSocket;
    tcpSocket = nullptr;
  }
  if (tcpServer) {
    tcpServer->close();
    delete tcpServer;
    tcpServer = nullptr;
  }
  setMenubar(false);
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

void Chess::writeClick(int i) {
  QString msg = QString("Clicked %1").arg(i);
  tcpSocket->write(QString(msg).toUtf8());
  tcpSocket->flush();
}

void Chess::read() {
  const QString msg = QString::fromUtf8(tcpSocket->readAll());
  if (msg.first(8) == "Clicked ")
    game->clickOn(59 - msg.sliced(8).toInt());
}