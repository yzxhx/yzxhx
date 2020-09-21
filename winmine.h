#ifndef WINMINE_H
#define WINMINE_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <ctime>
#include <QTimer>
#include <QPushButton>
#include "block.h"
#include "customize_window.h"
#include "rank.h"
#include "dialog.h"

#define block_length 35
#define block_height 35
#define title_begin 23
#define title_height  (title_begin+100)

namespace Ui {
class Winmine;
}

class Winmine : public QMainWindow
{
    Q_OBJECT

public:
    explicit Winmine(QWidget *parent = 0);
    ~Winmine();
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    QString paint_flag="none";
    int end=0;
    bool showimage=1;
    void painttitle(QPainter *painter);
    void easy();
    void middle();
    void hard();
    void setcustomize(int row,int col,int boomnum);
    void readplayername(QString s);

private:
    Ui::Winmine *ui;
    QTimer *runtime;
    Customize_window w;
    rank w2;
    dialog w3;

private slots:
    void second();
    void restart_clicked();
    void window_show();
    void list_show();
    void dialogshow();


protected:

};
void showblock(int x,int y);
void paintboom(QPainter *painter);
#endif // WINMINE_H
