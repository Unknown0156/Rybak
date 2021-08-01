#include "rybak.h"
#include "ui_rybak.h"

extern HWND g_hWnd; //хэндлер окна

Rybak::Rybak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Rybak)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    m_status=FStatus::waiting;
    connect(ui->start, &QPushButton::clicked, this, &Rybak::start);
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
    Sleep(getRandomNumber(90,120));
    SwitchToThisWindow(g_hWnd, true);
    Sleep(getRandomNumber(90,120));
    if(timer==nullptr){
        timer=new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Rybak::fishing);
    }
    timer->start(TIMER_DELAY);
}

void Rybak::fishing()
{
    if(m_status==FStatus::waiting){
        mouseDown(CAST_PIXEL);
        m_status=FStatus::casting;
    }
    if(m_status==FStatus::casting){

        if (searchPixelPoint(CAST_PIXEL)){
            mouseUp(CAST_PIXEL);
            m_status=FStatus::watching;
        }

    }
    if(m_status==FStatus::watching){
      if(searchPixelArea(WATCH_AREA)){
        mouseDown(CAST_PIXEL);
        Sleep(87);
        mouseUp(CAST_PIXEL);
        m_status=FStatus::fishing;
      }
    }
    if(m_status==FStatus::fishing){
        mouseDown(CAST_PIXEL);
        for(int i=0;i<15;i++){
            if(searchPixelPoint(START_PIXEL)){
                m_status=FStatus::waiting;
                break;
            }
            Sleep(100);
        }
        mouseUp(CAST_PIXEL);
        if(m_status==FStatus::waiting){
            return;
        }else{
            if(searchPixelArea(RED_AREA)){
                Sleep(2500);
            }else{
                Sleep(1000);
            }
        }
    }
}

