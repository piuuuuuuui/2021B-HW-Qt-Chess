#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());
  game = new Game(centralWidget());
  game->setGeometry(0, 0, 441, 614);
  game->show();
}