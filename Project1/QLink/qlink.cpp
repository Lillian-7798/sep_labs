#include "qlink.h"
#include "ui_qlink.h"
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <cmath>
#include <QSettings>
#include <QPushButton>

int windowHeight=1200;
int windowWidth=1200;
int boxTypeNumber = 5;
int typeBoxNumber = 16;
QColor black(0,0,0),red(255,0,0),addsecondColor(200,0,200),shuffleColor(0,200,0),hintColor(0,200,200),enabledColor(220,200,30);

/************************************************************************/

QLink::QLink(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QLink)
{
    ui->setupUi(this);
    // 设置窗口的标题
    setWindowTitle(tr("QLink"));
    //游戏开始
    setMenu();
    // 设置 widget 大小
    resize(windowWidth, windowHeight);
    //倒计时,当时间过去一秒，painter绘制的文本框中数字-1
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, QOverload<>::of(&QLink::countTime));
    connect(countdownTimer, &QTimer::timeout, this, QOverload<>::of(&QLink::update));
    //hint道具的计时器
    hintTimer = new QTimer(this);
    connect(hintTimer,&QTimer::timeout,this,&QLink::endHint);
    connect(hintTimer,&QTimer::timeout,hintTimer,&QTimer::stop);
    //生成暂停后按钮界面
    saveButton->setGeometry(475,455,250,70);//设置按钮的位置和大小
    saveButton->setFont(QFont("宋体",18));//设置字体和大小
    saveButton->hide();
    connect(saveButton,&QPushButton::pressed,this,&QLink::saveGame);
    saveButton->setDisabled(true);
    loadButton->setGeometry(475,605,250,70);//设置按钮的位置和大小
    loadButton->setFont(QFont("宋体",18));//设置字体和大小
    loadButton->hide();
    connect(loadButton,&QPushButton::pressed,this,&QLink::continueGame);
    saveButton->setDisabled(true);
    //画出画面
    update();
}
QLink::~QLink()
{
    delete ui;
}
void QLink::soloMode(){
    isStart = true;
    isDoubleMode = false;
    //游戏中一些标记
    box1=box2=-1;
    //生成三种不同类型的箱子
    setBox();
    //生成小人
    setMan();
    //生成道具
    setProp();
    //开始倒计时
    countdownTimer->start(1000);
    update();
}
void QLink::doubleMode(){
    isStart = true;
    isDoubleMode = true;
    //游戏中一些标记
    box1=box2=-1;
    //生成三种不同类型的箱子
    setBox();
    //生成小人
    setMan();
    //生成道具
    setProp();
    //开始倒计时
    countdownTimer->start(1000);
    update();
}
void QLink::saveGame(){
    //储存所有箱子的信息
    for(int i=1;i<=boxTypeNumber*typeBoxNumber;i++){
        setting->setValue("box"+QString(i)+".x",box[i-1].x);
        setting->setValue("box"+QString(i)+".y",box[i-1].y);
        setting->setValue("box"+QString(i)+"color",box[i-1].boxColor);
        setting->setValue("box"+QString(i)+"isEnabled",box[i-1].isEnable);
        setting->setValue("box"+QString(i)+"isDelete",box[i-1].isDelete);
        setting->setValue("box"+QString(i)+"isHint",box[i-1].isHint);
    }
    //储存玩家信息
    for(int i=1;i<=2;i++){
        setting->setValue("man"+QString(i)+".x",man[i-1].x);
        setting->setValue("man"+QString(i)+".y",man[i-1].y);
        setting->setValue("man"+QString(i)+".haveEnabled",man[i-1].haveEnabled);
        setting->setValue("man"+QString(i)+".enabledBox",man[i-1].enabledBox);
        setting->setValue("man"+QString(i)+".point",man[i-1].point);
    }
    //储存道具信息
    for(int i=1;i<=6;i++){
        setting->setValue("prop"+QString(i)+".x",prop[i-1].x);
        setting->setValue("prop"+QString(i)+".y",prop[i-1].y);
        setting->setValue("prop"+QString(i)+".isDelete",prop[i-1].isDelete);
        setting->setValue("prop"+QString(i)+".penColor",prop[i-1].penColor);
    }
    //储存QLink里面的一些窗口信息和判断信息
    setting->setValue("windowHeight",height());
    setting->setValue("windowWidth",width());
    setting->setValue("isDoubleMode",isDoubleMode);
    setting->setValue("isGameover",isGameOver);
    setting->setValue("isStop",isStop);
    setting->setValue("isStart",isStart);
    setting->setValue("tmpX",tmpX);
    setting->setValue("tmpY",tmpY);
    setting->setValue("box1",box1);
    setting->setValue("box2",box2);
    setting->setValue("allTime",allTime);
    setting->setValue("hintTimer",hintTimer->remainingTime());
}
void QLink::continueGame(){
    //读取箱子信息
    for(int i=1;i<=boxTypeNumber*typeBoxNumber;i++){
        box[i-1].x=setting->value("box"+QString(i)+".x").toInt();
        box[i-1].y=setting->value("box"+QString(i)+".y").toInt();
        box[i-1].boxColor=setting->value("box"+QString(i)+"color").value<QColor>();
        box[i-1].isEnable=setting->value("box"+QString(i)+"isEnabled").toBool();
        box[i-1].isDelete=setting->value("box"+QString(i)+"isDelete").toBool();
        box[i-1].isHint=setting->value("box"+QString(i)+"isHint").toBool();
        boxDistribution[box[i-1].x/5][box[i-1].y/5]=1;
    }
    //读取玩家信息
    for(int i=1;i<=2;i++){
        man[i-1].x=setting->value("man"+QString(i)+".x").toInt();
        man[i-1].y=setting->value("man"+QString(i)+".y").toInt();
        man[i-1].haveEnabled=setting->value("man"+QString(i)+".haveEnabled").toBool();
        man[i-1].enabledBox=setting->value("man"+QString(i)+".enabledBox").toInt();
        man[i-1].point=setting->value("man"+QString(i)+".point").toInt();
    }
    man[0].manColor.setRgb(255,0,0);
    man[1].manColor.setRgb(0,255,0);
    //读取道具信息
    for(int i=1;i<=6;i++){
        prop[i-1].x=setting->value("prop"+QString(i)+".x").toInt();
        prop[i-1].y=setting->value("prop"+QString(i)+".y").toInt();
        prop[i-1].isDelete=setting->value("prop"+QString(i)+".isDelete").toBool();
        prop[i-1].penColor=setting->value("prop"+QString(i)+".penColor").value<QColor>();
    }
    //读取游戏信息
    windowHeight=setting->value("windowHeight").toInt();
    windowWidth=setting->value("windowWidth").toInt();
    isDoubleMode=setting->value("isDoubleMode").toBool();
    isGameOver=setting->value("isGameover").toBool();
    isStart=setting->value("isStart").toBool();
    tmpX=setting->value("tmpX").toInt();
    tmpY=setting->value("tmpY").toInt();
    box1=setting->value("box1").toInt();
    box2=setting->value("box2").toInt();
    allTime=setting->value("allTime").toInt();
    isStop=false;
    //禁止按钮获取焦点并使其消失
    loadButton->setDisabled(true);
    saveButton->setDisabled(true);
    loadButton->hide();
    saveButton->hide();
    //开始倒计时
    countdownTimer->start(1000);
    //如果hint，继续hint
    if(setting->value("hintTimer").toInt()>0)
        hintTimer->start(setting->value("hintTimer").toInt());

    update();
}
void QLink::hideButtons(){
    soloModeButton->hide();
    doubleModeButton->hide();
    continueButton->hide();
    exitButton->hide();
    soloModeButton->setDisabled(true);
    doubleModeButton->setDisabled(true);
    continueButton->setDisabled(true);
    exitButton->setDisabled(true);
}
void QLink::setMenu(){
    //单人模式按钮
    soloModeButton->setGeometry(475,235,250,70);//设置按钮的位置和大小
    soloModeButton->setFont(QFont("宋体",18));//设置字体和大小
    connect(soloModeButton,&QPushButton::pressed,this,&QLink::soloMode);//设置点击按钮后发生的事件
    connect(soloModeButton,&QPushButton::pressed,this,&QLink::hideButtons);
    //双人模式按钮
    doubleModeButton->setGeometry(475,385,250,70);
    doubleModeButton->setFont(QFont("宋体",18));
    connect(doubleModeButton,&QPushButton::pressed,this,&QLink::doubleMode);
    connect(doubleModeButton,&QPushButton::pressed,this,&QLink::hideButtons);
    //读档继续游戏按钮
    continueButton->setGeometry(475,535,250,70);
    continueButton->setFont(QFont("宋体",18));
    connect(continueButton,&QPushButton::pressed,this,&QLink::continueGame);
    connect(continueButton,&QPushButton::pressed,this,&QLink::hideButtons);
    //退出游戏
    exitButton->setGeometry(475,685,250,70);
    exitButton->setFont(QFont("宋体",18));
    connect(exitButton,&QPushButton::pressed,this,&QLink::close);
}

void QLink::setBox(){
    srand(time(NULL));
    int r,g,b,x,y;
    for(int i=0;i<boxTypeNumber;i++){
        //设置一类箱子的颜色
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
        for(int j=0;j<typeBoxNumber;j++){
            do{
                x=rand()%26+1;
                y=rand()%26+3;
            }while(boxDistribution[x][y]==1);//排除几个箱子重叠的情况
            box[i*typeBoxNumber+j].x=x*5;
            box[i*typeBoxNumber+j].y=y*5;
            box[i*typeBoxNumber+j].setBoxcolor(r,g,b);
            boxDistribution[x][y]=1;
        }
    }
}
void QLink::setMan(){
    srand(time(NULL));
    man[0].setMan(255,0,0,rand()%30*5,rand()%30*5);
    man[1].setMan(0,255,0,rand()%30*5,rand()%30*5);
}
void QLink::setProp(){
    srand(time(NULL));
    for(int i=0;i<6;i++){
        prop[i].x=rand()%26*5+5;
        prop[i].y=rand()%26*5+15;
    }
    //设置不同道具的描边颜色
    prop[0].setpenColor(addsecondColor);
    prop[1].setpenColor(addsecondColor);
    prop[2].setpenColor(shuffleColor);
    prop[3].setpenColor(shuffleColor);
    prop[4].setpenColor(hintColor);
    prop[5].setpenColor(hintColor);
}
void QLink::stopGame(){
    isStop=true;
    saveButton->show();
    saveButton->setDisabled(false);
    loadButton->show();
    loadButton->setDisabled(false);

}
bool QLink::noTurnLinked(int x1, int y1, int x2, int y2){
    if((x1==x2&&std::max(y1,y2)==std::min(y1,y2)+1)||(y1==y2&&std::max(x1,x2)==std::min(x1,x2)+1))
        return true;
    if(x1==x2){
        for(int i=std::min(y1,y2)+1;i<std::max(y1,y2);i++)
            if(boxDistribution[x1][i]!=0) return false;
        return true;
    }
    else{
        for(int i=std::min(x1,x2)+1;i<std::max(x1,x2);i++)
            if(boxDistribution[i][y1]!=0) return false;
        return true;
    }
}
bool QLink::deDesicionLogic(int boxOne,int boxTwo){
    int x1=box[boxOne].x/5;
    int y1=box[boxOne].y/5;
    int x2=box[boxTwo].x/5;
    int y2=box[boxTwo].y/5;
    //类型不同无法相消
    if(box[boxOne].boxColor!=box[boxTwo].boxColor) return false;
    //同一个箱子无法相消
    if(boxOne==boxTwo) return false;
    //没有拐弯
    if((x1==x2||y1==y2)&&noTurnLinked(x1,y1,x2,y2))
        return true;
    //一个拐弯
    if(noTurnLinked(x1,y1,(x2>x1)?x2+1:x2-1,y1)&&noTurnLinked(x2,(y1>y2)?y1+1:y1-1,x2,y2))
        return true;
    if(noTurnLinked(x1,y1,x1,(y2>y1)?y2+1:y2-1)&&noTurnLinked((x1<x2)?x1-1:x1+1,y2,x2,y2))
        return true;
    //两个拐弯
    for(int i=3;i<30;i++){
        if(noTurnLinked(x1,(y1>i)?i-1:i+1,x1,y1)&&noTurnLinked((x1>x2)?x2-1:x1-1,i,(x1>x2)?x1+1:x2+1,i)&&noTurnLinked(x2,(y2>i)?i-1:i+1,x2,y2))
            return true;
    }
    for(int i=0;i<28;i++){
        if(noTurnLinked((x1>i)?i-1:i+1,y1,x1,y1)&&noTurnLinked(i,(y1>y2)?y2-1:y1-1,i,(y1>y2)?y1+1:y2+1)&&noTurnLinked((x2>i)?i-1:i+1,y2,x2,y2))
            return true;
    }
    return false;
}

bool QLink::desicionLogic(int boxOne, int boxTwo){
    int x1=box[boxOne].x/5;
    int y1=box[boxOne].y/5;
    int x2=box[boxTwo].x/5;
    int y2=box[boxTwo].y/5;

    //类型不同无法相消
    if(box[boxOne].boxColor!=box[boxTwo].boxColor) return false;
    //同一个箱子无法相消
    if(boxOne==boxTwo) return false;
    //没有拐弯
    if((x1==x2||y1==y2)&&noTurnLinked(x1,y1,x2,y2)){
        if(box1==-1){
            tmpX=x1*5;
            tmpY=y1*5;
            box1=boxOne;
            box2=boxTwo;
        }
        return true;
    }
    //一个拐弯
    if(noTurnLinked(x1,y1,(x2>x1)?x2+1:x2-1,y1)&&noTurnLinked(x2,(y1>y2)?y1+1:y1-1,x2,y2)){
        if(box1==-1){
            tmpX=x2*5;
            tmpY=y1*5;
            box1=boxOne;
            box2=boxTwo;
        }
        return true;
    }
    if(noTurnLinked(x1,y1,x1,(y2>y1)?y2+1:y2-1)&&noTurnLinked((x1<x2)?x1-1:x1+1,y2,x2,y2)){
        if(box1==-1){
            tmpX=x1*5;
            tmpY=y2*5;
            box1=boxOne;
            box2=boxTwo;
        }
        return true;
    }
    //两个拐弯
    for(int i=3;i<30;i++){
        if(noTurnLinked(x1,(y1>i)?i-1:i+1,x1,y1)&&noTurnLinked((x1>x2)?x2-1:x1-1,i,(x1>x2)?x1+1:x2+1,i)&&noTurnLinked(x2,(y2>i)?i-1:i+1,x2,y2)){
            if(box1==-1){
                tmpX=x1*5;
                tmpY=i*5;
                box1=boxOne;
                box2=boxTwo;
            }
            return true;
        }
    }
    for(int i=0;i<28;i++){
        if(noTurnLinked((x1>i)?i-1:i+1,y1,x1,y1)&&noTurnLinked(i,(y1>y2)?y2-1:y1-1,i,(y1>y2)?y1+1:y2+1)&&noTurnLinked((x2>i)?i-1:i+1,y2,x2,y2)){
            if(box1==-1){
                tmpX=i*5;
                tmpY=y1*5;
                box1=boxOne;
                box2=boxTwo;
            }
            return true;
        }
    }
    return false;
}
void QLink::countTime(){
    if(!isStop)    allTime--;
}
void QLink::addSecond(){
    allTime+=30;
}
void QLink::shuffle(){
    int x,y;
    memset(boxDistribution,0,sizeof(boxDistribution));//给记录箱子分布的数组全部赋值为0
    for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
        if(box[i].isDelete) continue;
        do{
            x=rand()%26+1;
            y=rand()%26+3;
        }while(boxDistribution[x][y]==1);//排除几个箱子重叠的情况
        box[i].x=x*5;
        box[i].y=y*5;
        boxDistribution[x][y]=1;
    }
}
void QLink::hint(){
    //倒计时10s
    hintTimer->start(10000);
    //寻找可以消除的箱子
    findHint();
}
void QLink::findHint(){
    for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
        if(box[i].isDelete) continue;
        for(int j=i+1;j<boxTypeNumber*typeBoxNumber;j++){
            if(box[j].isDelete) continue;
            if(deDesicionLogic(i,j)) {
                box[i].isHint = true;
                box[j].isHint = true;
                return ;
            }
        }
    }
}
void QLink::endHint(){
    for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
        box[i].isHint = false;
    }
    update();
}
void QLink::useProp(int propNumber){
    prop[propNumber].isDelete = true;
    if(prop[propNumber].penColor==addsecondColor) addSecond();
    if(prop[propNumber].penColor==shuffleColor) shuffle();
    if(prop[propNumber].penColor==hintColor) hint();
    update();
}
void QLink::deleteBox(int boxOne, int boxTwo){
    bool needUpdate = true;
    boxDistribution[box[boxOne].x/5][box[boxOne].y/5]=0;
    boxDistribution[box[boxTwo].x/5][box[boxTwo].y/5]=0;
    box[boxOne].isDelete = true;
    box[boxTwo].isDelete = true;
    for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
        if(box[i].isDelete) continue;
        for(int j=i+1;j<boxTypeNumber*typeBoxNumber;j++){
            if(box[j].isDelete) continue;
            if(deDesicionLogic(i,j)) {
                needUpdate=false;
                break;
            }
        }
        if(!needUpdate) break;
    }
    if(needUpdate) isGameOver=true;
}
void QLink::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_P) {
        stopGame();
    }
    if(!isStop){
        /****先对小人1 进行操作******/
        if(event->key()==Qt::Key_Up){
            man[0].y-=5;
            man[0].y=(man[0].y+150)%150;
            //判断小人1是否可以继续移动以及激活箱子
            for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                //跳过已经消除的箱子
                if(box[i].isDelete) continue;
                //当小人移动后与箱子重叠时，选中箱子
                if(box[i].x==man[0].x&&box[i].y==man[0].y){
                    //小人已经激活了一个箱子
                    if(man[0].haveEnabled){
                        man[0].haveEnabled = false;
                        if(desicionLogic(man[0].enabledBox,i))
                        {
                            deleteBox(man[0].enabledBox,i);
                            if(box[i].isHint||box[man[0].enabledBox].isHint)
                                findHint();
                            man[0].point+=2;
                        }
                        else box[man[0].enabledBox].isEnable=box[i].isEnable = false;
                    }
                    //小人没激活箱子
                    else{
                        man[0].haveEnabled = true;
                        box[i].isEnable = true;
                        man[0].enabledBox = i;
                    }
                    man[0].y+=5;
                    break;
                }
            }
            for(int i=0;i<6;i++){
                if(prop[i].isDelete) continue;
                if(man[0].x==prop[i].x&&man[0].y==prop[i].y) useProp(i);
            }
        }
        if(event->key()==Qt::Key_Left){
            man[0].x-=5;
            man[0].x=(man[0].x+150)%150;
            for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                if(box[i].isDelete) continue;
                if(box[i].x==man[0].x&&box[i].y==man[0].y){
                    if(man[0].haveEnabled){
                        man[0].haveEnabled = false;
                        if(desicionLogic(man[0].enabledBox,i))
                        {
                            deleteBox(man[0].enabledBox,i);
                            if(box[i].isHint||box[man[0].enabledBox].isHint)
                                findHint();
                            man[0].point+=2;
                        }
                        else box[man[0].enabledBox].isEnable=box[i].isEnable = false;
                    }
                    else{
                        man[0].haveEnabled = true;
                        box[i].isEnable = true;
                        man[0].enabledBox = i;
                    }
                    man[0].x+=5;
                    break;
                }
            }
            for(int i=0;i<6;i++){
                if(prop[i].isDelete) continue;
                if(man[0].x==prop[i].x&&man[0].y==prop[i].y) useProp(i);
            }
        }
        if(event->key()==Qt::Key_Down){
            man[0].y+=5;
            man[0].y=(man[0].y+150)%150;
            for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                if(box[i].isDelete) continue;
                if(box[i].x==man[0].x&&box[i].y==man[0].y){
                    if(man[0].haveEnabled){
                        man[0].haveEnabled = false;
                        if(desicionLogic(man[0].enabledBox,i))
                        {
                            deleteBox(man[0].enabledBox,i);
                            if(box[i].isHint||box[man[0].enabledBox].isHint)
                                findHint();
                            man[0].point+=2;
                        }
                        else box[man[0].enabledBox].isEnable=box[i].isEnable = false;
                    }
                    else{
                        man[0].haveEnabled = true;
                        box[i].isEnable = true;
                        man[0].enabledBox = i;
                    }
                    man[0].y-=5;
                    break;
                }
            }
            for(int i=0;i<6;i++){
                if(prop[i].isDelete) continue;
                if(man[0].x==prop[i].x&&man[0].y==prop[i].y) useProp(i);
            }
        }
        if(event->key()==Qt::Key_Right){
            man[0].x+=5;
            man[0].x=(man[0].x+150)%150;
            for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                if(box[i].isDelete) continue;
                if(box[i].x==man[0].x&&box[i].y==man[0].y){
                    if(man[0].haveEnabled){
                        man[0].haveEnabled = false;
                        if(desicionLogic(man[0].enabledBox,i))
                        {
                            deleteBox(man[0].enabledBox,i);
                            if(box[i].isHint||box[man[0].enabledBox].isHint)
                                findHint();
                            man[0].point+=2;
                        }
                        else box[man[0].enabledBox].isEnable=box[i].isEnable = false;
                    }
                    else{
                        man[0].haveEnabled = true;
                        box[i].isEnable = true;
                        man[0].enabledBox = i;
                    }
                    man[0].x-=5;
                    break;
                }
            }
            for(int i=0;i<6;i++){
                if(prop[i].isDelete) continue;
                if(man[0].x==prop[i].x&&man[0].y==prop[i].y) useProp(i);
            }
        }

        /*****对小人2进行操作*****/
        if(isDoubleMode){
            if(event->key()==Qt::Key_W){
                man[1].y-=5;
                man[1].y=(man[1].y+150)%150;
                //判断小人1是否可以继续移动以及激活箱子
                for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                    if(box[i].isDelete) continue;
                    if(box[i].x==man[1].x&&box[i].y==man[1].y){
                        if(man[1].haveEnabled){
                            man[1].haveEnabled = false;
                            if(desicionLogic(man[1].enabledBox,i))
                            {
                                deleteBox(man[1].enabledBox,i);
                                if(box[i].isHint||box[man[1].enabledBox].isHint)
                                    findHint();
                                man[1].point+=2;
                            }
                            else box[man[1].enabledBox].isEnable=box[i].isEnable = false;
                        }
                        else{
                            man[1].haveEnabled = true;
                            box[i].isEnable = true;
                            man[1].enabledBox = i;
                        }
                        man[1].y+=5;
                        break;
                    }
                }
                for(int i=0;i<6;i++){
                    if(prop[i].isDelete) continue;
                    if(man[1].x==prop[i].x&&man[1].y==prop[i].y) useProp(i);
                }
            }
            if(event->key()==Qt::Key_A){
                man[1].x-=5;
                man[1].x=(man[1].x+150)%150;
                for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                    if(box[i].isDelete) continue;
                    if(box[i].x==man[1].x&&box[i].y==man[1].y){
                        if(man[1].haveEnabled){
                            man[1].haveEnabled = false;
                            if(desicionLogic(man[1].enabledBox,i))
                            {
                                deleteBox(man[1].enabledBox,i);
                                if(box[i].isHint||box[man[1].enabledBox].isHint)
                                    findHint();
                                man[1].point+=2;
                            }
                            else box[man[1].enabledBox].isEnable=box[i].isEnable = false;
                        }
                        else{
                            man[1].haveEnabled = true;
                            box[i].isEnable = true;
                            man[1].enabledBox = i;
                        }
                        man[1].x+=5;
                        break;
                    }
                }
                for(int i=0;i<6;i++){
                    if(prop[i].isDelete) continue;
                    if(man[1].x==prop[i].x&&man[1].y==prop[i].y) useProp(i);
                }
            }
            if(event->key()==Qt::Key_S){
                man[1].y+=5;
                man[1].y=(man[1].y+150)%150;
                for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                    if(box[i].isDelete) continue;
                    if(box[i].x==man[1].x&&box[i].y==man[1].y){
                        if(man[1].haveEnabled){
                            man[1].haveEnabled = false;
                            if(desicionLogic(man[1].enabledBox,i))
                            {
                                deleteBox(man[1].enabledBox,i);
                                if(box[i].isHint||box[man[1].enabledBox].isHint)
                                    findHint();
                                man[1].point+=2;
                            }
                            else box[man[1].enabledBox].isEnable=box[i].isEnable = false;
                        }
                        else{
                            man[1].haveEnabled = true;
                            box[i].isEnable = true;
                            man[1].enabledBox = i;
                        }
                        man[1].y-=5;
                        break;
                    }
                }
                for(int i=0;i<6;i++){
                    if(prop[i].isDelete) continue;
                    if(man[1].x==prop[i].x&&man[1].y==prop[i].y) useProp(i);
                }
            }
            if(event->key()==Qt::Key_D){
                man[1].x+=5;
                man[1].x=(man[1].x+150)%150;
                for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                    if(box[i].isDelete) continue;
                    if(box[i].x==man[1].x&&box[i].y==man[1].y){
                        if(man[1].haveEnabled){
                            man[1].haveEnabled = false;
                            if(desicionLogic(man[1].enabledBox,i))
                            {
                                deleteBox(man[1].enabledBox,i);
                                if(box[i].isHint||box[man[1].enabledBox].isHint)
                                    findHint();
                                man[1].point+=2;
                            }
                            else box[man[1].enabledBox].isEnable=box[i].isEnable = false;
                        }
                        else{
                            man[1].haveEnabled = true;
                            box[i].isEnable = true;
                            man[1].enabledBox = i;
                        }
                        man[1].x-=5;
                        break;
                    }
                }
                for(int i=0;i<6;i++){
                    if(prop[i].isDelete) continue;
                    if(man[1].x==prop[i].x&&man[1].y==prop[i].y) useProp(i);
                }
            }
        }
        update();
    }
}
void QLink::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    //painter抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    //设置painter的坐标系转换
    int side = qMin(windowWidth, windowHeight);
    painter.scale(side / 150.0, side / 150.0);

    if(isStart){
        if(!isGameOver){
            //画道具
            painter.setBrush(black);
            for(int i=0;i<6;i++){
                if(prop[i].isDelete) continue;
                painter.setPen(prop[i].penColor);
                painter.drawEllipse(prop[i].x,prop[i].y,5,5);
            }
            //开始画箱子
            if(box1!=-1&&box2!=-1){
                if(tmpX!=box[box1].x||tmpY!=box[box1].y)
                    painter.drawLine(box[box1].x+2,box[box1].y+2,tmpX+2,tmpY+2);
                if(tmpX==box[box1].x){
                    if(tmpX!=box[box2].x)
                        painter.drawLine(tmpX+2,tmpY+2,box[box2].x+2,tmpY+2);
                    if(tmpY!=box[box2].y)
                        painter.drawLine(box[box2].x+2,tmpY+2,box[box2].x+2,box[box2].y+2);
                }
                else{
                    if(tmpY!=box[box2].y)
                        painter.drawLine(tmpX+2,tmpY+2,tmpX+2,box[box2].y+2);
                    if(tmpX!=box[box2].x)
                        painter.drawLine(tmpX+2,box[box2].y+2,box[box2].x+2,box[box2].y+2);
                }
                box1=box2=-1;
            }
            for(int i=0;i<boxTypeNumber*typeBoxNumber;i++){
                if(box[i].isDelete) continue;
                //设置画笔颜色
                painter.setBrush(box[i].boxColor);
                //判断是否激活，激活状态下箱子用黄边表示
                if(box[i].isEnable) painter.setPen(enabledColor);
                else{
                    //判断是否是hint状态下的高亮状态
                    if(box[i].isHint) painter.setPen(hintColor);
                    else painter.setPen(Qt::NoPen);
                }
                //开始画箱子，箱子大小为5*5
                QPoint boxPoint[4]{
                    QPoint(box[i].x,box[i].y),
                            QPoint(box[i].x+5,box[i].y),
                            QPoint(box[i].x+5,box[i].y+5),
                            QPoint(box[i].x,box[i].y+5),
                };
                painter.drawConvexPolygon(boxPoint,4);
            }

            //开始画小人1
            painter.setBrush(man[0].manColor);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(man[0].x,man[0].y,5,5);
            painter.setPen(black);
            painter.drawText(0,15,"P1:"+QString::number(man[0].point));
            if(isDoubleMode){
                painter.setBrush(man[1].manColor);
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(man[1].x,man[1].y,5,5);
                painter.setPen(black);
                painter.drawText(40+floor(log10(man[0].point+1))*10,15,"P2:"+QString::number(man[1].point));
            }//画小人2
            //倒计时
            painter.setPen(red);
            if(allTime>0) painter.drawText(140-floor(log10(allTime))*10,15,QString::number(allTime));//倒计时为0之前都会显示
            else isGameOver = true;
        }
        else {//游戏结束界面
            painter.setPen(red);
            painter.drawText(30,55,"Game Over");
            painter.drawText(30,75,"P1:"+QString::number(man[0].point));
            if(isDoubleMode){
                painter.drawText(30,92,"P2:"+QString::number(man[1].point));
                //判断输赢
                if(man[0].point>man[1].point)
                    painter.drawText(30,109,"P1 Wins!");
                if(man[0].point<man[1].point)
                    painter.drawText(30,109,"P2 Wins!");
                if(man[0].point==man[1].point)
                    painter.drawText(30,109,"Dogfall");
            }
        }
    }
}

/************************************************************************/

Box::Box(){
    isEnable=false;
    isDelete=false;
    isHint = false;
    //x=y=10;
}

void Box::setBoxcolor(int r,int g,int b,int a){
    boxColor.setRgb(r,g,b,a);
}

/************************************************************************/

void Man::setMan(int r,int g,int b,int t,int p){
    //设置小人的初始位置
    x=t;
    y=p;
    point=0;
    //设置小人初始颜色
    manColor.setRgb(r,g,b);
    haveEnabled=false;
}

/*************************************************************************/

Prop::Prop(){
    isDelete = false;
}
Prop::~Prop(){}
void Prop::setpenColor(QColor color){
    penColor.setRgb(color.red(),color.green(),color.blue());
}
