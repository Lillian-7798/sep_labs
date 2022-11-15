#include "simpletest.h"
#include "simple.h"

/*boxDistribution分布
        X0 X1 X2 X3 X4 X5 X6 X7 X8 X9|
      Y0                             |
      Y1             2  2  2     1   |
      Y2    1     1        2  2  2   |
      Y3    1  2                     |
      Y4                         2   |
      Y5             2     2  1  2   |
      Y6                             |
      --------------------------------
  */

SimpleTest::SimpleTest()
{

}
//不同类型不能消除
void SimpleTest::case1_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(1,2,6,1)==false);
}
//同一个不能消除
void SimpleTest::case2_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(3,2,3,2)==false);
}
//直线消除
  //没有间隔
void SimpleTest::case3_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,1,6,2)==true);
}
void SimpleTest::case4_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,6,1)==true);
}
void SimpleTest::case5_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,7,2)==true);
}
void SimpleTest::case6_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(7,2,6,2)==true);
}
  //有间隔
void SimpleTest::case7_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(1,2,3,2)==true);
}
void SimpleTest::case8_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(3,2,1,2)==true);
}
void SimpleTest::case9_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,6,5)==true);
}
void SimpleTest::case10_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,5,6,2)==true);
}
//有有一个弯
 //连线在右上方
void SimpleTest::case11_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,1,7,2)==true);
}
void SimpleTest::case12_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(7,2,6,1)==true);
}
  //连线在左下方
void SimpleTest::case13_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,5,1)==true);
}
void SimpleTest::case14_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(5,1,6,2)==true);
}
  //连线在左上方
void SimpleTest::case15_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,4,5)==true);
}
void SimpleTest::case16_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,5,6,2)==true);
}
  //连线在右下方
void SimpleTest::case17_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(5,1,4,5)==true);
}
void SimpleTest::case18_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,5,5,1)==true);
}
//有两个拐角
void SimpleTest::case19_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,6,5)==true);
}
void SimpleTest::case20_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,5,6,2)==true);
}
void SimpleTest::case21_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(8,4,8,2)==true);
}
void SimpleTest::case22_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(8,2,8,4)==true);
}
void SimpleTest::case23_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,1,4,1)==true);
}
void SimpleTest::case24_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,1,6,1)==true);
}
void SimpleTest::case25_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,8,2)==true);
}
void SimpleTest::case26_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(8,2,6,2)==true);
}
void SimpleTest::case27_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,5,4,1)==true);
}
void SimpleTest::case28_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,1,6,5)==true);
}
void SimpleTest::case29_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,5,7,2)==true);
}
void SimpleTest::case30_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(7,2,6,5)==true);
}
void SimpleTest::case31_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(2,3,4,1)==true);
}
void SimpleTest::case32_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,1,2,3)==true);
}
void SimpleTest::case33_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(1,3,3,2)==true);
}
void SimpleTest::case34_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(3,2,1,3)==true);
}
void SimpleTest::case35_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,1,7,2)==true);
}
void SimpleTest::case36_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(7,2,4,1)==true);
}
void SimpleTest::case37_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(5,1,6,5)==true);
}
void SimpleTest::case38_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,5,5,1)==true);
}
//无法消除
void SimpleTest::case39_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,5,6,1)==false);
}
void SimpleTest::case40_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,1,4,5)==false);
}
void SimpleTest::case41_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(4,5,6,1)==false);
}
void SimpleTest::case42_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(6,2,8,5)==false);
}
void SimpleTest::case43_testcase(){
    Simple simple;
    QVERIFY(simple.desicionLogic(8,5,6,2)==false);
}
