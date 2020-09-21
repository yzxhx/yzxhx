#ifndef CUSTOMIZE_WINDOW_H
#define CUSTOMIZE_WINDOW_H

#include <QMainWindow>

namespace Ui {
class Customize_window;
}

class Customize_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Customize_window(QWidget *parent = 0);
    ~Customize_window();

private slots:
    void senddata();

private:
    Ui::Customize_window *ui;

signals:
    void slot(int row,int col,int boomnum);
};

#endif // CUSTOMIZE_WINDOW_H
