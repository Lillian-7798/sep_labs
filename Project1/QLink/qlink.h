#ifndef QLINK_H
#define QLINK_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class QLink; }
QT_END_NAMESPACE

extern int boxTypeNumber;
extern int typeBoxNumber;
/***************************Box*******************************************/

//将所有箱子集合成一个类，用5*5的小方块表示箱子，通过改变箱子颜色区别不同类型的箱子，通过描边区别箱子是否处于激活状态
class Box{
public:
    //记录箱子颜色和描边颜色
    QColor boxColor;
    //记录箱子左上角在坐标轴的位置,箱子大小为5*5
    int x,y;
    //记录箱子激活状态
    bool isEnable;
    //记录hint状态下的高亮状态
    bool isHint;
    //记录箱子是否已被消除
    bool isDelete;

    Box();
    ~Box(){};
    //设置箱子RGB颜色
    void setBoxcolor(int r,int g,int b,int a=255);
private:

};

/**************************Man********************************************/

//将小人做成另一个类，用5*5的小圆圈表示
class Man{
public:

    Man(){};
    ~Man(){};
    void setMan(int r,int g,int b,int t,int p);
private:
    friend class QLink;
    //用x,y记录小人的左上角的位置
    int x,y;
    //记录小人是否已经激活了一个箱子
    bool haveEnabled;
    //记录已经激活的箱子
    int enabledBox;
    //记录小人颜色
    QColor manColor;
    //记录该玩家的分数
    int point;

};

/**********************Prop*************************************************/

class Prop{
public:
    Prop();
    ~Prop();
    void setpenColor(QColor);
    friend class QLink;
private:
    //记录道具是否已经被消除
    bool isDelete;
    //道具的颜色和描边颜色
    QColor penColor;
    //道具的位置
    int x,y;

};

/**************************QLink********************************************/

class QLink : public QWidget
{
    Q_OBJECT

public:
    QLink(QWidget *parent = nullptr);
    ~QLink();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;//用上下左右键控制小人0移动，用WASD控制小人2移动

private:
    Ui::QLink *ui;

    //记录箱子
    Box *box=new Box[boxTypeNumber*typeBoxNumber];
    //至多两个小人
    Man man[2];
    //建立数组，用0表示此处没有箱子，用1表示此处有箱子，主要用于连连看消除逻辑判断
    int boxDistribution[30][30]={0};
    //判断当前是单人模式还是双人模式
    bool isDoubleMode;
    //判断游戏是否结束
    bool isGameOver = false;
    //判断游戏是否暂停
    bool isStop = false;
    //判断是否开始游戏
    bool isStart = false;
    //记录消除连线拐点
    int tmpX,tmpY;
    //记录每一次消除的两个箱子
    int box1,box2;
    //道具
    Prop prop[6];
    QTimer *hintTimer;
    //倒计时
    int allTime=20;
    QTimer *countdownTimer;
    //游戏存档
    QPushButton *saveButton = new QPushButton("Save",this);
    QPushButton *loadButton = new QPushButton("Load",this);
    QSettings *setting = new QSettings("MySoft","QLink");
    //开始菜单按钮
    QPushButton *soloModeButton = new QPushButton("Solo Mode",this);
    QPushButton *doubleModeButton = new QPushButton("Double Mode",this);
    QPushButton *continueButton = new QPushButton("Continue Game",this);
    QPushButton *exitButton = new QPushButton("Exit",this);

    //生成一定数量的成对的箱子
    void setBox();
    //更新现有箱子的位置
    void boxUpdate();
    //设置小人的初始参数
    void setMan();
    //设置初始道具
    void setProp();
    //判定逻辑
    bool noTurnLinked(int x1,int y1, int x2,int y2);//一条直线上的两点间是否有障碍
    bool desicionLogic(int boxOne,int boxTwo);
    bool deDesicionLogic(int boxOne,int boxTwo);
    //消除箱子
    void deleteBox(int boxOne,int boxTwo);
    //倒计时
    void countTime();
    //暂停游戏
    void stopGame();
    //触发道具及道具功能
    void useProp(int propNumber);
    void addSecond();
    void shuffle();
    void hint();
    void findHint();
    void endHint();
    //存档读档
    void saveGame();
    void continueGame();
    //开始菜单
    void setMenu();
    void soloMode();
    void doubleMode();
    void hideButtons();
};


#endif // QLINK_H
