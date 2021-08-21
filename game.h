#pragma once
#include <vector>
#include <set>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class Game : public QWidget {
  //Q_OBJECT
public:
  Game(QWidget *parent = Q_NULLPTR);
protected:
  void mousePressEvent(QMouseEvent *);
private:
  enum STATUS : int;
  enum TYPE : int;
  const std::vector<QPixmap> pics;
  const std::vector<int> railwayStations;
  const std::vector<std::set<int>> railways;
  const std::vector<std::vector<bool>> attackTable;
  std::vector<QRect> grids;
  std::vector<STATUS> gridStatus, initStatus;
  std::vector<TYPE> gridType;
  QWidget *board = Q_NULLPTR;
  std::vector<QLabel *> pieces;
  int focus = -1;
  int numOfRM = 3;
  int numOfBM = 3;
  void start();
  void setStatus(int, STATUS);
  void focusOn(int);
  void focusOff();
  void moveFromTo(int, int);
  bool isAdjacent(int, int);
  bool isReachable(int, int);
  bool isAttackable(STATUS, STATUS);
};
