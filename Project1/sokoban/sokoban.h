#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class sokoban; }
QT_END_NAMESPACE

class sokoban : public QWidget
{
    Q_OBJECT

public:
    sokoban(QWidget *parent = nullptr);
    ~sokoban();

private:
    Ui::sokoban *ui;

    //记录人物和箱子的位置，人物和箱子5*5
    int x[2],y[2];
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};
#endif // SOKOBAN_H
