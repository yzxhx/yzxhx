#include "winmine.h"
#include "ui_winmine.h"
#include <iostream>
#include<QTextStream>
#include <QGraphicsWidget>
#include <QPropertyAnimation>

int initial_row=9;
int initial_col=9;
int initial_boomnum=10;
int click_x,click_y,timeend=1;

QPainter *paintmywindow;
block *mine=new block(initial_row,initial_col,initial_boomnum);
int length=mine->getcol()*35;
int height=mine->getrow()*35+title_height;
int boom_number=mine->getboomnum();
int flag_number=boom_number;
int win();
Winmine::Winmine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Winmine)
{
    ui->setupUi(this);
    runtime = new QTimer(this);
    connect(runtime,SIGNAL(timeout()),this,SLOT(second()));
    connect(runtime,SIGNAL(timeout()),this,SLOT(update()));

    connect(ui->Easy,&QAction::triggered,this,&Winmine::easy);
    connect(ui->Middle,&QAction::triggered,this,&Winmine::middle);
    connect(ui->Hard,&QAction::triggered,this,&Winmine::hard);
    connect(ui->Customize,&QAction::triggered,this,&Winmine::window_show);

    void(Customize_window::*funslot)(int row,int col,int boomnum)=&Customize_window::slot;
    connect(&w,funslot,this,&Winmine::setcustomize);

    connect(ui->listinfo,&QAction::triggered,this,&Winmine::list_show);

    void(dialog::*okclicked)(QString s)=&dialog::slotname;
    connect(&w3,okclicked,this,&Winmine::readplayername);

    runtime = new QTimer(this);
    connect(runtime,SIGNAL(timeout()),this,SLOT(second()));
    connect(runtime,SIGNAL(timeout()),this,SLOT(update()));
}
void Winmine::readplayername(QString s){
    w2.setplayername(s);
    int zzcj=mine->gettimenum();
    w2.settime(zzcj);
    w2.px();
}
void Winmine::mousePressEvent(QMouseEvent *event)
{
    click_x=event->x();
    click_y=event->y();
    if(click_x>block_length*mine->getcol()/2-block_length/2&&click_x<block_length*mine->getcol()/2-block_length/2+block_length
       &&click_y>title_begin+title_height/3&&click_y<title_begin+title_height/3+block_height){
        restart_clicked();
        paint_flag="none";
        end=0;
        runtime->stop();
        update();
        return;
    }
    if(click_y<title_height)
         return;
    else if(end==1)
         return;

    click_x=click_x/block_length;
    click_y=(click_y-title_height)/block_height;

    if(event->buttons()==(Qt::LeftButton)){
        //开始计时
        if(timeend==1)
           runtime->start(1000);
        //点击到炸弹
        if(mine->p[click_y][click_x]==99){
            paint_flag="boom";
            mine->p[click_y][click_x]=199;
            update();
            end=1;
            timeend=1;   //游戏结束，停止计时
            runtime->stop();
            QMessageBox::warning(this,"END","YOU LOSE");
        }
        else if(mine->p[click_y][click_x]<10){
             if(mine->p[click_y][click_x]!=0)
                mine->p[click_y][click_x]=mine->p[click_y][click_x]+100;
             else
                showblock(click_y,click_x);

            paint_flag="number";
            update();
            if(win()){
                timeend=1;
                end = 1;
                runtime->stop();
                QMessageBox::warning(this,"END","YOU WIN");
                dialogshow();
            }
        }
        else
            return;
    }

   else if(event->buttons()==(Qt::RightButton)){
        //判断未插旗方块，表示插旗
        if((mine->p[click_y][click_x]<10)||(mine->p[click_y][click_x]==99)){
            //若旗帜插满则不响应
            if(flag_number==0){
                return;
            }
            mine->p[click_y][click_x]=mine->p[click_y][click_x]+50;//
            flag_number--;//控制插旗数量
            paint_flag="flag";
        }
        //判断插旗方块，表示打问号
   else if((mine->p[click_y][click_x]>=50&&mine->p[click_y][click_x]<60)||(mine->p[click_y][click_x]==149)){
            mine->p[click_y][click_x]=mine->p[click_y][click_x]+150;
            paint_flag="question";
            flag_number++;
        }
        //判断问号方块，去掉问号
   else if((mine->p[click_y][click_x]>=200&&mine->p[click_y][click_x]<210)||(mine->p[click_y][click_x]==299))//判断问好方块，去掉问号
        {
            mine->p[click_y][click_x]=mine->p[click_y][click_x]-200;
            paint_flag=="flag";
        }
        update();
        if(win()==1)
        {
            timeend=1;
            end = 1;
            runtime->stop();
            QMessageBox::warning(this,"END","YOU WIN");
            dialogshow();
        }
    }
}

void Winmine::paintEvent(QPaintEvent *event)
{
    this->setFixedSize(mine->getcol()*block_length,mine->getrow()*block_height+title_height);
    QPainter painter(this);
    paintmywindow=&painter;
    painttitle(paintmywindow);
    QPixmap initial("://new/prefix1/images/initial.png");
    if(paint_flag=="none")
       // qDebug()<<"画出初始界面";
       for(int i=0;i<mine->getcol()*block_length;i+=block_length)
          for(int j=title_height;j<mine->getrow()*block_height+title_height;j+=block_height)
            painter.drawPixmap(i,j,initial,0,0,block_length,block_length);
       //qDebug()<<i<<" "<<j;
    else if(paint_flag=="boom"){
        QPixmap initial("://new/prefix1/images/initial.png");
        QPixmap explode("://new/prefix1/images/explode.png");
        QPixmap flag("://new/prefix1/images/flag.png");
        QPixmap blank("://new/prefix1/images/blank.png");
        QPixmap question("://new/prefix1/images/question.png");
        QPixmap blk_1("://new/prefix1/images/1.png");
        QPixmap blk_2("://new/prefix1/images/2.png");
        QPixmap blk_3("://new/prefix1/images/3.png");
        QPixmap blk_4("://new/prefix1/images/4.png");
        QPixmap blk_5("://new/prefix1/images/5.png");
        QPixmap blk_6("://new/prefix1/images/6.png");
        QPixmap blk_7("://new/prefix1/images/7.png");
        QPixmap blk_8("://new/prefix1/images/8.png");
        //重新绘制雷区
        for(int i=0;i<mine->getrow();i++){
            for(int j=0;j<mine->getcol();j++){
                if(mine->p[i][j]<10){
                    painter.drawPixmap(j*block_length,i*block_height+title_height,initial,0,0,block_length,block_height);
                }
                //显示所有炸弹
                else if(mine->p[i][j]==99){
                    painter.drawPixmap(j*block_length,i*block_height+title_height,explode,0,0,block_length,block_height);
                }
                //显示所有旗帜
                else if(mine->p[i][j]>110||(mine->p[i][j]>=50)&&(mine->p[i][j]<60)){
                    painter.drawPixmap(j*block_length,i*block_height+title_height,flag,0,0,block_length,block_height);
                }
                else if((mine->p[i][j])>99&&(mine->p[i][j]<110)){
                   int blk_num=mine->p[i][j]-100;
                   switch (blk_num){
                    case 0: painter.drawPixmap(j*block_length,i*block_height+title_height,blank,0,0,block_length,block_height);break;
                    case 1: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_1,0,0,block_length,block_height);break;
                    case 2: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_2,0,0,block_length,block_height);break;
                    case 3: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_3,0,0,block_length,block_height);break;
                    case 4: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_4,0,0,block_length,block_height);break;
                    case 5: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_5,0,0,block_length,block_height);break;
                    case 6: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_6,0,0,block_length,block_height);break;
                    case 7: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_7,0,0,block_length,block_height);break;
                    case 8: painter.drawPixmap(j*block_length,i*block_height+title_height,blk_8,0,0,block_length,block_height);break;
                   }
                }
             }
         }
         painter.drawPixmap(click_x*block_length,click_y*block_height+title_height,explode,0,0,block_length,block_height);
    }
    else if(paint_flag=="number"){
        paintboom(paintmywindow);
    }
    else if(paint_flag=="flag"){
        paintboom(paintmywindow);
    }
    else if(paint_flag=="question"){
        paintboom(paintmywindow);
    }
}

void showblock(int x, int y){

    for(int row=x-1;row<=x+1;row++)
        for(int col=y-1;col<=y+1;col++){
            if(row<0||col<0||row>=mine->getrow()||col>=mine->getcol())continue;
            if(mine->p[row][col]<10){
                //如果区域中炸弹数为0，显示周围区域
                if(mine->p[row][col]==0){
                    mine->p[row][col]=100;
                    showblock(row,col);
                }
                else{
                    mine->p[row][col]=mine->p[row][col]+100;
                }
            }
        }
}

void Winmine::painttitle(QPainter *painter)
{
    QPixmap initial("://new/prefix1/images/initial.png");
    QPixmap explode("://new/prefix1/images/explode.png");
    QPixmap flag("://new/prefix1/images/flag.png");
    QPixmap blank("://new/prefix1/images/blank.png");
    QPixmap question("://new/prefix1/images/question.png");
    QPixmap blk_1("://new/prefix1/images/1.png");
    QPixmap blk_2("://new/prefix1/images/2.png");
    QPixmap blk_3("://new/prefix1/images/3.png");
    QPixmap blk_4("://new/prefix1/images/4.png");
    QPixmap blk_5("://new/prefix1/images/5.png");
    QPixmap blk_6("://new/prefix1/images/6.png");
    QPixmap blk_7("://new/prefix1/images/7.png");
    QPixmap blk_8("://new/prefix1/images/8.png");
    QPixmap title("://new/prefix1/images/title.bmp");
    QPixmap again("://new/prefix1/images/again.png");
    QPixmap again2("://new/prefix1/images/again2.png");
    QPixmap lose("://new/prefix1/images/lose.png");
    QPixmap digital0("://new/prefix1/images/time_digital0.png");
    QPixmap digital1("://new/prefix1/images/time_digital1.png");
    QPixmap digital2("://new/prefix1/images/time_digita2.png");
    QPixmap digital3("://new/prefix1/images/time_digita3.png");
    QPixmap digital4("://new/prefix1/images/time_digita4.png");
    QPixmap digital5("://new/prefix1/images/time_digita5.png");
    QPixmap digital6("://new/prefix1/images/time_digita6.png");
    QPixmap digital7("://new/prefix1/images/time_digita7.png");
    QPixmap digital8("://new/prefix1/images/time_digita8.png");
    QPixmap digital9("://new/prefix1/images/time_digita9.png");

    painter->drawPixmap(block_length*mine->getcol()/2-block_length/2,title_begin+title_height/3,again2,0,0,block_length,block_height);
    if(paint_flag=="boom")
        painter->drawPixmap(block_length*mine->getcol()/2-block_length/2,title_begin+title_height/3,lose,0,0,block_length,block_height);
    //显示旗帜数目
    switch  (flag_number/100)
    {
    case 0:painter->drawPixmap(0,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(0,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(0,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(0,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(0,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(0,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(0,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(0,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(0,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(0,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  ((flag_number/10)%10)
    {
    case 0:painter->drawPixmap(35,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(35,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(35,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(35,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(35,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(35,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(35,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(35,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(35,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(35,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  (flag_number%10)
    {
    case 0:painter->drawPixmap(70,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(70,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(70,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(70,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(70,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(70,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(70,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(70,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(70,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(70,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    //显示时间
    int timenum = mine->gettimenum();
    length=mine->getcol()*35;
    switch  (timenum/100)
    {
    case 0:painter->drawPixmap(length-105,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(length-105,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(length-105,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(length-105,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(length-105,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(length-105,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(length-105,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(length-105,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(length-105,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(length-105,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  ((timenum/10)%10)
    {
    case 0:painter->drawPixmap(length-70,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(length-70,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(length-70,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(length-70,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(length-70,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(length-70,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(length-70,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(length-70,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(length-70,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(length-70,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
    switch  (timenum%10)
    {
    case 0:painter->drawPixmap(length-35,title_begin+title_height/3,digital0,0,0,block_length,block_height);break;
    case 1:painter->drawPixmap(length-35,title_begin+title_height/3,digital1,0,0,block_length,block_height);break;
    case 2:painter->drawPixmap(length-35,title_begin+title_height/3,digital2,0,0,block_length,block_height);break;
    case 3:painter->drawPixmap(length-35,title_begin+title_height/3,digital3,0,0,block_length,block_height);break;
    case 4:painter->drawPixmap(length-35,title_begin+title_height/3,digital4,0,0,block_length,block_height);break;
    case 5:painter->drawPixmap(length-35,title_begin+title_height/3,digital5,0,0,block_length,block_height);break;
    case 6:painter->drawPixmap(length-35,title_begin+title_height/3,digital6,0,0,block_length,block_height);break;
    case 7:painter->drawPixmap(length-35,title_begin+title_height/3,digital7,0,0,block_length,block_height);break;
    case 8:painter->drawPixmap(length-35,title_begin+title_height/3,digital8,0,0,block_length,block_height);break;
    case 9:painter->drawPixmap(length-35,title_begin+title_height/3,digital9,0,0,block_length,block_height);break;
    }
}

int win(){

    int sz= 0,boompc=0;
    for(int row = 0; row <mine->getrow();row++)
        for(int col =0;col<mine->getcol();col++)
        {
            if((mine->p[row][col]>=100)&&(mine->p[row][col]<110))
                sz++;
            if(mine->p[row][col]==149)boompc++;
        }
    if(sz>=((mine->getcol()*mine->getrow())-mine->getboomnum())||boompc==boom_number)
        return 1;
    else
        return 0;
}
void Winmine::dialogshow(){
    w3.show();
}
void Winmine::second()
{
    mine->addtimeNum();
}

void paintboom(QPainter *painter)
{
    QPixmap initial("://new/prefix1/images/initial.png");
    QPixmap explode("://new/prefix1/images/explode.png");
    QPixmap flag("://new/prefix1/images/flag.png");
    QPixmap blank("://new/prefix1/images/blank.png");
    QPixmap question("://new/prefix1/images/question.png");
    QPixmap blk_1("://new/prefix1/images/1.png");
    QPixmap blk_2("://new/prefix1/images/2.png");
    QPixmap blk_3("://new/prefix1/images/3.png");
    QPixmap blk_4("://new/prefix1/images/4.png");
    QPixmap blk_5("://new/prefix1/images/5.png");
    QPixmap blk_6("://new/prefix1/images/6.png");
    QPixmap blk_7("://new/prefix1/images/7.png");
    QPixmap blk_8("://new/prefix1/images/8.png");

    for(int i=0;i<mine->getrow();i++)
        for(int j=0;j<mine->getcol();j++){
            if(mine->p[i][j]<10||mine->p[i][j]==99)
                painter->drawPixmap(j*block_length,i*block_height+title_height,initial,0,0,block_length,block_height);

            else if((mine->p[i][j]>=50&&mine->p[i][j]<60)||mine->p[i][j]==149)//插旗
                painter->drawPixmap(j*block_length,i*block_height+title_height,flag,0,0,block_length,block_height);

            else if((mine->p[i][j]>=200&&mine->p[i][j]<210)||mine->p[i][j]==299)
                painter->drawPixmap(j*block_length,i*block_height+title_height,question,0,0,block_length,block_height);

            else if(mine->p[i][j]>99&&mine->p[i][j]<110){
                int num=mine->p[i][j]-100;
                switch  (num){
                case 0:painter->drawPixmap(j*block_length,i*block_height+title_height,blank,0,0,block_length,block_height);break;
                case 1:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_1,0,0,block_length,block_height);break;
                case 2:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_2,0,0,block_length,block_height);break;
                case 3:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_3,0,0,block_length,block_height);break;
                case 4:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_4,0,0,block_length,block_height);break;
                case 5:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_5,0,0,block_length,block_height);break;
                case 6:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_6,0,0,block_length,block_height);break;
                case 7:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_7,0,0,block_length,block_height);break;
                case 8:painter->drawPixmap(j*block_length,i*block_height+title_height,blk_8,0,0,block_length,block_height);break;
                }
            }
            else if(mine->p[i][j]==199)
                painter->drawPixmap(j*block_length,i*block_height+title_height,explode,0,0,block_length,block_height);
        }
}

void Winmine::restart_clicked()
{
    mine=new block(mine->getrow(),mine->getcol(),mine->getboomnum());
    boom_number=mine->getboomnum();
    flag_number=boom_number;
}

void Winmine::easy()
{
    mine=new block(9,9,10);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
}

void Winmine::middle()
{
    mine=new block(18,18,36);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
    this->move(631,63);
}

void Winmine::hard()
{
    mine=new block(24,54,144);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    update();
    this->move(0,0);
}

void Winmine::window_show()
{
    w.setFixedSize(400,400);
    w.move(block_length*mine->getcol()/2-90,block_height+title_height/2-50);
    w.setWindowTitle("自定义");
    w.show();
}

void Winmine::list_show()
{
    w2.setWindowTitle("排行榜");
    w2.move(0,0);
    w2.show();
}

void Winmine::setcustomize(int row,int col,int boomnum)
{
    mine=new block(row,col,boomnum);
    boom_number=mine->getboomnum();
    flag_number=boom_number;
    paint_flag="none";
    end=0;
    w.hide();
    update();
}

Winmine::~Winmine()
{
    delete ui;
}
