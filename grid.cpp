#include "grid.h"

Grid::Grid(const QRect &gridRect)
    : QRect(gridRect),
      stat(UNKNOWN),
      type(STATION) {}

COLOR Grid::getColor() {
  if (stat < 2) return NO;
  return COLOR(stat & 1);
}