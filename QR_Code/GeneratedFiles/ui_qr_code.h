/********************************************************************************
** Form generated from reading UI file 'qr_code.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QR_CODE_H
#define UI_QR_CODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QR_CodeClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QR_CodeClass)
    {
        if (QR_CodeClass->objectName().isEmpty())
            QR_CodeClass->setObjectName(QStringLiteral("QR_CodeClass"));
        QR_CodeClass->resize(600, 400);
        centralWidget = new QWidget(QR_CodeClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(90, 90, 75, 23));
        QR_CodeClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QR_CodeClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        QR_CodeClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QR_CodeClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QR_CodeClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QR_CodeClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QR_CodeClass->setStatusBar(statusBar);

        retranslateUi(QR_CodeClass);

        QMetaObject::connectSlotsByName(QR_CodeClass);
    } // setupUi

    void retranslateUi(QMainWindow *QR_CodeClass)
    {
        QR_CodeClass->setWindowTitle(QApplication::translate("QR_CodeClass", "QR_Code", 0));
        pushButton->setText(QApplication::translate("QR_CodeClass", "\346\265\213\350\257\225", 0));
    } // retranslateUi

};

namespace Ui {
    class QR_CodeClass: public Ui_QR_CodeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QR_CODE_H
