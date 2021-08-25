#include "timer.h"

Timer::Timer(QWidget *parent)
    : QWidget(parent),
      timer(new QTimer(this)) {
  connect(timer, &QTimer::timeout, this, [&]() {
            update();
            if (--sec == 0) emit timeOver();
          });
}

void Timer::start() {
  sec = 20;
  update();
  timer->start(1000);
}

void Timer::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(QPen(Qt::darkGray, 20));
  p.drawLine(QPointF(0, 0), QPointF(width(), 0));
  p.setPen(QPen(Qt::white, 20));
  p.drawLine(QPointF(0, 0), QPointF((float)width() * sec / 20, 0));
}