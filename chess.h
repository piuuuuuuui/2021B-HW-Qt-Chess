#pragma once
#include <QtWidgets/QMainWindow>
#include <QList>
#include "ui_chess.h"

class Chess : public QMainWindow {
  Q_OBJECT
public:
  Chess(QWidget *parent = Q_NULLPTR);
private:
  Ui::ChessClass ui;
  QWidget *board_;
  QList<QRect> grids_;
  QList<QWidget *> pieces_;
  void setBoard();
  void setPieces();
};
