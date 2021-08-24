#include <random>
#include <QStyle>
#include "game.h"

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
      camps({11, 13, 17, 21, 23, 36, 38, 42, 46, 48}),
      railways({
        {5, 6, 7, 8, 9},
        {25, 26, 27, 28, 29},
        {30, 31, 32, 33, 34},
        {50, 51, 52, 53, 54},
        {5, 10, 15, 20, 25, 30, 35, 40, 45, 50},
        {9, 14, 19, 24, 29, 34, 39, 44, 49, 54},
        {27, 32}
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
  rot.translate(220, 600);
  rot.rotate(30, Qt::XAxis);
  rot.translate(-220, -600);
  setGrids();
  setAdjacent();
  for (int i = 0; i < 60; i++){
    pieces.push_back(new QLabel(this));
    pieces[i]->setGeometry(grids[i]);
    pieces[i]->setScaledContents(true);
  }
}

Game::~Game() {
  for (auto p : pieces) delete p;
}

void Game::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.setTransform(rot);
  p.drawPixmap(QRect(0, 0, 441, 614), QPixmap(":/Chess/Grids"));
}

void Game::mousePressEvent(QMouseEvent *event) {
  if (!available) return;
  if (event->button() == Qt::MouseButton::LeftButton)
    for (int i = grids.size(); i--; )
      if (grids[i].contains(event->pos())) {
        if (focus == -1)
          focusOn(i);
        else
          moveFromTo(focus, i);
        return;
      }
  focusOff();
}

void Game::setGrids() {
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++) {
      QLine bottom = rot.map(QLine(j * 94 + 5, i * 44 + 27, j * 94 + 32, i * 44 + 27));
      grids.push_back(QRect(
        bottom.x1(),
        bottom.y1() - bottom.dx(),
        bottom.dx() * 2,
        bottom.dx()
      ));
    }
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 5; j++) {
      QLine bottom = rot.map(QLine(j * 94 + 5, i * 44 + 387, j * 94 + 32, i * 44 + 387));
      grids.push_back(QRect(
        bottom.x1(),
        bottom.y1() - bottom.dx(),
        bottom.dx() * 2,
        bottom.dx()
      ));
    }
}

void Game::setAdjacent() {
  adjacent.resize(60);
  for (auto &adj : adjacent) adj.assign(60, false);
  for (int i = 0; i < 60; i++) {
    if (i >= 5) adjacent[i][i - 5] = true;     // up
    if (i < 55) adjacent[i][i + 5] = true;     // down
    if (i % 5 != 0) adjacent[i][i - 1] = true; // left
    if (i % 5 != 4) adjacent[i][i + 1] = true; // right
  }
  for (int i : camps) {
    adjacent[i][i - 6] = adjacent[i - 6][i] =
    adjacent[i][i - 4] = adjacent[i - 4][i] =
    adjacent[i][i + 4] = adjacent[i + 4][i] =
    adjacent[i][i + 6] = adjacent[i + 6][i] = true;
  }
  adjacent[26][31] = adjacent[31][26] =
  adjacent[28][33] = adjacent[33][28] = false;
}

void Game::setStatus(int i, STATUS s) {
  grids[i].stat = s;
  if (s == EMPTY) {
    pieces[i]->hide();
  } else {
    pieces[i]->setPixmap(pics[s]);
    pieces[i]->show();
  }
}

void Game::focusOn(int f) {
  if (grids[f].stat == EMPTY) return;
  if (grids[f].stat == UNKNOWN) {
    setStatus(f, initStatus[f]);
    if (color == NO) {
      if (grids[f].getColor() == last2) {
        color = COLOR(!available ^ grids[f].getColor());
        qDebug() << (color ? "BLUE" : "RED");
      } else {
        last2 = last1;
        last1 = grids[f].getColor();
      }
    }
    updateRound();
    return;
  }
  if (color == NO || color == available ^ grids[f].getColor()) return;
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
  STATUS fs = grids[f].stat, ts = grids[t].stat;
  if (!isReachable(f, t) ||
      !isAttackable(fs, ts))
    return;

  // execute
  setStatus(f, EMPTY);
  if ((fs ^ ts) == 1 ||
      ts != EMPTY && (
        fs == RB ||
        fs == BB)) {
    setStatus(t, EMPTY);
  } else {
    setStatus(t, fs);
  }
  updateRound();

  // post-check
  if (ts == RM) numOfRM--;
  if (ts == BM) numOfBM--;
  if (ts == RF || ts == BF) win();
}

void Game::updateRound() {
  qDebug() << "Round" << ++round;
  //available = !available; // exchange control
  if (round == 20) emit enableSurrender(true);
}

bool Game::isReachable(int a, int b) {
  // conflict
  if (a == b) return false;

  // protected
  if (grids[b].type == CAMP && grids[b].stat != EMPTY) return false;

  // adjacent
  if (adjacent[a][b]) return true;

  // off railway
  if (grids[a].type != RAILWAY || grids[b].type != RAILWAY) return false;
  
  // Floyd-Warshall
  std::vector<bool> reachable(60, false);
  for (int i = 0; i < 32; i++) {
    for (int j : railwayStations)
      if (j == a || reachable[j] && grids[j].stat == EMPTY)
        for (int k : railwayStations)
          if (adjacent[j][k]) reachable[k] = true;
  }
  if (!reachable[b]) return false;

  // GongBing
  if (grids[a].stat == R1 || grids[a].stat == B1) return true;

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
  return attackable[a][b];
}

void Game::start() {
  available = true;
  round = 0;
  focus = -1; // defocus
  numOfRM = 3;
  numOfBM = 3;

  // set grids
  for (auto &grid : grids) {
    grid.stat = UNKNOWN;
    grid.type = STATION;
  }
  for (int i : railwayStations) {
    grids[i].type = RAILWAY;
  }
  for (int i : camps) {
    grids[i].stat = EMPTY;
    grids[i].type = CAMP;
  }

  // set chess pieces
  for (int i = 0; i < 60; i++)
    setStatus(i, grids[i].stat);

  // initialize pieces randomly
  initStatus.clear();
  for (auto s : {R1, B1, R2, B2, R3, B3, RM, BM})
    initStatus.insert(initStatus.end(), 3, s);
  for (auto s : {R4, B4, R5, B5, R6, B6, R7, B7, RB, BB})
    initStatus.insert(initStatus.end(), 2, s);
  for (auto s : {R8, B8, R9, B9, RF, BF})
    initStatus.insert(initStatus.end(), 1, s);
  std::shuffle(
    initStatus.begin(),
    initStatus.end(),
    std::default_random_engine((unsigned)(new char))
  );
  for (int i : camps)
    initStatus.insert(initStatus.begin() + i, EMPTY);
}

void Game::win() {
  available = false;
  qDebug() << "You Win";
  emit over();
}

void Game::lose() {
  available = false;
  qDebug() << "You Lose";
  emit over();
}