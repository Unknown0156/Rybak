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
        mouseDown(CAST_PIXEL);
        while(!findPixelArea(RED_AREA)){
            Sleep(getRandomNumber(80,120));
            if(findPixelArea(FISH_AREA)){
                break;
            }
        }
        mouseUp(CAST_PIXEL);
        if(findPixelArea(FISH_AREA)){
            mouseDown(CAST_PIXEL);
            Sleep(getRandomNumber(80,120));
            mouseUp(CAST_PIXEL);
            setStatus(FStatus::waiting);
        }else{
            for (int i=0;i<20;i++) {
                Sleep(getRandomNumber(80,120));
                QApplication::processEvents();
            }
        }
    }
}

