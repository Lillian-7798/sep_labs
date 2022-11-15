#ifndef BOARD_H
#define BOARD_H
#endif // BOARD_H

// Don't change, width = N, height = N
#define N 100
//#include <QPoint>
#include<iostream>
#include <vector>
using namespace std;
class BattleField
{
public:
    int field[N][N];
    bool defield[N][N]={0};
    // find path in this function
    //QVector <QPoint> &path;
    vector <int> path_x;
    vector <int> path_y;
    bool findpath(int x,int y);
    bool findPath();
    int straight_x(int x1,int y1);
    int straight_y_down(int x1,int y1);
    int straight_y_up(int x1,int y1);
};
