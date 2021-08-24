#include <random>
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

void Chess::gameInit() {
  game = new Game(centralWidget());
  game->setGeometry(0, -100, 441, 614);
  game->show();
  connect(game, &Game::clicked, this, &Chess::writeClick);
  connect(game, &Game::over, this, &Chess::gameOver);
  connect(game, &Game::enableResign, ui.actionAdmit_defeat, &QAction::setEnabled);
  connect(ui.actionAdmit_defeat, &QAction::triggered, game, &Game::lose);
}

void Chess::setServer() {
  // init server
  tcpServer = new QTcpServer();
  connect(tcpServer, &QTcpServer::newConnection, this, [this] {
            if (tcpSocket) { // connection exists
              tcpServer->nextPendingConnection()->abort();
              return;
            }

            qDebug() << "Connected to client";
            tcpSocket = tcpServer->nextPendingConnection();
            connect(tcpSocket, &QTcpSocket::disconnected, this, [this] {
                      qDebug() << "Disconnected from client";
                      if (game) {
                        delete game;
                        game = nullptr;
                      }
                      ui.actionStart->setEnabled(false);
                    });
            connect(tcpSocket, &QTcpSocket::readyRead, this, &Chess::read);
            gameInit();
            ui.actionStart->setEnabled(true);
          });

  // start listening
  if (!tcpServer->listen(QHostAddress::LocalHost, 8080)) {
    qDebug() << "Failed to listen";
    return;
  }
  qDebug() << "Listening";
  ui.actionCreate_the_connection->setEnabled(false);
  ui.actionConnect_to_server->setEnabled(false);
  ui.actionDisconnect->setEnabled(true);
}

void Chess::setClient() {
  // init client
  tcpSocket = new QTcpSocket();
  connect(tcpSocket, &QTcpSocket::connected, this, [this] {
            qDebug() << "Connected to server";
            gameInit();
            ui.actionStart->setEnabled(true);
          });
  connect(tcpSocket, &QTcpSocket::disconnected, this, [this] {
            qDebug() << "Disconnected from server";
            if (game) {
              delete game;
              game = nullptr;
            }
            ui.actionStart->setEnabled(false);
          });
  connect(tcpSocket, &QTcpSocket::readyRead, this, &Chess::read);

  // start connecting
  tcpSocket->connectToHost(QHostAddress::LocalHost, 8080);
  qDebug() << "Connecting";
  ui.actionCreate_the_connection->setEnabled(false);
  ui.actionConnect_to_server->setEnabled(false);
  ui.actionDisconnect->setEnabled(true);
}

void Chess::disconnect() {
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
  ui.actionCreate_the_connection->setEnabled(true);
  ui.actionConnect_to_server->setEnabled(true);
  ui.actionDisconnect->setEnabled(false);
}

void Chess::gameStart() {
  qDebug() << "Game start";
  ui.actionStart->setEnabled(false);
  ui.actionAdmit_defeat->setEnabled(false);
  srand((unsigned)(new char));
  unsigned seed = rand();
  bool first = rand() & 1;
  QString msg = QString("Start %1 %2")
                .arg(first ? "next " : "first")
                .arg(seed);
  tcpSocket->write(QString(msg).toUtf8());
  tcpSocket->flush();
  game->start(seed, first);
}

void Chess::gameOver() {
  ui.actionStart->setEnabled(true);
  ui.actionAdmit_defeat->setEnabled(false);
  qDebug() << "Game over";
}

void Chess::writeClick(int i) {
  QString msg = QString("Click %1").arg(i);
  tcpSocket->write(QString(msg).toUtf8());
  tcpSocket->flush();
}

void Chess::read() {
  const QString msg = QString::fromUtf8(tcpSocket->readAll());
  if (msg.first(6) == "Click ") {
    game->clickOn(59 - msg.sliced(6).toInt());
    return;
  }
  if (msg.first(6) == "Start ") {
    qDebug() << "Game start";
    ui.actionStart->setEnabled(false);
    ui.actionAdmit_defeat->setEnabled(false);
    game->start(msg.sliced(12).toUInt(), msg.sliced(6, 5) == "first");
  }
}