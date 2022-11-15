#include <iostream>
#include "simpletest.h"
#include "simple.h"
using namespace std;

int main()
{
    /*Test code start*/
    SimpleTest *test = new SimpleTest();
    QTest::qExec(test);
    delete test;
    /*Test code end*/
}
