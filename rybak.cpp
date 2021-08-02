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
        default: ui->status->setText("unknown");
        }
    }
}

void Rybak::start()
{
    ui->start->setDisabled(true);
    ui->stop->setEnabled(true);
    setStatus(FStatus::waiting);
    Sleep(getRandomNumber(90,120));
    SwitchToThisWindow(g_hWnd, true);
    Sleep(getRandomNumber(400,500));
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
        stop();
        QWidget::activateWindow();
        QMessageBox reset;
        reset.setWindowTitle("Error");
        reset.setText("Game window lose focus! \n Restart bot?");
        reset.setStandardButtons(QMessageBox::Yes);
        reset.addButton(QMessageBox::No);
        reset.setDefaultButton(QMessageBox::No);
        if(reset.exec() == QMessageBox::Yes){
          start();
        }
    }

    while(!findPixelArea(POINT_AREA)){
        mouse_event(MOUSEEVENTF_ABSOLUTE || MOUSEEVENTF_MOVE,5,0,0,0);
    }

    if(m_status==FStatus::waiting){
        if(findPixelArea(REPAIR_AREA1) && !findPixelArea(REPAIR_AREA2)){
           repair();
        }
        mouseDown(CAST_PIXEL);
        Sleep(getRandomNumber(500,600));
        if(findPixelArea(CAST_AREA)){
            setStatus(FStatus::casting);
        }else{
            mouseUp(CAST_PIXEL);
        }
    }

    if(m_status==FStatus::casting){
        pixel rCastPixel = CAST_PIXEL;
        rCastPixel.y+=getRandomNumber(1,10);
        if (findPixelPoint(rCastPixel)){
            mouseUp(rCastPixel);
            setStatus(FStatus::watching);
        }

    }

    if(m_status==FStatus::watching){
      if(findPixelPoint(START_PIXEL)){
          setStatus(FStatus::waiting);
          Sleep(getRandomNumber(900,1000));
          return;
      }
      if(findPixelArea(FLOAT_AREA)){
          mouse_event(MOUSEEVENTF_ABSOLUTE || MOUSEEVENTF_MOVE,0,-5,0,0);
      }
      if(findPixelArea(WATCH_AREA)){
        Sleep(getRandomNumber(80,120));
        mouseDown(CAST_PIXEL);
        Sleep(getRandomNumber(80,120));
        mouseUp(CAST_PIXEL);
        Sleep(getRandomNumber(80,120));
        mouseDown(CAST_PIXEL);
        Sleep(getRandomNumber(3000,3500));
        mouseUp(CAST_PIXEL);
        Sleep(getRandomNumber(2000,2500));
        setStatus(FStatus::fishing);
      }
    }

    if(m_status==FStatus::fishing){
        if(findPixelPoint(START_PIXEL)){
            setStatus(FStatus::waiting);
            Sleep(getRandomNumber(900,1000));
            return;
        }
        if(!m_mouseDown){
            mouseDown(CAST_PIXEL);
            m_mouseDown=true;
        }
        if(findPixelArea(RED_AREA)){
            mouseUp(CAST_PIXEL);
            m_mouseDown=false;
            setStatus(FStatus::chilling);
        }
        if(findPixelArea(FISH_AREA)){
            mouseUp(CAST_PIXEL);
            m_mouseDown=false;
            Sleep(getRandomNumber(40,50));
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
}

void Rybak::repair()
{
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
        QWidget::activateWindow();
        QMessageBox reset;
        reset.setWindowTitle("Error");
        reset.setText("Cant repair! \n Restart bot?");
        reset.setStandardButtons(QMessageBox::Yes);
        reset.addButton(QMessageBox::No);
        reset.setDefaultButton(QMessageBox::No);
        if(reset.exec() == QMessageBox::Yes){
          start();
        }
    }
}

