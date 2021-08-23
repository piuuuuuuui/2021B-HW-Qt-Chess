#pragma once
#include <QtWidgets/QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
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
  QTcpServer *server;
  QTcpSocket *socket;
  Game *game = Q_NULLPTR;

  void connecting(bool);
  void gameInit();

public slots:
  void setServer();
  void setClient();
  void disconnect();
  void gameStart();
  void gameOver();
};
