#include <qevent.h>
#include "chessboard.h"

Chessboard::Chessboard(QWidget *parent) :
    QWidget(parent) {
  setGeometry(0, 0, 441, 614);
  setStyleSheet("border-image:url(:/Chess/BG)");
  setGrids();
}

void Chessboard::mousePressEvent(QMouseEvent *event) {
  // for (const Grid &grid : grids_)
  for (int i = 0; i < grids_.size(); i++)
    // if (grid.contains(event->pos())) qDebug() << event->x() << event->y();
    if (grids_[i].contains(event->pos())) qDebug() << i;
}

void Chessboard::setGrids() {
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids_.push_back(QRect(j * 88 + 6, i * 43 + 8, 77, 37));
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids_.push_back(QRect(j * 88 + 6, i * 43 + 356, 77, 37));
}