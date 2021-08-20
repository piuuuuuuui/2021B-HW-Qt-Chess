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
  std::vector<QRect> grids;
  std::vector<STATUS> gridStatus, initStatus;
  std::vector<TYPE> gridType;
  QWidget *board;
  std::vector<QLabel *> pieces;
  int cache;
  void start();
  void setStatus(int, STATUS);
};
