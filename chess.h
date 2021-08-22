#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_chess.h"
#include "game.h"

class Chess : public QMainWindow {

  Q_OBJECT

public:
  Chess(QWidget *parent = Q_NULLPTR);

protected:
  void paintEvent(QPaintEvent *);

private:
  Ui::ChessClass ui;
  Game *game;
};
