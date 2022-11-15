#include "parser.h"

parser::parser()
{

}
bool parser::IsConstantexp(QString x){
    bool ok;
    x.toInt(&ok);
    return ok;
}
bool parser::IsCompoundexp(QString x){
    if(x == "+" || x == "-" || x == "*" || x == "/" || x == "(" || x == ")" || x == "="||x =="**") return true;
    else return false;
}
bool parser::IsIdentifierexp(QString x){
    if(x[0]>='A'&&x[0]<='Z') return true;
    if(x[0]>='a'&&x[0]<='z') return true;
    return false;
}
int parser::GetPriority(QString ope){
    if(ope=="(")return 0;
    if(ope=="=") return 1;
    if(ope=="+"||ope=="-") return 2;
    if(ope=="*"||ope=="/") return 3;
    if(ope=="**") return 4;
}
Expression* parser::ToTree(QList<QString> tokens){
    //先给-号前加上0，再根据二元符号性质判断表达式的合法性
    if(tokens[0] == "-")
    {
        tokens.insert(0,"0");
    }
    for(int i = 0;i<tokens.length();++i)
    {
        int j = i+1;
        if((tokens[i] == "(" ||tokens[i] == "=")&& tokens[j] == "-")
            tokens.insert(j,"0");
    }
    QString tokensstr = "";
    for(int i = 0;i<tokens.length();++i)
    {
        if(tokens[i] == "+"||tokens[i] == "-"||tokens[i] == "*"||tokens[i] == "/"||tokens[i] == "=")
        {
            tokensstr += "+";
        }
        else {
            tokensstr += tokens[i];
        }
    }

    if(tokensstr.contains("++"))
    {
        QString T = "Illegal operator in expression , an operator must have left value and right value!";
        throw (T);
    }



    //判断表达式合法后开始构造表达式树
        Expression *root = NULL;
    for(int k=0;k<tokens.length();k++){
        //构造常数并压入栈
        if(IsConstantexp(tokens[k])) {
            ConstantExp *constant =new ConstantExp(tokens[k].toInt());
            Operands.push(constant);
        }
        //构造变量并压入栈
        if(IsIdentifierexp(tokens[k])){
            IdentifierExp *identifier = new IdentifierExp(tokens[k].toStdString());
            Operands.push(identifier);
        }
        //考虑是符号时
        if(IsCompoundexp(tokens[k])){
            if(tokens[k]=="("){
                Operators.push(tokens[k]);
            }
            else{
                if(tokens[k]==")"){
                    while(Operators.top()!="("){
                        QString op = Operators.top();
                        Operators.pop();

                        //构造左右两个子节点
                        Expression *left,*right;
                        if(!Operands.empty()) {
                            right = Operands.top();
                            Operands.pop();
                        }
                        else right = NULL;
                        if(!Operands.empty()) {
                            left = Operands.top();
                            Operands.pop();
                        }
                        else left = NULL;
                        //构造复合语句压入栈
                        CompoundExp *compound = new CompoundExp(op.toStdString(),left,right);
                        Operands.push(compound);
                    }
                    Operators.pop();
                }
                else{
                    if(Operators.empty())            //如果符号栈是空的，就直接进
                    {
                        Operators.push(tokens[k]);
                    }
                    else
                    {
                        int now=GetPriority(tokens[k]);//当前符号优先级
                        int bef=GetPriority(Operators.top());//栈顶符号优先级
                        if(now>bef) Operators.push(tokens[k]);//优先级大于，压入符号栈
                        else{
                            while (now<=bef) {
                                QString op = Operators.top();
                                Operators.pop();
                                //构造左右两个子节点
                                Expression *left,*right;
                                if(!Operands.empty()) {
                                    right = Operands.top();
                                    Operands.pop();
                                }
                                else right = NULL;
                                if(!Operands.empty()) {
                                    left = Operands.top();
                                    Operands.pop();
                                }
                                else left = NULL;
                                //构造复合语句压入栈
                                CompoundExp *compound = new CompoundExp(op.toStdString(),left,right);
                                Operands.push(compound);
                                if(!Operators.empty()) bef=GetPriority(Operators.top());
                                else break;
                            }
                            Operators.push(tokens[k]);
                        }
                    }
                }
            }
        }

    }
    //全部压入栈后栈中还有残余
    while (!Operators.empty()) {
        QString op = Operators.top();
        Operators.pop();
        //构造左右两个子节点
        Expression *left,*right;
        if(!Operands.empty()) {
            right = Operands.top();
            Operands.pop();
        }
        else right = NULL;
        if(!Operands.empty()) {
            left = Operands.top();
            Operands.pop();
        }
        else left = NULL;
        //构造复合语句压入栈
        CompoundExp *compound = new CompoundExp(op.toStdString(),left,right);
        Operands.push(compound);
    }
    root = Operands.top();
    return root;
}
QString parser::printTreeExp(QList<QString> tokens,int n){
    Expression *p=ToTree(tokens);
    int num=n;
    QString output="";
    deque<Expression*> stac;
    deque<int> number;
    stac.push_back(p);
    number.push_back(0);
    while(!stac.empty()){
        p=stac.front();
        num=number.front();
        number.pop_front();
        stac.pop_front();
        ExpressionType type = p->type();
        if(type==CONSTANT) {
            for(int i=0;i<num;i++) output+="\t";
            output+=QString::number(p->getConstantValue())+"\n";
        }
        if(type==IDENTIFIER) {
            for(int i=0;i<num;i++) output+="\t";
            output+= QString::fromStdString(p->getIdentifierName())+"\n";
        }
        if(type==COMPOUND){
            for(int i=0;i<num;i++) output+="\t";
            output+=QString::fromStdString(p->getOperator())+"\n";
            stac.push_back(p->getLHS());
            number.push_back(num+1);
            stac.push_back(p->getRHS());
            number.push_back(num+1);
        }
    }
    return output;
}
