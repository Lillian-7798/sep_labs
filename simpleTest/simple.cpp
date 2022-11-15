#include "simple.h"
#include <algorithm>

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

Simple::Simple()
{
    boxDistribution[4][1]=2;
    boxDistribution[5][1]=2;
    boxDistribution[6][1]=2;
    boxDistribution[8][1]=1;
    boxDistribution[1][2]=1;
    boxDistribution[3][2]=1;
    boxDistribution[6][2]=2;
    boxDistribution[7][2]=2;
    boxDistribution[8][2]=2;
    boxDistribution[1][3]=1;
    boxDistribution[2][3]=2;
    boxDistribution[8][4]=2;
    boxDistribution[4][5]=2;
    boxDistribution[6][5]=2;
    boxDistribution[7][5]=1;
    boxDistribution[8][5]=2;

}
bool Simple::noTurnLinked(int x1,int y1, int x2,int y2){
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
bool Simple::desicionLogic(int x1,int y1,int x2,int y2){
    //类型不同无法相消
    if(boxDistribution[x1][y1]!=boxDistribution[x2][y2]) return false;
    //同一个箱子无法相消
    if(x1==x2&&y1==y2) return false;
    //没有拐弯
    if((x1==x2||y1==y2)&&noTurnLinked(x1,y1,x2,y2))
        return true;
    //一个拐弯
    if(noTurnLinked(x1,y1,(x2>x1)?x2+1:x2-1,y1)&&noTurnLinked(x2,(y1>y2)?y1+1:y1-1,x2,y2))
        return true;
    if(noTurnLinked(x1,y1,x1,(y2>y1)?y2+1:y2-1)&&noTurnLinked((x1<x2)?x1-1:x1+1,y2,x2,y2))
        return true;
    //两个拐弯
    for(int i=0;i<7;i++){
        if(noTurnLinked(x1,(y1>i)?i-1:i+1,x1,y1)&&noTurnLinked((x1>x2)?x2-1:x1-1,i,(x1>x2)?x1+1:x2+1,i)&&noTurnLinked(x2,(y2>i)?i-1:i+1,x2,y2))
            return true;
    }
    for(int i=0;i<10;i++){
        if(noTurnLinked((x1>i)?i-1:i+1,y1,x1,y1)&&noTurnLinked(i,(y1>y2)?y2-1:y1-1,i,(y1>y2)?y1+1:y2+1)&&noTurnLinked((x2>i)?i-1:i+1,y2,x2,y2))
            return true;
    }
    return false;
}
