#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_chess.h"

class Chess : public QMainWindow {
  Q_OBJECT
public:
  Chess(QWidget *parent = Q_NULLPTR);
private:
  Ui::ChessClass ui;
};
