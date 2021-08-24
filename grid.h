#pragma once
#include <QRect>

enum COLOR : int {RED, BLUE, NO};

enum STATUS : int {
  EMPTY,
  UNKNOWN,
  R1, B1, // GongBing
  R2, B2, // PaiZhang
  R3, B3, // LianZhang
  R4, B4, // YingZhang
  R5, B5, // TuanZhang
  R6, B6, // LvZhang
  R7, B7, // ShiZhang
  R8, B8, // JunZhang
  R9, B9, // SiLing
  RF, BF, // Flag
  RB, BB, // Bomb
  RM, BM, // Mine
};

enum TYPE : int {STATION, RAILWAY, CAMP};

struct Grid : public QRect {
  Grid(const QRect &);
  STATUS stat;
  TYPE type;

  COLOR getColor();
};