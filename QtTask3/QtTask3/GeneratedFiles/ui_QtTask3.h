/********************************************************************************
** Form generated from reading UI file 'QtTask3.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTASK3_H
#define UI_QTTASK3_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtTask3Class
{
public:
    QWidget *centralWidget;
    QLabel *label_1;
    QPushButton *btn_open;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtTask3Class)
    {
        if (QtTask3Class->objectName().isEmpty())
            QtTask3Class->setObjectName(QStringLiteral("QtTask3Class"));
        QtTask3Class->resize(600, 400);
        centralWidget = new QWidget(QtTask3Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_1 = new QLabel(centralWidget);
        label_1->setObjectName(QStringLiteral("label_1"));
        label_1->setGeometry(QRect(50, 20, 301, 211));
        btn_open = new QPushButton(centralWidget);
        btn_open->setObjectName(QStringLiteral("btn_open"));
        btn_open->setGeometry(QRect(50, 290, 93, 28));
        QtTask3Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtTask3Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        QtTask3Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtTask3Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtTask3Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtTask3Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtTask3Class->setStatusBar(statusBar);

        retranslateUi(QtTask3Class);
        QObject::connect(btn_open, SIGNAL(clicked()), QtTask3Class, SLOT(openCamera()));

        QMetaObject::connectSlotsByName(QtTask3Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtTask3Class)
    {
        QtTask3Class->setWindowTitle(QApplication::translate("QtTask3Class", "QtTask3", Q_NULLPTR));
        label_1->setText(QApplication::translate("QtTask3Class", "TextLabel", Q_NULLPTR));
        btn_open->setText(QApplication::translate("QtTask3Class", "openCamera", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtTask3Class: public Ui_QtTask3Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTASK3_H
