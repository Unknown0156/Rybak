#include "rybak.h"
#include "ui_rybak.h"

#include "QMessageBox"

#include "QDebug"

extern HWND g_hWnd; //хэндлер окна

Rybak::Rybak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Rybak)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    m_status=FStatus::waiting;
    connect(ui->start, &QPushButton::clicked, this, &Rybak::start);
    connect(ui->stop, &QPushButton::clicked, this, &Rybak::stop);
}

Rybak::~Rybak()
{
    if(timer!=nullptr)
        delete timer;
    delete ui;
}

void Rybak::setStatus (FStatus status){//изменение статуса игрока
    if (status!=m_status){
        m_status=status;
        switch (m_status) {
        case FStatus::waiting:ui->status->setText("waiting");break;
        case FStatus::casting:ui->status->setText("casting");break;
        case FStatus::watching:ui->status->setText("watching");break;
        case FStatus::fishing:ui->status->setText("fishing");break;
        case FStatus::chilling:ui->status->setText("chilling");break;
        case FStatus::repairing:ui->status->setText("repairing");break;
        default: ui->status->setText("unknown");
        }
    }
}

void Rybak::start()
{
    ui->start->setDisabled(true);
    ui->stop->setEnabled(true);
    setStatus(FStatus::waiting);
    Sleep(getRandomNumber(5000,6000));
    SwitchToThisWindow(g_hWnd, true);
    /*RECT wRect;
    if(GetWindowRect(g_hWnd, &wRect)){
        int wWidth=wRect.right-wRect.left;
        int wHeight=wRect.bottom-wRect.top;
        MoveWindow(g_hWnd, 0, 0, wWidth, wHeight, false);
    }*/
    Sleep(getRandomNumber(900,1500));
    if(timer==nullptr){
        timer=new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Rybak::fishing);
    }
    timer->start(TIMER_DELAY);
}

void Rybak::stop()
{
    ui->start->setDisabled(false);
    ui->stop->setEnabled(false);
    mouseUp(CAST_PIXEL);
    if(timer){
        timer->stop();
    }
}

void Rybak::fishing()
{
    if(GetForegroundWindow()!=g_hWnd){
        Sleep(getRandomNumber(1500,1600));
        stop();
        Sleep(getRandomNumber(1500,1600));
        start();
    }



    if(m_status==FStatus::waiting){
        if(!findPixelArea(POINT_AREA)){
            mouse_event(MOUSEEVENTF_ABSOLUTE || MOUSEEVENTF_MOVE,20,0,0,0);
            return;
        }
        if(findPixelArea(REPAIR_AREASTART) && !findPixelArea(REPAIR_AREA)){
           setStatus(FStatus::repairing);
           return;
        }
        if(!findPixelArea(ROD_PIXELAREA) && !findPixelArea(ROD_PIXELAREA2)){
           setStatus(FStatus::repairing);
           return;
        }
        keybd_event( 0x41, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(70,80));
        keybd_event( 0x41, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(400,500));
        keybd_event( 0x44, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(70,80));
        keybd_event( 0x44, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(500,600));
        mouseDown(CAST_PIXEL);
        Sleep(getRandomNumber(500,600));
        if(findPixelArea(CAST_AREA)){
            setStatus(FStatus::casting);
        }else{
            mouseUp(CAST_PIXEL);
            Sleep(getRandomNumber(400,600));
        }
    }

    if(m_status==FStatus::casting){
        pixel rCastPixel = CAST_PIXEL;
        rCastPixel.y+=getRandomNumber(0,5);
        if (findPixelArea(CAST_PIXELAREA)){
            mouseUp(rCastPixel);
            setStatus(FStatus::watching);
        }
        if(findPixelPoint(START_PIXEL)){
            setStatus(FStatus::waiting);
            Sleep(getRandomNumber(900,1000));
            return;
        }

    }

    if(m_status==FStatus::watching){
      if(findPixelPoint(START_PIXEL)){
          setStatus(FStatus::waiting);
          Sleep(getRandomNumber(900,1000));
          return;
      }
      if(findPixelArea(FLOAT_AREA)){
          mouse_event(MOUSEEVENTF_ABSOLUTE || MOUSEEVENTF_MOVE,0,-10,0,0);
      }
      if(findPixelArea(FLOAT_AREA2)){
          mouse_event(MOUSEEVENTF_ABSOLUTE || MOUSEEVENTF_MOVE,0,15,0,0);
      }
      if(findPixelArea(WATCH_AREA)){
        Sleep(getRandomNumber(80,120));
        mouseDown(CAST_PIXEL);
        Sleep(getRandomNumber(80,120));
        mouseUp(CAST_PIXEL);
        Sleep(getRandomNumber(80,120));
        mouseDown(CAST_PIXEL);
        Sleep(getRandomNumber(2000,2500));
        mouseUp(CAST_PIXEL);
        Sleep(getRandomNumber(2000,2500));
        setStatus(FStatus::fishing);
      }
    }

    if(m_status==FStatus::fishing){
        if(findPixelPoint(START_PIXEL)){
            m_mouseDown=false;
            setStatus(FStatus::waiting);
            Sleep(getRandomNumber(900,1000));
            return;
        }
        if(!m_mouseDown){
            mouseDown(CAST_PIXEL);
            m_mouseDown=true;
        }
        if(findPixelArea(RED_AREA)){
            Sleep(getRandomNumber(120,180));
            mouseUp(CAST_PIXEL);
            m_mouseDown=false;
            setStatus(FStatus::chilling);
        }
        if(findPixelArea(FISH_AREA)){
            mouseUp(CAST_PIXEL);
            m_mouseDown=false;
            Sleep(getRandomNumber(80,150));
            mouseDown(CAST_PIXEL);
            Sleep(getRandomNumber(80,120));
            mouseUp(CAST_PIXEL);
            setStatus(FStatus::waiting);
        }
    }
    if(m_status==FStatus::chilling){
        if(m_chillCount<15){
            Sleep(getRandomNumber(80,120));
        }else{
            m_chillCount=0;
            setStatus(FStatus::fishing);
        }
        m_chillCount++;
    }
    if(m_status==FStatus::repairing){
        Sleep(getRandomNumber(900,1000));
        keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(70,80));
        keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(100,150));
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,(ROD_PIXEL.x*65535/1920),(ROD_PIXEL.y*65535/1080 ),0,0);
        Sleep(getRandomNumber(100,150));
        keybd_event( 0x52, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(100,150));
        mouse_event(MOUSEEVENTF_LEFTDOWN, getRandomNumber(0,5), getRandomNumber(0,5), 0, 0);
        Sleep(getRandomNumber(70,80));
        mouse_event(MOUSEEVENTF_LEFTUP, getRandomNumber(0,5), getRandomNumber(0,5), 0, 0);
        Sleep(getRandomNumber(100,150));
        keybd_event( 0x52, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(100,150));
        keybd_event( 0x45, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(70,80));
        keybd_event( 0x45, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(100,150));
        keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(70,80));
        keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(1500,2000));
        keybd_event( VK_F3, 0, KEYEVENTF_EXTENDEDKEY, 0 );
        Sleep(getRandomNumber(70,80));
        keybd_event( VK_F3, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        Sleep(getRandomNumber(1500,2000));
        if(findPixelPoint(START_PIXEL)){


            keybd_event( 0x41, 0, KEYEVENTF_EXTENDEDKEY, 0 );
            Sleep(getRandomNumber(70,80));
            keybd_event( 0x41, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
            Sleep(getRandomNumber(90,100));
            keybd_event( 0x44, 0, KEYEVENTF_EXTENDEDKEY, 0 );
            Sleep(getRandomNumber(70,80));
            keybd_event( 0x44, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );


            Sleep(getRandomNumber(400,500));
            keybd_event( 0x52, 0, KEYEVENTF_EXTENDEDKEY, 0 );
            Sleep(getRandomNumber(70,80));
            keybd_event( 0x52, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
            Sleep(getRandomNumber(100,150));
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,(BAIT_PIXEL.x*65535/1920),(BAIT_PIXEL.y*65535/1080 ),0,0);
            Sleep(getRandomNumber(100,150));
            mouse_event(MOUSEEVENTF_LEFTDOWN, getRandomNumber(0,5), getRandomNumber(0,5), 0, 0);
            Sleep(getRandomNumber(70,80));
            mouse_event(MOUSEEVENTF_LEFTUP, getRandomNumber(0,5), getRandomNumber(0,5), 0, 0);
            Sleep(getRandomNumber(100,150));
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,(EQ_BAIT_PIXEL.x*65535/1920),(EQ_BAIT_PIXEL.y*65535/1080 ),0,0);
            Sleep(getRandomNumber(100,150));
            mouse_event(MOUSEEVENTF_LEFTDOWN, getRandomNumber(0,5), getRandomNumber(0,5), 0, 0);
            Sleep(getRandomNumber(70,80));
            mouse_event(MOUSEEVENTF_LEFTUP, getRandomNumber(0,5), getRandomNumber(0,5), 0, 0);
            Sleep(getRandomNumber(100,150));

            setStatus(FStatus::waiting);
            Sleep(getRandomNumber(900,1000));
        }else{
            stop();
            Sleep(1000);
            start();
        }
    }
}

void Rybak::repair()
{

    Sleep(getRandomNumber(900,1000));
    keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY, 0 );
    Sleep(getRandomNumber(70,80));
    keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    Sleep(getRandomNumber(100,150));
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,(ROD_PIXEL.x*65535/1920),(ROD_PIXEL.y*65535/1080 ),0,0);
    Sleep(getRandomNumber(100,150));
    keybd_event( 0x52, 0, KEYEVENTF_EXTENDEDKEY, 0 );
    Sleep(getRandomNumber(100,150));
    mouse_event(MOUSEEVENTF_LEFTDOWN, getRandomNumber(5,10), getRandomNumber(5,10), 0, 0);
    Sleep(getRandomNumber(70,80));
    mouse_event(MOUSEEVENTF_LEFTUP, getRandomNumber(5,10), getRandomNumber(5,10), 0, 0);
    Sleep(getRandomNumber(100,150));
    keybd_event( 0x52, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    Sleep(getRandomNumber(100,150));
    keybd_event( 0x45, 0, KEYEVENTF_EXTENDEDKEY, 0 );
    Sleep(getRandomNumber(70,80));
    keybd_event( 0x45, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    Sleep(getRandomNumber(100,150));
    keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY, 0 );
    Sleep(getRandomNumber(70,80));
    keybd_event( VK_TAB, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    Sleep(getRandomNumber(1500,2000));
    keybd_event( VK_F3, 0, KEYEVENTF_EXTENDEDKEY, 0 );
    Sleep(getRandomNumber(70,80));
    keybd_event( VK_F3, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    Sleep(getRandomNumber(1500,2000));
    if(findPixelPoint(START_PIXEL)){
        setStatus(FStatus::waiting);
        Sleep(getRandomNumber(900,1000));
    }else{
        stop();
        Sleep(1000);
        start();
    }
}

