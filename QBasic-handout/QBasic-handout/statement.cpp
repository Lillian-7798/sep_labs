#include "statement.h"

statement::statement()
{
    lineNumber=0;
}
statement::statement(QString line,statement *next){
    int i=0;
    //line.remove(' ');
    while (line[i]>='0'&&line[i]<='9') i++;
    lineNumber=line.mid(0,i).toInt();
    text=line.mid(i+1,line.length()-i-1);
    this->next=next;
}
int statement::getLineNumber()
{
    return lineNumber;
}

QString statement::getText()
{
    return text;
}
