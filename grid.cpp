#include "grid.h"

Grid::Grid(const QRect &gridRect)
    : QRect(gridRect),
      stat(UNKNOWN),
      type(STATION) {}