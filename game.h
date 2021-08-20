#pragma once
#include <QWidget>
#include <QList>
#include <QMouseEvent>

class Game : public QWidget {
  //Q_OBJECT
public:
  Game(QWidget *parent = Q_NULLPTR);
protected:
  void mousePressEvent(QMouseEvent *);
private:
  enum class STATUS { EMPTY, UNKNOWN };
  enum class TYPE { BINGZHAN, XINGYING };
  QWidget *board_;
  QList<QRect> grids_;
  QList<STATUS> gridStatus_;
  QList<TYPE> gridType_;
  QList<QWidget *> pieces_;
  void start();
};
