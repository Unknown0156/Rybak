#ifndef RYBAK_H
#define RYBAK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Rybak; }
QT_END_NAMESPACE

class Rybak : public QMainWindow
{
    Q_OBJECT

public:
    Rybak(QWidget *parent = nullptr);
    ~Rybak();

private:
    Ui::Rybak *ui;
};
#endif // RYBAK_H
