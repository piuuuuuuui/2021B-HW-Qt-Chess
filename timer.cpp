#include "timer.h"

Timer::Timer(QWidget *parent)
    : QWidget(parent),
      timer(new QTimer(this)) {
  connect(timer, &QTimer::timeout, this, [&]() {
            timeSec--;
            update();
            if (timeSec == 0) emit timeOver();
          });
}

void Timer::start(int color) {
  timeSec = 20;
  if (color == 0) {
    frontColor = Qt::red;
    backColor = Qt::darkRed;
  } else if (color == 1) {
    frontColor = Qt::blue;
    backColor = Qt::darkBlue;
  } else {
    frontColor = Qt::white;
    backColor = Qt::darkGray;
  }
  update();
  timer->start(1000);
}

void Timer::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(QPen(backColor, height()));
  p.drawLine(QPointF(0, 0), QPointF(width(), 0));
  p.setPen(QPen(frontColor, height()));
  if (timeSec > 0) p.drawLine(QPointF(0, 0), QPointF((float)width() * timeSec / 20, 0));
}