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
const pixelArea WATCH_AREA {600, 650, 135, 180, 239,202,74};
const pixelArea RED_AREA {566, 700, 123, 450, 255, 134, 39};
const pixel START_PIXEL {704, 530, 0x69787F};

#endif // CONSTATNS_H
