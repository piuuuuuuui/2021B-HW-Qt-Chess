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
  server = new QTcpServer();
  qDebug() << server->listen();
  connecting(true);
  gameInit();
}

void Chess::setClient() {
  connecting(true);
}

void Chess::disconnect() {
  delete game;
  connecting(false);
}

void Chess::gameStart() {
  ui.actionStart->setEnabled(false);
  ui.actionAdmit_defeat->setEnabled(false);
  game->start();
}

void Chess::gameOver() {
  ui.actionStart->setEnabled(true);
  ui.actionAdmit_defeat->setEnabled(false);
  qDebug() << "Game Over";
}