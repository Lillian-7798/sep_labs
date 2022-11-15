#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tokenizer.h"
#include "program.h"
#include "parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdLineEdit_editingFinished();
    void clearAll();//与“清楚代码”连接，用于清空三个方框内所有内容
    void codeLineEdit_return();//在命令输入窗口输入内容，并显示在代码窗口部分
    void run();
    void getInput();
private:
    Ui::MainWindow *ui;
    tokenizer *tok;
    parser *par;
    program *pro;
    EvaluationContext var;
    QString inputvar;//存储INPUT的变量名
    statement*inputpos;            //需要input的位置
    QString loadCode(QString originalCode,QString input);
    void  printTree();//用于打印某一句语法树
    void runProgram(EvaluationContext &var,statement* start);
    bool isinput=false;
    bool IfCondition(QString str);
    //一些和主功能无关的函数小工具
    int getLineNumber(QString command);//将string型命令的前端行号转化成int型
};
#endif // MAINWINDOW_H

