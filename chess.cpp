#include <qstyle.h>
#include "chess.h"
#include "chessboard.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());
  Chessboard *board = new Chessboard(centralWidget());
  board->show();
}
