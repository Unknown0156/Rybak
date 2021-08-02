#ifndef CONSTATNS_H
#define CONSTATNS_H

#include "windows.h"

struct pixel {
    int x;
    int y;
    COLORREF color;
};

struct pixelArea {
    int x1;
    int x2;
    int y1;
    int y2;
    int red;
    int green;
    int blue;
};

const int TIMER_DELAY = 48; //таймер главного окна
const pixel CAST_PIXEL {640, 352, 0x0BBFFF};
const pixelArea CAST_AREA {630, 645, 336, 500, 255, 191, 11};
const pixelArea WATCH_AREA {560, 700, 130, 330, 239,202,74};
const pixelArea FLOAT_AREA {560, 700, 135, 148, 255, 255, 255};
const pixelArea RED_AREA {560, 700, 130, 500, 255, 134, 39};
const pixelArea FISH_AREA {750, 815, 461, 480, 255, 255, 255};
const pixel START_PIXEL {704, 530, 0x69787F};
const pixelArea POINT_AREA {631, 646, 44, 67, 251, 210, 99};
const pixelArea REPAIR_AREA1 {1125, 1129, 765, 770, 255, 255, 255};
const pixelArea REPAIR_AREA2 {1130, 1135, 765, 770, 255, 255, 255};
const pixel ROD_PIXEL { 577, 521, 0x0};

#endif // CONSTATNS_H
