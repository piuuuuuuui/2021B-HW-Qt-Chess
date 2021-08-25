#include <random>
#include <QNetworkInterface>
#include "chess.h"
#include "dialog.h"

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
  game->setGeometry(0, -100, 441, 714);
  game->show();
  connect(game, &Game::clicked, this, [&](int i) {
            QString msg = QString("Click %1").arg(59 - i, 2, 10, QChar('0'));
            tcpSocket->write(QString(msg).toUtf8());
            tcpSocket->flush();
          });
  connect(game, &Game::switched, this, [&]() {
            QString msg = QString("Your turn");
            tcpSocket->write(QString(msg).toUtf8());
            tcpSocket->flush();
          });
  connect(game, &Game::over, this, &Chess::gameOver);
  connect(game, &Game::enableResign, ui.actionAdmit_defeat, &QAction::setEnabled);
  connect(ui.actionAdmit_defeat, &QAction::triggered, game, &Game::lose);
}

void Chess::setServer() {
  QDialog *hostIP = new QDialog(this);
  hostIP->setModal(true);
  hostIP->setWindowTitle("Host IP");
  hostIP->setFixedSize(161, 60);
  QLabel *info1 = new QLabel("Server created", hostIP);
  info1->setGeometry(10, 10, 141, 20);
  QHostAddress localhost = QHostAddress::LocalHost;
  for (auto ip : QNetworkInterface::allAddresses())
    if (ip != localhost && ip.toIPv4Address()) {
      localhost = ip;
      break;
    }
  QLabel *info2 = new QLabel("Your IP: " + localhost.toString(), hostIP);
  info2->setGeometry(10, 30, 141, 20);
  hostIP->exec();

  // init server
  tcpServer = new QTcpServer();
  connect(tcpServer, &QTcpServer::newConnection, this, [&]() {
            if (tcpSocket) { // connection exists
              tcpServer->nextPendingConnection()->close();
              return;
            }
            setWindowTitle(QString("%1 [%2]")
                           .arg("Chess")
                           .arg("Connected to client"));
            tcpSocket = tcpServer->nextPendingConnection();
            connect(tcpSocket, &QTcpSocket::disconnected, this, [&]() {
                      setWindowTitle(QString("%1 [%2]")
                                     .arg("Chess")
                                     .arg("Disconnected from client"));
                      tcpSocket->deleteLater();
                      tcpSocket = nullptr;
                      if (game) {
                        game->deleteLater();
                        game = nullptr;
                      }
                      ui.actionStart->setEnabled(false);
                      ui.actionAdmit_defeat->setEnabled(false);
                    });
            connect(tcpSocket, &QTcpSocket::readyRead, this, &Chess::read);
            gameInit();
            ui.actionStart->setEnabled(true);
            ui.actionAdmit_defeat->setEnabled(false);
          });

  // start listening
  if (!tcpServer->listen(QHostAddress::Any, 514)) {
    setWindowTitle(QString("%1 [%2]")
                   .arg("Chess")
                   .arg("Failed to listen"));
    return;
  }
  setWindowTitle(QString("%1 [%2]")
                 .arg("Chess")
                 .arg("Listening"));
  ui.actionCreate_the_connection->setEnabled(false);
  ui.actionConnect_to_server->setEnabled(false);
  ui.actionDisconnect->setEnabled(true);
}

void Chess::setClient() {
  // input host address
  QHostAddress host = QHostAddress::Null;
  Dialog *inputHostAddress = new Dialog(this);
  connect(inputHostAddress, &Dialog::getIP, this, [&](QString ip) {
            inputHostAddress->deleteLater();
            if (ip.isEmpty() || !host.setAddress(ip)) {
              QDialog *warning = new QDialog(this);
              warning->setModal(true);
              warning->setWindowTitle("Warning");
              warning->setFixedSize(161, 40);
              QLabel *info = new QLabel("Invalid IP", warning);
              info->setGeometry(10, 10, 141, 20);
              warning->exec();
            }
          });
  inputHostAddress->exec();

  if (host == QHostAddress::Null) return;

  // init client
  tcpSocket = new QTcpSocket();
  connect(tcpSocket, &QTcpSocket::connected, this, [&]() {
            setWindowTitle(QString("%1 [%2]")
                           .arg("Chess")
                           .arg("Connected to server"));
            gameInit();
            ui.actionStart->setEnabled(true);
            ui.actionAdmit_defeat->setEnabled(false);
          });
  connect(tcpSocket, &QTcpSocket::disconnected, this, [&]() {
            setWindowTitle(QString("%1 [%2]")
                           .arg("Chess")
                           .arg("Disconnected from server"));
            tcpSocket->deleteLater();
            tcpSocket = nullptr;
            if (game) {
              game->deleteLater();
              game = nullptr;
            }
            ui.actionStart->setEnabled(false);
            ui.actionAdmit_defeat->setEnabled(false);
          });
  connect(tcpSocket, &QTcpSocket::readyRead, this, &Chess::read);

  // start connecting
  tcpSocket->connectToHost(host, 514);
  setWindowTitle(QString("%1 [%2]")
                 .arg("Chess")
                 .arg("Connecting"));
  ui.actionCreate_the_connection->setEnabled(false);
  ui.actionConnect_to_server->setEnabled(false);
  ui.actionDisconnect->setEnabled(true);
}

void Chess::disconnect() {
  if (tcpSocket) {
    tcpSocket->disconnectFromHost();
    tcpSocket->deleteLater();
    tcpSocket = nullptr;
  }
  if (tcpServer) {
    tcpServer->close();
    tcpServer->deleteLater();
    tcpServer = nullptr;
  }
  ui.actionCreate_the_connection->setEnabled(true);
  ui.actionConnect_to_server->setEnabled(true);
  ui.actionDisconnect->setEnabled(false);
}

void Chess::gameStart() {
  setWindowTitle("Chess");
  ui.actionStart->setEnabled(false);
  ui.actionAdmit_defeat->setEnabled(false);
  unsigned seed = std::random_device{}();
  bool first = std::random_device{}() & 1;
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
  tcpSocket->write(QString("You win").toUtf8());
  tcpSocket->flush();
}

void Chess::read() {
  const QString msg = QString::fromUtf8(tcpSocket->readAll());
  if (msg.first(5) == "Click") {
    game->clickOn(msg.last(2).toInt());
    return;
  }
  if (msg == "Your turn") {
    game->updateRound(false);
    return;
  }
  if (msg.first(5) == "Start") {
    ui.actionStart->setEnabled(false);
    ui.actionAdmit_defeat->setEnabled(false);
    game->start(msg.sliced(12).toUInt(), msg.sliced(6, 5) == "first");
    return;
  }
  if (msg == "You win") {
    ui.actionStart->setEnabled(true);
    ui.actionAdmit_defeat->setEnabled(false);
    game->win();
    return;
  }
}