#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect
    connect(ui->btnClearCode,SIGNAL(clicked()),this,SLOT(clearAll()));// "清除代码"
    connect(ui->cmdLineEdit,SIGNAL(returnPressed()),this,SLOT(codeLineEdit_return()));
    connect(ui->btnRunCode,SIGNAL(clicked()),this,SLOT(run()));
    //connect(ui->textBrowser,SIGNAL(returenPressed()),this,SLOT(getInput()));
    //初始化
    par = new parser;
    pro = new program;
    tok = new tokenizer;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getInput(){

}
int MainWindow::getLineNumber(QString command){
    std::string tmp=command.toStdString();
    int i=0,lineNumber=0;
    while(tmp[i]!=' '&&tmp[i]!='\n'){
        lineNumber=lineNumber*10+tmp[i]-'0';
        i++;
    }
    return lineNumber;
}
QString MainWindow::loadCode(QString originalCode,QString input){
    try {
        if(input!="\n"){
            if(input[0]<'0'||input[0]>'9'){
                QString cmd=input.left(input.indexOf(' '));
                if(cmd=="LET"){
                    Expression* expRoot;
                    try {
                        expRoot=par->ToTree(tok->ToTokens(input.remove("LET ")));
                    } catch (QString error) {
                        cout<<error.toStdString()<<endl;
                    }
                    try {
                        expRoot->eval(var);
                    } catch (QString error) {
                        cout<<error.toStdString()<<endl;
                    }
                    QString output="";
                    output+="LET ";
                    output+=par->printTreeExp(tok->ToTokens(input.remove("LET ")),0);
                    ui->treeDisplay->append(output);
                }
                if(cmd=="PRINT"){
                    Expression* expRoot;
                    try {
                        expRoot=par->ToTree(tok->ToTokens(input.remove("PRINT ")));
                    } catch (QString error) {
                        cout<<error.toStdString()<<endl;
                    }
                    int num;
                    try {
                        num=expRoot->eval(var);
                    } catch (QString error) {
                        cout<<error.toStdString()<<endl;
                    }
                    ui->textBrowser->append(QString::number(num)+"\n");
                    QString output="";
                    output+="PRINT\n";
                    output+=par->printTreeExp(tok->ToTokens(input.remove("PRINT ")),1);
                    ui->treeDisplay->append(output);
                }
            }
            else{
            if(input[0]<'0'&&input[0]>'9') throw "input wrong";
            //先将input里写的行号转化成数字便于排序，由于接下来代码框内行号也需要转化行号，索性写成一个函数
            int inputLine=getLineNumber(input);
            int i=-1;
            //删除语句
            int j=0;
            bool isdelete=false;
            while(input[j]>='0'&&input[j]<='9')
                j++;
            if(input[j]=='\n') isdelete=true;
            //开始输入第一个语句
            if(originalCode.length()==0){
                originalCode.insert(0,input);
                statement *tmp=new statement(input);
                pro->head=tmp;
            }
            else{
                bool isinsert=false;
                //寻找输入语句插入的位置
                while(i<originalCode.length()-1){
                    i++;
                    //插入语句
                    if(inputLine<getLineNumber(originalCode.mid(i))&&!isdelete)
                    {
                        originalCode.insert(i,input);
                        pro->insert(inputLine,input);
                        isinsert=true;
                        break;
                    }
                    //比较插入语句和原来语句的行号，一旦<立即跳出循环
                    if(inputLine==getLineNumber(originalCode.mid(i))){
                        isinsert = true;
                        //更新语句
                        if(!isdelete) {
                            originalCode.insert(i,input);
                            pro->insert(inputLine,input);
                            i+=input.length();
                        }
                        //删除语句
                        j=i;
                        while (originalCode[j]!='\n') j++;
                        originalCode.remove(i,j-i+1);
                        if(isdelete) pro->del(inputLine);
                        break;


                    }
                    //寻找下一个语句的开始
                    while (originalCode[i]!='\n') i++;
                }
                if(!isinsert&&!isdelete){
                originalCode.insert(originalCode.length(),input);
                pro->insert(inputLine,input);}
            }
        }
        }
    } catch (QString error) {
        std::cout<<error.toStdString()<<std::endl;
    }
    return originalCode;
}
//load代码
void MainWindow::on_cmdLineEdit_editingFinished()
{
}
//与"清除代码"连接，用于清空三个方框内所有内容
void MainWindow::clearAll()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    if(pro->getlength()!=0) pro->clean();
}
//在命令输入窗口输入内容，并显示在代码窗口部分
void MainWindow::codeLineEdit_return(){
    try{
        QString input = ui->cmdLineEdit->text();
        if(!isinput){
            if(input=="RUN") run();
            else{
                if(input=="CLEAR") clearAll();
                else{
                    if(input=="LOAD") on_cmdLineEdit_editingFinished();
                    else{
                        if(input=="QUIT") this ->close();
                        else{
                            input.insert(input.length(),'\n');
                            ui->cmdLineEdit->clear();
                            input=loadCode(ui->CodeDisplay->toPlainText(),input);
                            ui->CodeDisplay->clear();
                            ui->CodeDisplay->append(input);
                        }
                    }
                }
            }
        }
        else{
            int num = ui->cmdLineEdit->text().remove('?').remove(' ').toInt();
            ui->cmdLineEdit->clear();
            //ui->textBrowser->append(QString::number(num)+'\n');
            var.setValue(inputvar.toStdString(),num);
            isinput=false;
            if(inputpos->next!=NULL)
            runProgram(var,inputpos->next);
        }
    }catch(QString error){
        std::cout<<error.toStdString()<<endl;
    }
}
void MainWindow::run(){
    try {
            printTree();
    } catch (QString error) {
        cout<<error.toStdString()<<endl;
    }
    try {
        runProgram(var,pro->head);
    } catch (QString error) {
        cout<<error.toStdString()<<endl;
    }
}
void MainWindow::runProgram(EvaluationContext &var,statement *start){
    statement *now = start;
    QString output="";
    while (now!=NULL) {
        QString tmp = now->text;
        QString cmd=tmp.left(tmp.indexOf(' '));//取指令前面进行判断
        if(cmd=="END") break;
        if(cmd=="REM") {now=now->next;continue;}
        if(cmd=="LET"){
            Expression* expRoot;
            try {
                expRoot=par->ToTree(tok->ToTokens(tmp.remove("LET ")));
            } catch (QString error) {
                cout<<error.toStdString()<<endl;
            }
            try {
                expRoot->eval(var);
            } catch (QString error) {
                cout<<error.toStdString()<<endl;
            }
        }
        if(cmd=="PRINT"){
            Expression* expRoot;
            try {
                expRoot=par->ToTree(tok->ToTokens(tmp.remove("PRINT ")));
            } catch (QString error) {
                cout<<error.toStdString()<<endl;
            }
            int num;
            try {
                num=expRoot->eval(var);
            } catch (QString error) {
                cout<<error.toStdString()<<endl;
            }
            output+=QString::number(num)+"\n";
        }
        if(cmd=="INPUT"){
            //ui->textBrowser->append("? ");
            ui->cmdLineEdit->insert("?");
            inputvar=tmp.remove("INPUT ").remove('\n');
            inputpos = now;
            isinput=true;
            return;
        }
        if(cmd=="IF"){
            QString tmp1=tmp.remove("IF ").remove("THEN ").remove('\n');
            int i=0;
            while(tmp1[i]!='<'&&tmp1[i]!='>'&&tmp1[i]!='=')i++;
            bool ok = false;
            try {
                ok=IfCondition(tmp1.mid(0,tmp1.lastIndexOf(' ')).remove(' '));
            } catch (QString error) {
                cout<<error.toStdString()<<endl;
            }
            if(ok) {
                int n =tmp1.mid(tmp1.lastIndexOf(' ')+1,tmp1.length()-tmp1.lastIndexOf(' ')-1).toInt();
                if(pro->findline(n)!=NULL){
                    now=pro->findline(n);
                    continue;
                }
                else throw "The linenumber is invalid！";
            }
        }
        if(cmd=="GOTO"){
            int linum = tmp.remove("GOTO ").toInt();
            if(pro->findline(linum)==NULL) throw "The linenumber is invalid！";
            else now=pro->findline(linum);
            continue;
        }
        now=now->next;
    }
    ui->textBrowser->append(output);
}
void MainWindow::printTree(){
    statement *now = pro->head;
    QString output="";
    while(now!=NULL){
        QString tmp = now ->text;
        output+=QString::number(now->getLineNumber())+" ";
        QString cmd=tmp.left(tmp.indexOf(' '));//取指令前面进行判断
        //对cmd进行分类
        if(cmd=="REM"){
            output+="REM\n\t"+tmp.remove("REM ");
        }
        else if(cmd=="LET"){
            output+="LET ";
            output+=par->printTreeExp(tok->ToTokens(tmp.remove("LET ")),0);
        }
        else if(cmd=="IF"){
            output+="IF THEN\n\t";
            //拆分出语句中的变量
            QString tmp1=tmp.remove("IF ").remove("THEN ");
            int i=0;
            while(tmp[i]!='<'&&tmp1[i]!='>'&&tmp1[i]!='=')i++;
            output+=tmp1.mid(0,i-1)+"\n\t";
            output+=tmp1.mid(i,1)+"\n\t";
            output+=tmp1.mid(i+2,tmp1.lastIndexOf(' ')-i-2)+"\n\t";
            output+=tmp1.mid(tmp1.lastIndexOf(' ')+1,tmp1.length()-tmp1.lastIndexOf(' ')-1)+'\n';
        }
        else if(cmd=="GOTO"){
            output+="GOTO\n\t";
            output+=tmp.mid(5,tmp.length()-5)+"\n";
        }
        else if(cmd=="PRINT"){
            output+="PRINT\n";
            output+=par->printTreeExp(tok->ToTokens(tmp.remove("PRINT ")),1);
        }
        else {
            output+=cmd+'\n';
        }
        now=now->next;
    }
    ui->treeDisplay->clear();
    ui->treeDisplay->append(output);
}
bool MainWindow::IfCondition(QString str)
{

   if(str.contains("="))
    {
        QList<QString> ifstrs = str.split('=',QString::SkipEmptyParts);

        QList<QString>tokensleft;
        tokensleft.clear();
        tokensleft = tok->ToTokens(ifstrs[0]);
        //Expression*valleft = Parser->ToTree(tokensleft);                   //化成表达树
        Expression*valleft;

        try {
            valleft = par->ToTree(tokensleft);                   //化成表达树
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }
        int numleft ;
        QString err;
        try {
            numleft = valleft->eval(var);                                        //进行求值
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }

        QList<QString>tokensright;
        tokensright.clear();
        tokensright = tok->ToTokens(ifstrs[1]);
        //Expression*valright = Parser->ToTree(tokensright);                   //化成表达树
        Expression*valright;

        try {
            valright = par->ToTree(tokensright);                   //化成表达树
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }
        int numright ;                             //进行求值
        QString err2;
        try {
            numright = valright->eval(var);                                        //进行求值
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }

        if(numleft == numright)
        {
            return true;
        }
        else {
            return false;
        }
    }
    else if (str.contains(">")) {
        QList<QString> ifstrs = str.split('>',QString::SkipEmptyParts);
        QList<QString>tokensleft;
        tokensleft.clear();
        tokensleft = tok->ToTokens(ifstrs[0]);
        //Expression*valleft = Parser->ToTree(tokensleft);                   //化成表达树
        Expression*valleft;

        try {
            valleft = par->ToTree(tokensleft);                   //化成表达树
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }
        int numleft ;
        QString err;
        try {
            numleft = valleft->eval(var);                                        //进行求值
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }

        QList<QString>tokensright;
        tokensright.clear();
        tokensright = tok->ToTokens(ifstrs[1]);
        //Expression*valright = Parser->ToTree(tokensright);                   //化成表达树
        Expression*valright;

        try {
            valright = par->ToTree(tokensright);                   //化成表达树
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }
        int numright ;                             //进行求值
        QString err2;
        try {
            numright = valright->eval(var);                                        //进行求值
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }

        if(numleft > numright)
        {
            return true;
        }
        else {
            return false;
        }
    }
    else if (str.contains("<")) {
        QList<QString> ifstrs = str.split('<',QString::SkipEmptyParts);
        QList<QString>tokensleft;
        tokensleft.clear();
        tokensleft = tok->ToTokens(ifstrs[0]);
        //Expression*valleft = Parser->ToTree(tokensleft);                   //化成表达树
        Expression*valleft;

        try {
            valleft = par->ToTree(tokensleft);                   //化成表达树
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }
        int numleft ;
        QString err;
        try {
            numleft = valleft->eval(var);                                        //进行求值
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }

        QList<QString>tokensright;
        tokensright.clear();
        tokensright = tok->ToTokens(ifstrs[1]);
        //Expression*valright = Parser->ToTree(tokensright);                   //化成表达树
        Expression*valright;

        try {
            valright = par->ToTree(tokensright);                   //化成表达树
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }
        int numright ;                             //进行求值
        QString err2;
        try {
            numright = valright->eval(var);                                        //进行求值
        } catch (QString errorTips) {
            cout<<errorTips.toStdString()<<endl;
        }

        //inputconsole->write(QString::number(numleft));

        if(numleft < numright)
        {
            return true;
        }
        else {
            return false;
        }


    }
    else {
        QList<QString> ifstrs;            //这里要记得写报错条件
    }
    return false;
}
