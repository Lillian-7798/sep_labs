#include "sokoban.h"
#include "ui_sokoban.h"
#include <QPainter>
#include <QKeyEvent>
#include <QPoint>
#include <QEvent>

sokoban::sokoban(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sokoban)
{
    ui->setupUi(this);
    // 设置窗口的标题
    setWindowTitle(tr("Sokoban"));
    // 设置 widget 大小
    resize(500, 500);
    //设置人物和箱子的初始位置
    //x[0]=y[0]=y[1]=0;x[1]=5;
    x[0]=y[0]=y[1]=5;x[1]=10;
    //设置初始画面
    paintEvent(NULL);
}
sokoban::~sokoban()
{
    delete ui;
}
void sokoban::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//线条平滑
    painter.setPen(Qt::NoPen);//设置边框颜色
    painter.scale(width()/100,height()/100);//设置笔触大小

    //设置人物颜色和箱子颜色
    QColor mancolor(127, 0, 127);
    QColor boxcolor(82, 139, 139);

    //画一个圆代表人物
    painter.setBrush(mancolor);
    painter.drawEllipse(x[0],y[0],5,5);

    //画一个方框代表箱子
    painter.setBrush(boxcolor);
    QPoint box[4]{
        QPoint(x[1],y[1]),
        QPoint(x[1]+5,y[1]),
        QPoint(x[1]+5,y[1]+5),
        QPoint(x[1],y[1]+5),
    };
    painter.drawConvexPolygon(box,4);
}
void sokoban::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_W){
        y[0]-=5;
        y[0]=(y[0]+100)%100;
        if(y[0]==y[1]&&x[0]==x[1]) y[1]-=5;
        y[1]=(y[1]+100)%100;
    }
    if(event->key()==Qt::Key_A){
        x[0]-=5;
        x[0]=(x[0]+100)%100;
        if(y[0]==y[1]&&x[0]==x[1]) x[1]-=5;
        x[1]=(x[1]+100)%100;
    }
    if(event->key()==Qt::Key_S){
        y[0]+=5;
        y[0]=(y[0]+100)%100;
        if(y[0]==y[1]&&x[0]==x[1]) y[1]+=5;
        y[1]=(y[1]+100)%100;
    }
    if(event->key()==Qt::Key_D){
        x[0]+=5;
        x[0]=(x[0]+100)%100;
        if(y[0]==y[1]&&x[0]==x[1]) x[1]+=5;
        x[1]=(x[1]+100)%100;
    }
    update();
}
