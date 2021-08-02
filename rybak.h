#ifndef RYBAK_H
#define RYBAK_H

#include <QMainWindow>
#include <QTimer>

#include "windows.h"

#include "constatns.h"
#include "functions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Rybak; }
QT_END_NAMESPACE

enum FStatus //что делает персонаж
{
    waiting,
    casting,
    watching,
    fishing,
    chilling
};

class Rybak : public QMainWindow
{
    Q_OBJECT

public:
    Rybak(QWidget *parent = nullptr);
    ~Rybak();

    void setStatus (FStatus status);
    void start();
    void stop();
    void fishing();
    void repair();

private:
    Ui::Rybak *ui;
    QTimer *timer=nullptr;
    FStatus m_status;
    bool m_mouseDown=false;
    int m_chillCount=0;
};

#endif // RYBAK_H
