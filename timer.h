#pragma once
#include <QWidget>
#include <QTimer>
#include <QPainter>

class Timer : public QWidget {

  Q_OBJECT

public:
  Timer(QWidget *parent = nullptr);
  void start();

protected:
  void paintEvent(QPaintEvent *);

private:
  int sec;
  QTimer *timer;

signals:
  void timeOver();
};