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
      focus(-1),
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
      }),
      attackable({
        /*     EM UN R1 B1 R2 B2 R3 B3 R4 B4 R5 B5 R6 B6 R7 B7 R8 B8 R9 B9 RF BF RB BB RM BM */
        /*EM*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /*UN*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /*R1*/ {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        /*B1*/ {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        /*R2*/ {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B2*/ {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R3*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B3*/ {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R4*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B4*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R5*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B5*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R6*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B6*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R7*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B7*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R8*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
        /*B8*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        /*R9*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
        /*B9*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
        /*RF*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /*BF*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /*RB*/ {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        /*BB*/ {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        /*RM*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /*BM*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
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
  if (event->button() == Qt::MouseButton::LeftButton)
    for (int i = 0; i < grids.size(); i++)
      if (grids[i].contains(event->pos())) {
        qDebug() << "Click on grid" << i; // debug
        if (focus == -1) focusOn(i);
        else moveFromTo(focus, i);
        return;
      }
  focusOff();
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

void Game::focusOn(int f) {
  if (gridStatus[f] == EMPTY) return;
  if (gridStatus[f] == UNKNOWN) {
    setStatus(f, initStatus[f]);
    return;
  }
  focus = f;
  pieces[focus]->setEnabled(false);
}

void Game::focusOff() {
  if (focus == -1) return;
  pieces[focus]->setEnabled(true);
  focus = -1;
}

void Game::moveFromTo(int f, int t) {
  focusOff();
  STATUS fs = gridStatus[f], ts = gridStatus[t];
  if (f == t || !isReachable(f, t) || !isAttackable(fs, ts)) return;
  if ((fs ^ ts) == 1 || fs == RB || fs == BB)
    setStatus(t, EMPTY);
  else
    setStatus(t, gridStatus[f]);
  setStatus(f, EMPTY);
}

bool Game::isReachable(int a, int b) {
  return true;
}

bool Game::isAttackable(STATUS a, STATUS b) {
  if (b == RF) {
    int numOfBM = 0;
    for (STATUS s : gridStatus)
      if (s == BM) numOfBM++;
    if (numOfBM) return false;
  }
  if (b == BF) {
    int numOfRM = 0;
    for (STATUS s : gridStatus)
      if (s == RM) numOfRM++;
    if (numOfRM) return false;
  }
  return attackable[a][b];
}