#pragma once
#include <QWidget>
#include <QTimer>
#include <QPainter>

class Timer : public QWidget {

  Q_OBJECT

public:
  Timer(QWidget *parent = nullptr);
  void start(int color = 2);

protected:
  void paintEvent(QPaintEvent *);

private:
  int timeSec;
  Qt::GlobalColor frontColor, backColor;
  QTimer *timer;

signals:
  void timeOver();
};