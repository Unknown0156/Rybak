#include "functions.h"

DWORD getProcId (const wchar_t* procName) //определение ид процесса по имени исполняемого файла
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //делает снимок всех процессов в системе
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry; //структура процесса из снимка
        procEntry.dwSize = sizeof(procEntry); //устанавливает размер

        if (Process32First(hSnap, &procEntry)) //первый процесс в снимке
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName)) //имя ехе файла процесса совпадает с искомым
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry)); //следующий процесс в снимке

        }
    }
    CloseHandle(hSnap); //закрывает снимок
    return procId; //возвращает ид процесса, либо 0 если процесс не найден
}

HWND getWindowHandle(DWORD procId, const QString &title) //определние хэндлера окна по ид процесса
{
    HWND hWnd=0;//нужный хэндлер
    HWND curHWnd = NULL;//текущий хэндлер окна
    do
    {
        curHWnd = FindWindowEx(NULL, curHWnd, NULL, NULL);//ищет следующее окно
        DWORD curProcId = 0;//текущий ид процесса
        GetWindowThreadProcessId(curHWnd, &curProcId);//получает ид процесса окна
        if (curProcId == procId)
        {
            char curTitle[256];
            GetWindowTextA(curHWnd, curTitle,sizeof(curTitle));//получает заголовок окна
            QString qcurTitle = QString::fromStdString(curTitle);
            if (qcurTitle.contains(title)){//если заголов сожержит нужный текст
                hWnd=curHWnd;
                break;
            }
        }
    }
    while (curHWnd != NULL);
    return hWnd;
}

void mouseDown(pixel cPixel) //нажатие кнопки
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, getRandomNumber(10,20), getRandomNumber(10,20), 0, 0);
}

void mouseUp(pixel cPixel) //отжатие кнопки
{
    mouse_event(MOUSEEVENTF_LEFTUP, getRandomNumber(10,20), getRandomNumber(10,20), 0, 0);
}

bool findPixelPoint(pixel cPixel)
{
    HDC screen=GetDC(NULL);
    if(GetPixel(screen, cPixel.x, cPixel.y)==cPixel.color){
        ReleaseDC(NULL, screen);
        return true;
    }
    ReleaseDC(NULL, screen);
    return false;
}

bool findPixelArea(pixelArea cArea)
{
    HWND hWnd = GetDesktopWindow();
    HDC hdc = GetDC(hWnd);

    RECT rect;
    GetWindowRect(hWnd, &rect);

    int MAX_WIDTH = rect.right - rect.left;
    int MAX_HEIGHT = rect.bottom - rect.top;

    HDC compHdc = CreateCompatibleDC(hdc);

    BITMAPINFO bitmap;
    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = MAX_WIDTH;
    bitmap.bmiHeader.biHeight = -MAX_HEIGHT;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 32;
    bitmap.bmiHeader.biCompression = BI_RGB;
    bitmap.bmiHeader.biSizeImage = 0;
    bitmap.bmiHeader.biClrUsed = 0;
    bitmap.bmiHeader.biClrImportant = 0;

    LPRGBQUAD bitPointer;
    HBITMAP hBitmap = CreateDIBSection(compHdc, &bitmap, DIB_RGB_COLORS, (void**)&bitPointer, 0, 0);

    HBITMAP hbmpOld = (HBITMAP) SelectObject(compHdc, hBitmap);
    BitBlt(compHdc, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, 0, 0, SRCCOPY);

    for(int i=cArea.x1;i<cArea.x2;i++){
        for(int j=cArea.y1;j<cArea.y2;j++){
            LPRGBQUAD pColor = &bitPointer[(MAX_WIDTH*j)+i];
            int red = pColor->rgbRed;
            int green = pColor->rgbGreen;
            int blue = pColor->rgbBlue;
            if((abs(red-cArea.red))<5 && (abs(green-cArea.green))<5 && (abs(blue-cArea.blue))<5){
                SelectObject(compHdc, hbmpOld);
                DeleteObject(hBitmap);
                DeleteDC(compHdc);
                ReleaseDC(hWnd, hdc);
                return true;
            }
        }
        QApplication::processEvents();
    }
    SelectObject(compHdc, hbmpOld);
    DeleteObject(hBitmap);
    DeleteDC(compHdc);
    ReleaseDC(hWnd, hdc);
    return false;
}

int getRandomNumber(int min, int max) //генератор рандомных чисел (от, до)
{
    std::random_device rd; //случайное системное число (зависит от ОС)
    std::mt19937 mersenne(rd()); // инициализирует Вихрь Мерсенна случайным системным числом
    std::uniform_int_distribution <int> dist(min, max); //задаем интервал целых чисел
    return dist(mersenne); //генерирует следующее целое число в интервале с сидом из вихря
}
