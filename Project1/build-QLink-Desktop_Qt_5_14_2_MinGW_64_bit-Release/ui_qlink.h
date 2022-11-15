/********************************************************************************
** Form generated from reading UI file 'qlink.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLINK_H
#define UI_QLINK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QLink
{
public:

    void setupUi(QWidget *QLink)
    {
        if (QLink->objectName().isEmpty())
            QLink->setObjectName(QString::fromUtf8("QLink"));
        QLink->resize(800, 600);

        retranslateUi(QLink);

        QMetaObject::connectSlotsByName(QLink);
    } // setupUi

    void retranslateUi(QWidget *QLink)
    {
        QLink->setWindowTitle(QCoreApplication::translate("QLink", "QLink", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QLink: public Ui_QLink {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLINK_H
