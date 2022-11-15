#include "battlefield.h"

// find path in this function
// you can add other functions to solve this problem
// you should print the path if you can find one
// if you can find one, return true, otherwise false
int BattleField::straight_x(int x1,int y1){
    for(int i=x1;i<N;i++){
        if(defield[i][y1]) return i-1;
    }
    return N-1;
}
int BattleField::straight_y_down(int x1,int y1){
    for(int i=y1;i<N;i++){
        if(defield[x1][i]) return i-1;
    }
    return N-1;
}
int BattleField::straight_y_up(int x1,int y1){
    for(int i=y1;i>=0;i--){
        if(defield[x1][i]) return i+1;
    }
    return 0;
}
bool BattleField::findpath(int x,int y){
    path_x.push_back(x);
    path_y.push_back(y);
    x=straight_x(x,y);
    path_x.push_back(x);
    path_y.push_back(y);
    int y1=straight_y_up(x,y);
    int y2=straight_y_down(x,y);
    if(x==N-1) return true;
    for(int j=y1;j<=y2;j++){
        int tmp=straight_x(x,j);
        if(x!=tmp&&findpath(x,j)) return true;
    }
    path_x.pop_back();
    path_y.pop_back();
    path_x.pop_back();
    path_y.pop_back();
    return false;
}
bool BattleField::findPath(){
    for(int j=0;j<N;j++){
        if(findpath(0,j))  return true;
    }
    return false;
}

