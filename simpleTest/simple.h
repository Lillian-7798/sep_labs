#ifndef SIMPLE_H
#define SIMPLE_H

class Simple
{
public:
    Simple();
    //一条直线上的两点间是否有障碍
    bool noTurnLinked(int x1,int y1, int x2,int y2);
    //判定逻辑
    bool desicionLogic(int x1,int y1,int x2,int y2);
    int boxDistribution[10][7]={0};
    /*boxDistribution分布
        X0 X1 X2 X3 X4 X5 X6 X7 X8 X9|
      Y0                             |
      Y1             2  2  2     1   |
      Y2    1     1        2  2  2   |
      Y3    1  2                     |
      Y4                         2   |
      Y5             2     2  1  2   |
      Y6                             |
      */


};

#endif // SIMPLE_H
