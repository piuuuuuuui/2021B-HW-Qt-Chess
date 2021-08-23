#pragma once
#include <QtWidgets/QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "ui_chess.h"
#include "game.h"

class Chess : public QMainWindow {

  Q_OBJECT

public:
  Chess(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *);

private:
  Ui::ChessClass ui;
  QTcpServer *tcpServer;
  QTcpSocket *tcpSocketServer, *tcpSocketClient;
  Game *game = nullptr;

  void connecting(bool);
  void gameInit();

public slots:
  void setServer();
  void setClient();
  void disconnect();
  void gameStart();
  void gameOver();

private slots:
  void onNewConnection();
  void onConnectedToServer();
  void onDisconnectedFromhost();
};
