#pragma once
#include <qwidget.h>
#include <qlist.h>

class Chessboard : public QWidget {
  //Q_OBJECT
public:
  Chessboard(QWidget *parent = Q_NULLPTR);
protected:
  void mousePressEvent(QMouseEvent *);
private:
  QList<QRect> grids_;
  void setGrids();
};
