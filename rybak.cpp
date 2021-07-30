#include "rybak.h"
#include "ui_rybak.h"

Rybak::Rybak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Rybak)
{
    ui->setupUi(this);
}

Rybak::~Rybak()
{
    delete ui;
}

