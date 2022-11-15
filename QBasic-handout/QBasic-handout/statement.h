//Declare/implement statements
//声明/执行语句
#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include <QObject>
//#include "mainwindow.h"
//#include "program.h"

class statement
{
    friend class MainWindow;
    friend class program;
public:
    statement();
    statement(QString line,statement *next=NULL);
    int getLineNumber();
    QString getText();
private:
    int lineNumber ;
    QString text;
    statement *next;
    QList<QString> lst;
};

#endif // STATEMENT_H
