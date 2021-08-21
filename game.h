#pragma once
#include <vector>
#include <set>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class Game : public QWidget {

  // Q_OBJECT

public:
  Game(QWidget *parent = Q_NULLPTR);

protected:
  void mousePressEvent(QMouseEvent *);
  void paintEvent(QPaintEvent *);

private:
  enum STATUS : int;
  enum TYPE : int;

  // textures
  const std::vector<QPixmap> pics;

  // properties
  const std::vector<int> railwayStations;
  const std::vector<int> camps;
  const std::vector<std::set<int>> railways;

  // logic
  std::vector<std::vector<bool>> attackTable;
  std::vector<std::vector<bool>> adjacentTable;

  // grids
  std::vector<QRect> grids;
  std::vector<STATUS> gridStatus, initStatus;
  std::vector<TYPE> gridType;

  // display
  QTransform tr;
  std::vector<QLabel *> pieces;

  // runtime variables
  int focus = -1;
  int numOfRM = 3;
  int numOfBM = 3;

  // initialize
  void setGrids();
  void setAdjacentTable();

  // game control
  void gameStart();
  void gameOver();

  // other
  void setStatus(int, STATUS);
  void focusOn(int);
  void focusOff();
  void moveFromTo(int, int);
  bool isReachable(int, int);
  bool isAttackable(STATUS, STATUS);
};