#include "functions.h"
#include "rybak.h"

#include <QApplication>
#include <QDebug>

const wchar_t* PROC_NAME = L"NewWorld.exe";
HWND g_hWnd = NULL; //хэндлер окна

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DWORD procId= getProcId(PROC_NAME); //ид процесса
    g_hWnd=getWindowHandle(procId, "New World"); //открывает хэндлер окна
    Rybak window;
    window.show();
    return a.exec();
}
