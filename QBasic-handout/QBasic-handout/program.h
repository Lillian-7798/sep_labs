//Declare/implement a stored program
//声明/实现一个存储程序
#ifndef PROGRAM_H
#define PROGRAM_H
#include "statement.h"

class program
{
    friend class MainWindow;
public:
    program();
    void clean();
    int getlength();
    statement* move(int lnum); //找到插入statement或者更改statement的位置
    void insert(int lnum,QString newtext);//插入或者替换statement
    void del(int lnum);
    statement* findline(int lnum);
private:
    int prolength = 0;
    statement *head;
    bool state = 1; //显示move到的位置是需要替换还是添加新的statement，0为替换，1为添加
};

#endif // PROGRAM_H
