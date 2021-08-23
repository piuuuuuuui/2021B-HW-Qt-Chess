#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());

  game = new Game(centralWidget());
  game->setGeometry(0, -100, 441, 614);

  connect(ui.actionStart, SIGNAL(triggered(bool)), this, SLOT(gameStart()));
  connect(ui.actionAdmit_defeat, SIGNAL(triggered(bool)), game, SLOT(lose()));
  connect(game, SIGNAL(over()), this, SLOT(gameOver()));
  connect(game, SIGNAL(enableSurrender(bool)), ui.actionAdmit_defeat, SLOT(setEnabled(bool)));
}

void Chess::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.drawPixmap(QRect(0, 21, 441, 614), QPixmap(":/Chess/Background"));
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