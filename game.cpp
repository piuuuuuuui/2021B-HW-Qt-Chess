#include <qstyle.h>
#include "game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    board_(Q_NULLPTR) {
  setStyleSheet("border-image:url(:/Chess/BG)");
  start();
}

void Game::start() {
  // set chess board
  board_ = new QWidget(this);
  board_->setGeometry(0, 0, 441, 614);
  board_->setStyleSheet("border-image:url(:/Chess/Grids)");
  board_->show();

  // set grids
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids_.push_back(QRect(j * 94, i * 44, 65, 31));
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids_.push_back(QRect(j * 94, i * 44 + 360, 65, 31));
  gridStatus_.fill(STATUS::UNKNOWN, 60);
  gridType_.fill(TYPE::BINGZHAN, 60);
  for (int i : {11, 13, 17, 21, 23, 36, 38, 42, 46, 48}) {
    gridStatus_[i] = STATUS::EMPTY;
    gridType_[i] = TYPE::XINGYING;
  }

  // set chess pieces
  pieces_.fill(Q_NULLPTR, 60);
  for (int i = 0; i < 60; i++) {
    if (gridType_[i] == TYPE::BINGZHAN) {
      pieces_[i] = new QWidget(this);
      pieces_[i]->setGeometry(grids_[i]);
      pieces_[i]->setStyleSheet("border-image:url(:/Chess/UN)");
      pieces_[i]->show();
    }
  }
}

void Game::mousePressEvent(QMouseEvent *event) {
  for (int i = 0; i < grids_.size(); i++) {
    if (grids_[i].contains(event->pos())) {
      // debug
      qDebug() << "Click on grid" << i;

      return;
    }
  }
}