/********************************************************************************
** Form generated from reading UI file 'sokoban.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOKOBAN_H
#define UI_SOKOBAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sokoban
{
public:

    void setupUi(QWidget *sokoban)
    {
        if (sokoban->objectName().isEmpty())
            sokoban->setObjectName(QString::fromUtf8("sokoban"));
        sokoban->resize(800, 600);

        retranslateUi(sokoban);

        QMetaObject::connectSlotsByName(sokoban);
    } // setupUi

    void retranslateUi(QWidget *sokoban)
    {
        sokoban->setWindowTitle(QCoreApplication::translate("sokoban", "sokoban", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sokoban: public Ui_sokoban {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOKOBAN_H
