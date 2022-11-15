#include "mainWindow.h"
#include <QPainter>
#include <QKeyEvent>
//#include <QDir>
#include <fstream>
#include <QColor>

QColor red(255,0,0),gray(0,0,0),wihte(255,255,255),blue(0,0,244);

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // set window title
    setWindowTitle(tr("Find Way To Exit!!!"));
    // @TODO write your codes here
    setFixedSize(1000,1000);
    //menubar输入文件
    /*loadFile = new QAction(tr("Load File"), this);
    connect(loadFile, &QAction::triggered, this, &MainWindow::load_data);
    QMenuBar menubar(this);
    QMenu *load = menubar.addMenu(tr("load file"));
    load->addAction(loadFile);*/
    load->setGeometry(0,0,250,70);//设置按钮的位置和大小
    load->setFont(QFont("宋体",18));//设置字体和大小
    connect(load,&QPushButton::pressed,this,&MainWindow::load_data);
    load->show();
}

void MainWindow::outpath(){
    int num=battle_field.path_x.size()+1;
    cout<<num<<endl;
    for(int i=0;i<num-1;i++){
        cout<<battle_field.path_x[i]<<" "<<battle_field.path_y[i]<<endl;
    }
    cout<<N-1<<" "<<battle_field.path_y[num-1]<<endl;
}
void MainWindow::load_data(){
    // @TODO write your codes here
    int num,x1,x2,y1,y2;
    ifstream datafile( "C:\\Users/YUXU/Desktop/mid-exam-SEP/exam/SEP/testcase1.txt" );
    if(!datafile.is_open ())
        cout << "Open file failure" << endl;
    else
    {
        datafile >> num;
        for(int i=0;i<num;i++){
            datafile>>x1>>y1>>x2>>y2;
            for(int j=std::min(x1,x2);j<=std::max(x1,x2);j++){
                for(int k=std::min(y1,y2);k<=std::max(y1,y2);k++)
                    battle_field.defield[j][k]=true;
            }
        }
    }
load->hide();
update();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // @TODO write your codes here
    if(event->key()==Qt::Key_P){
        if(battle_field.findPath()){
            isfind=true;
            outpath();
            update();
        }
        else{}
    }
}


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //painter抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    //设置painter的坐标系转换
    painter.scale(1,1);
    // @TODO write your codes here
    painter.setPen(gray);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(battle_field.defield[i][j]==true) painter.setBrush(red);
            else painter.setBrush(wihte);
            QPoint Point[4]{
                QPoint(i*10,j*10),
                QPoint(i*10+10,j*10),
                QPoint(i*10+10,j*10+10),
                QPoint(i*10,j*10+10),
            };
            painter.drawConvexPolygon(Point,4);
        }
    }
    if(isfind){
   int x1,y1,x2,y2;
    while(battle_field.path_x.size()>1){
        painter.setPen(gray);
        painter.setBrush(blue);
        x1=battle_field.path_x.back();
        y1=battle_field.path_y.back();
        battle_field.path_x.pop_back();
        battle_field.path_y.pop_back();
        x2=battle_field.path_x.back();
        y2=battle_field.path_y.back();
        if(y1==y2){
        QPoint Point[4]{
            QPoint(x2*10,y2*10),
            QPoint(x1*10+10,y1*10),
            QPoint(x1*10+10,y1*10+10),
            QPoint(x2*10,y2*10+10),
        };
        painter.drawConvexPolygon(Point,4);
        }
        if(x1==x2){
            QPoint Point[4]{
                QPoint(x2*10,std::min(y1,y2)*10),
                QPoint(x2*10+10,std::min(y1,y2)*10),
                QPoint(x2*10+10,std::max(y1,y2)*10+10),
                QPoint(x2*10,std::max(y1,y2)*10+10),
            };
            painter.drawConvexPolygon(Point,4);
        }
    }
    painter.setPen(gray);
    painter.setBrush(blue);
    x1=battle_field.path_x.back();
    y1=battle_field.path_y.back();
    QPoint Point[4]{
        QPoint(x1*10,y1*10),
        QPoint((N-1)*10+10,y1*10),
        QPoint((N-1)*10+10,y1*10+10),
        QPoint(x1*10,y1*10+10),
    };
    painter.drawConvexPolygon(Point,4);
    battle_field.path_x.pop_back();
    battle_field.path_y.pop_back();
    }
}

