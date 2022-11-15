//Parse a given expression
//解析给定表达式
#ifndef PARSER_H
#define PARSER_H

#include "exp.h"
#include <QList>
#include <QString>
#include <stack>
#include <deque>

class parser
{
public:
    parser();
    Expression * ToTree(QList<QString> tokens);
    QString printTreeExp(QList<QString> tokens,int n);
private:
    stack<QString> Operators;
    stack<Expression*> Operands;

    bool IsCompoundexp(QString);
    bool IsConstantexp(QString);
    bool IsIdentifierexp(QString);

    int GetPriority(QString ope);//符号优先级
};

#endif // PARSER_H
