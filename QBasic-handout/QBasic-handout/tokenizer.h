//Convert strings to a list of tokens
//将字符串转换为标记列表
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QString>
#include <QObject>
class tokenizer
{
public:
    tokenizer();
    QList<QString>ToTokens(QString expression);
private:
    QList<QString>Tokens;
    bool isOperater(QCharRef cha);
};

#endif // TOKENIZER_H
