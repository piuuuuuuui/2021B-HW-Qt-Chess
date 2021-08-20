#pragma once
#include <vector>
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
  const std::vector<std::vector<bool>> attackable;
  std::vector<QRect> grids;
  std::vector<STATUS> gridStatus, initStatus;
  std::vector<TYPE> gridType;
  QWidget *board;
  std::vector<QLabel *> pieces;
  int focus;
  void start();
  void setStatus(int, STATUS);
  void focusOn(int);
  void focusOff();
  void moveFromTo(int, int);
  bool isReachable(int, int);
  bool isAttackable(STATUS, STATUS);
};
