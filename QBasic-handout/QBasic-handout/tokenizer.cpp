#include "tokenizer.h"
#include <QObject>
tokenizer::tokenizer()
{

}
QList<QString>tokenizer::ToTokens(QString expression)
{
    Tokens.clear();
    expression.remove(QChar(' '), Qt::CaseInsensitive); //删除表达式中的所有空格
    expression.remove('\n');
    int i=0,j=-1;//i表示当前符号的位置，j表示上一个符号的位置
    do{
        while((!isOperater(expression[i]))&&i<expression.length()) i++;
        if(i<expression.length()){
            //插入符号中间的数字
            if((i-j)>1) Tokens.append(expression.mid(j+1,i-j-1));
            //插入数字后面的符号**
            if(expression[i]=='*'&&expression[i+1]=='*'){
                Tokens.append(expression.mid(i,2));
                i++;
            }
            //插入数字后面的符号（正常）
            else Tokens.append(expression.mid(i,1));
            j=i;
            i++;
        }
    }while (i<expression.length());
    //插入最后的数字
    Tokens.append(expression.mid(j+1,expression.length()-j-1));
    return Tokens;
}


bool tokenizer::isOperater(QCharRef x){
    if(x=='+'||x=='-'||x=='*'||x=='/'||x=='('||x==')'||x=='=') return true;
    else return false;
}
