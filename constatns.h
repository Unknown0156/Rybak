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

const int TIMER_DELAY = 89; //таймер главного окна
const pixel CAST_PIXEL {640, 325, 0x0BBFFF};
const pixelArea CAST_PIXELAREA {635, 645, 320, 330, 255, 191, 11};
const pixelArea CAST_AREA {630, 645, 336, 500, 255, 191, 11};
const pixelArea WATCH_AREA {560, 700, 130, 450, 239,202,74};
const pixelArea FLOAT_AREA {560, 700, 135, 140, 255, 255, 255};
const pixelArea FLOAT_AREA2 {560, 700, 350, 675, 255, 255, 255};
const pixelArea RED_AREA {560, 700, 130, 550, 255, 134, 39};
const pixelArea FISH_AREA {600, 685, 570, 585, 255, 255, 255};
const pixel START_PIXEL {704, 506, 0x69787F};
const pixelArea POINT_AREA {631, 646, 44, 67, 251, 210, 99};
const pixelArea REPAIR_AREASTART {1125, 1130, 715, 725, 255, 255, 255};
const pixelArea REPAIR_AREA {1165, 1170, 715, 725, 255, 255, 255};
const pixel ROD_PIXEL { 577, 477, 0x0};
const pixelArea ROD_PIXELAREA {1170, 1178, 693, 698, 235, 236, 235};
const pixelArea ROD_PIXELAREA2 {1201, 1207, 686, 690, 242, 238, 232};
const pixel BAIT_PIXEL { 785, 340, 0x0};
const pixel EQ_BAIT_PIXEL { 1000, 585, 0x0};
#endif // CONSTATNS_H
