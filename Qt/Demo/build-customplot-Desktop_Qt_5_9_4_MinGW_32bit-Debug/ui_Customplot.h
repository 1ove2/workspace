/********************************************************************************
** Form generated from reading UI file 'Customplot.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMPLOT_H
#define UI_CUSTOMPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Customplot
{
public:

    void setupUi(QWidget *Customplot)
    {
        if (Customplot->objectName().isEmpty())
            Customplot->setObjectName(QStringLiteral("Customplot"));
        Customplot->resize(400, 300);

        retranslateUi(Customplot);

        QMetaObject::connectSlotsByName(Customplot);
    } // setupUi

    void retranslateUi(QWidget *Customplot)
    {
        Customplot->setWindowTitle(QApplication::translate("Customplot", "Customplot", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Customplot: public Ui_Customplot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMPLOT_H
