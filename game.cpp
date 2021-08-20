#include <qstyle.h>
#include <random>
#include "game.h"

enum Game::STATUS : int {
  EMPTY,
  UNKNOWN,
  R1, B1, // GongBing
  R2, B2, // PaiZhang
  R3, B3, // LianZhang
  R4, B4, // YingZhang
  R5, B5, // TuanZhang
  R6, B6, // LvZhang
  R7, B7, // ShiZhang
  R8, B8, // JunZhang
  R9, B9, // SiLing
  RF, BF, // Flag
  RB, BB, // Bomb
  RM, BM, // Mine
};

enum Game::TYPE : int { BINGZHAN, XINGYING };

Game::Game(QWidget *parent)
    : QWidget(parent),
      board(Q_NULLPTR),
      cache(-1),
      pics({
        QPixmap(),
        QPixmap(":/Chess/UN"),
        QPixmap(":/Chess/R1"), QPixmap(":/Chess/B1"),
        QPixmap(":/Chess/R2"), QPixmap(":/Chess/B2"),
        QPixmap(":/Chess/R3"), QPixmap(":/Chess/B3"),
        QPixmap(":/Chess/R4"), QPixmap(":/Chess/B4"),
        QPixmap(":/Chess/R5"), QPixmap(":/Chess/B5"),
        QPixmap(":/Chess/R6"), QPixmap(":/Chess/B6"),
        QPixmap(":/Chess/R7"), QPixmap(":/Chess/B7"),
        QPixmap(":/Chess/R8"), QPixmap(":/Chess/B8"),
        QPixmap(":/Chess/R9"), QPixmap(":/Chess/B9"),
        QPixmap(":/Chess/RF"), QPixmap(":/Chess/BF"),
        QPixmap(":/Chess/RB"), QPixmap(":/Chess/BB"),
        QPixmap(":/Chess/RM"), QPixmap(":/Chess/BM")
      }) {
  setStyleSheet("border-image:url(:/Chess/Background)");
  start();
}

void Game::start() {
  // set grids
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids.push_back(QRect(j * 94, i * 44, 65, 31));
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids.push_back(QRect(j * 94, i * 44 + 360, 65, 31));

  // set grid status and type
  gridStatus.assign(60, UNKNOWN);
  gridType.assign(60, BINGZHAN);
  for (int i : {11, 13, 17, 21, 23, 36, 38, 42, 46, 48}) {
    gridStatus[i] = EMPTY;
    gridType[i] = XINGYING;
  }

  // set chess board
  board = new QWidget(this);
  board->setGeometry(0, 0, 441, 614);
  board->setStyleSheet("border-image:url(:/Chess/Grids)");
  board->show();

  // set chess pieces
  pieces.assign(60, Q_NULLPTR);
  for (int i = 0; i < 60; i++) {
    pieces[i] = new QLabel(this);
    pieces[i]->setGeometry(grids[i]);
    pieces[i]->setScaledContents(true);
    setStatus(i, gridStatus[i]);
  }

  // initialize pieces randomly
  for (STATUS s : {R1, B1, R2, B2, R3, B3, RM, BM})
    initStatus.insert(initStatus.end(), 3, s);
  for (STATUS s : {R4, B4, R5, B5, R6, B6, R7, B7, RB, BB})
    initStatus.insert(initStatus.end(), 2, s);
  for (STATUS s : {R8, B8, R9, B9, RF, BF})
    initStatus.insert(initStatus.end(), 1, s);
  std::shuffle(
    initStatus.begin(),
    initStatus.end(),
    std::default_random_engine((unsigned)(new char))
  );
  for (int i : {11, 13, 17, 21, 23, 36, 38, 42, 46, 48})
    initStatus.insert(initStatus.begin() + i, EMPTY);
}

void Game::mousePressEvent(QMouseEvent *event) {
  for (int i = 0; i < grids.size(); i++) {
    if (grids[i].contains(event->pos())) {
      // debug
      qDebug() << "Click on grid" << i;
      if (cache == -1) {
        if (gridStatus[i] == EMPTY)
          return;
        if (gridStatus[i] == UNKNOWN)
          setStatus(i, initStatus[i]);
        else
          cache = i;
      } else {
        setStatus(i, gridStatus[cache]);
        setStatus(cache, EMPTY);
        cache = -1;
      }
      return;
    }
  }
}

void Game::setStatus(int i, STATUS s) {
  gridStatus[i] = s;
  if (s == EMPTY) {
    pieces[i]->hide();
  } else {
    pieces[i]->setPixmap(pics[s]);
    pieces[i]->show();
  }
}