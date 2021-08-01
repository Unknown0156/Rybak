#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include <random>

#include <QString>
#include <QApplication>

#include <Windows.h>
#include <TlHelp32.h>

#include "constatns.h"

DWORD getProcId(const wchar_t* procName); //определение ид процесса по имени исполняемого файла
HWND getWindowHandle(DWORD procId, const QString &title);//определние хэндлера окна

void mouseDown(pixel cPixel); //нажатие мыши
void mouseUp(pixel cPixel); //отжатие кнопки

bool searchPixelPoint(pixel cPixel);
bool searchPixelArea(pixelArea cArea);

int getRandomNumber(int min = INT16_MIN, int max = INT16_MAX); //генератор рандомных чисел (от, до)

#endif // FUNCTIONS_H
