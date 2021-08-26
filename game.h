#pragma once
#include <vector>
#include <set>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include "grid.h"
#include "timer.h"

class Game : public QWidget {

  Q_OBJECT

public:
  Game(QWidget *parent = nullptr);
  ~Game();
  void clickOn(int);

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);

private:
  // textures
  const std::vector<QPixmap> pics;
  // properties
  const std::vector<int> railwayStations;
  const std::vector<int> camps;
  const std::vector<std::set<int>> railways;
  // logic
  std::vector<std::vector<bool>> attackable;
  std::vector<std::vector<bool>> adjacent;
  // grids
  std::vector<Grid> grids;
  std::vector<STATUS> initStatus;
  // display
  QTransform rot;
  std::vector<QLabel *> pieces;
  Timer *timer;
  // runtime variables
  COLOR color, last1, last2;
  bool available = false;
  int round;
  int timeOver;
  int focus;
  int numOfRM;
  int numOfBM;

  void updateRound();
  void setStatus(int, STATUS);
  void focusOn(int);
  void focusOff();
  void moveFromTo(int, int);
  bool isReachable(int, int);
  bool isAttackable(STATUS, STATUS);

public slots:
  void start(unsigned, bool);
  void win();
  void lose();

signals:
  void clicked(int);
  void enableResign(bool);
  void over();
};