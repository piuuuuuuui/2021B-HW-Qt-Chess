#include <qstyle.h>
#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  setFixedSize(width(), height());
  setBoard();
  setPieces();
}

void Chess::setBoard() {
  board_ = new QWidget(centralWidget());
  board_->setGeometry(0, 0, 441, 614);
  board_->setStyleSheet("border-image:url(:/Chess/Grids)");
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids_.push_back(QRect(j * 94, i * 44, 65, 31));
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids_.push_back(QRect(j * 94, i * 44 + 360, 65, 31));
  board_->show();
}

void Chess::setPieces() {
  for (int i = 0; i < 60; i++) {
    pieces_.push_back(new QWidget(centralWidget()));
    if (true) {
      pieces_[i]->setGeometry(grids_[i]);
      pieces_[i]->setStyleSheet("border-image:url(:/Chess/UN)");
      pieces_[i]->show();
    }
  }
}