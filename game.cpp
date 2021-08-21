#include <random>
#include <qstyle.h>
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

enum Game::TYPE : int {STATION, RAILWAY, CAMP};

Game::Game(QWidget *parent)
    : QWidget(parent),
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
      railwayStations({
         5,  6,  7,  8,  9,
        10,             14,
        15,             19,
        20,             24,
        25, 26, 27, 28, 29,
      /* |       |       | */
        30, 31, 32, 33, 34,
        35,             39,
        40,             44,
        45,             49,
        50, 51, 52, 53, 54
      }),
      railways({
        {5, 6, 7, 8, 9},
        {25, 26, 27, 28, 29},
        {30, 31, 32, 33, 34},
        {50, 51, 52, 53, 54},
        {5, 10, 15, 20, 25, 30, 35, 40, 45, 50},
        {9, 14, 19, 24, 29, 34, 39, 44, 49, 54},
        {27, 32}
      }),
      attackTable({
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
  gameStart();
}

void Game::gameStart() {
  // set grids
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids.push_back(QRect(j * 94, i * 44, 65, 31));
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++)
      grids.push_back(QRect(j * 94, i * 44 + 360, 65, 31));

  // set grid status and type
  gridStatus.assign(60, UNKNOWN);
  gridType.assign(60, STATION);
  for (int i : railwayStations) {
    gridType[i] = RAILWAY;
  }
  for (int i : {11, 13, 17, 21, 23, 36, 38, 42, 46, 48}) {
    gridStatus[i] = EMPTY;
    gridType[i] = CAMP;
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

void Game::gameOver() {
}

void Game::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::MouseButton::LeftButton)
    for (int i = 0; i < grids.size(); i++)
      if (grids[i].contains(event->pos())) {
        if (focus == -1)
          focusOn(i);
        else
          moveFromTo(focus, i);
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

  // check moveability
  STATUS fs = gridStatus[f], ts = gridStatus[t];
  if (!isReachable(f, t) ||
      !isAttackable(fs, ts))
    return;

  // execute
  if ((fs ^ ts) == 1 ||
      ts != EMPTY && (
        fs == RB ||
        fs == BB)) {
    setStatus(t, EMPTY);
  } else {
    setStatus(t, fs);
  }
  setStatus(f, EMPTY);

  // post-check
  if (ts == RM) numOfRM--;
  if (ts == BM) numOfBM--;
  if (ts == RF || ts == BF) gameOver();
}

bool Game::isAdjacent(int a, int b) {
  int dist = (grids[a].topLeft() - grids[b].topLeft()).manhattanLength();
  int left = grids[a].left();
  if (dist == 44 || dist == 94) return true;
  if (dist == 138 && (gridType[a] == CAMP || gridType[b] == CAMP)) return true;
  if (dist == 140 && (left == 0 || left == 188 || left == 376)) return true;
  return false;
}

bool Game::isReachable(int a, int b) {
  // coincide
  if (a == b) return false;

  // protected
  if (gridType[b] == CAMP && gridStatus[b] != EMPTY) return false;

  // adjacent
  if (isAdjacent(a, b)) return true;

  // off railway
  if (gridType[a] != RAILWAY || gridType[b] != RAILWAY) return false;
  
  // Floyd-Warshall
  std::vector<bool> reachable(60, false);
  for (int i = 0; i < 32; i++) {
    for (int j : railwayStations)
      if (j == a || reachable[j] && gridStatus[j] == EMPTY)
        for (int k : railwayStations)
          if (isAdjacent(j, k)) reachable[k] = true;
  }
  if (!reachable[b]) return false;

  // GongBing
  if (gridStatus[a] == R1 || gridStatus[a] == B1) return true;

  // others (move straight only)
  for (auto railway : railways)
    if (railway.count(a) && railway.count(b)) return true;
  return false;
}

bool Game::isAttackable(STATUS a, STATUS b) {
  if (b == RF)
    if (numOfRM) return false;
  if (b == BF)
    if (numOfBM) return false;
  return attackTable[a][b];
}