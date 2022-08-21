/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actConnect;
    QWidget *centralwidget;
    QSpinBox *spinBoxPort;
    QComboBox *comboBoxHostIP;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(834, 606);
        actConnect = new QAction(MainWindow);
        actConnect->setObjectName(QStringLiteral("actConnect"));
        actConnect->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/ico/images/network.png"), QSize(), QIcon::Normal, QIcon::Off);
        actConnect->setIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        spinBoxPort = new QSpinBox(centralwidget);
        spinBoxPort->setObjectName(QStringLiteral("spinBoxPort"));
        spinBoxPort->setGeometry(QRect(270, 220, 42, 22));
        comboBoxHostIP = new QComboBox(centralwidget);
        comboBoxHostIP->setObjectName(QStringLiteral("comboBoxHostIP"));
        comboBoxHostIP->setGeometry(QRect(100, 180, 69, 22));
        comboBoxHostIP->setEditable(false);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 834, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        toolBar->setFont(font);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actConnect);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actConnect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
