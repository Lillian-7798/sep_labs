#include "program.h"

program::program()
{
    prolength=0;
    head = new statement;
}
void program::clean()
{
    statement *p = head->next;
    statement *q;
    head->next = nullptr;
    while(p!=nullptr)
    {
        q = p->next;
        delete p;
        p = q;
    }
    prolength = 0;
}
int program::getlength()
{
    return prolength;
}
statement* program::move(int lnum)
{
    /*statement *p = head;
    if(head!=NULL){
        while (p->lineNumber<lnum)
    }*/
    statement*p = head->next;
    statement*q = head;
    if(head->next!=nullptr)
    {
        while((p->lineNumber < lnum ||lnum == 0)&&(p->next!=nullptr))
        {
            p = p->next;
            q = q->next;
        }

        if(p->lineNumber == lnum)
        {
            state = 0;      //0为要替换，1为要插入
            return p;
        }
        else
        {
            if(p->next == nullptr&&p->lineNumber<lnum)
            {
                state = 1;
                return p;
            }
            else {
                state = 1;
                return q;
            }
        }
    }
    else {
        state = 1;
    return head;
    }
}

void program::insert(int lnum,QString newtext)
{
        move(lnum);
        if(state == 0)      //需要替换
        {
            statement *tmp = move(lnum);
            statement s(newtext);
            tmp->text = s.text;
            /*tmp->text = newtext;
            QList<QString> tmplist = newtext.split(' ',QString::SkipEmptyParts);
            int tmplinenum = tmplist[0].toInt();
            tmp->lineNumber = tmplinenum;*/
        }
        else          //需要加新命令
        {
            statement *tmp = move(lnum);
            //statement *tmpq = tmp;
            /*if(tmp->next == nullptr)             //如果是在尾部添加statement
            {
               tmp->next = new statement(newtext,nullptr);
               prolength++;
            }
            else{                                //如果是在中间添加
                tmp->next = new statement(newtext,tmpq->next);
                prolength++;
            }*/
            if(tmp==head&&tmp->lineNumber>lnum){
                head=new statement(newtext,head);
                prolength++;
            }
            else{
                tmp->next=new statement(newtext,tmp->next);
                prolength++;
            }
       }
}
void program::del(int num)
{
    statement *p = head->next;
    statement *q = head;
    if(head->lineNumber==num) {head=head->next;delete q;}
    else{
        while(p!=nullptr)
        {
            if(p->lineNumber == num)
            {
                q->next = p->next;
                delete p;
                break;
            }
            p = p->next;
            q = q->next;
        }
    }

}

statement* program::findline(int lnum)
{
    statement*tmp = head;
    while(tmp!=nullptr)
    {
        if(tmp->lineNumber == lnum)
            return tmp;

        tmp = tmp->next;
    }
    return nullptr;
}
