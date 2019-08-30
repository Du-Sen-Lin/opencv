/********************************************************************************
** Form generated from reading UI file 'QtTask2.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTASK2_H
#define UI_QTTASK2_H

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

class Ui_QtTask2Class
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtTask2Class)
    {
        if (QtTask2Class->objectName().isEmpty())
            QtTask2Class->setObjectName(QStringLiteral("QtTask2Class"));
        QtTask2Class->resize(600, 403);
        centralWidget = new QWidget(QtTask2Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(330, 20, 191, 231));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 20, 231, 231));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 290, 88, 23));
        QtTask2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtTask2Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        QtTask2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtTask2Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtTask2Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtTask2Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtTask2Class->setStatusBar(statusBar);

        retranslateUi(QtTask2Class);
        QObject::connect(pushButton, SIGNAL(clicked()), QtTask2Class, SLOT(OnClicked()));

        QMetaObject::connectSlotsByName(QtTask2Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtTask2Class)
    {
        QtTask2Class->setWindowTitle(QApplication::translate("QtTask2Class", "QtTask2", Q_NULLPTR));
        label->setText(QApplication::translate("QtTask2Class", "TextLabel", Q_NULLPTR));
        label_2->setText(QApplication::translate("QtTask2Class", "TextLabel", Q_NULLPTR));
        pushButton->setText(QApplication::translate("QtTask2Class", "Open", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtTask2Class: public Ui_QtTask2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTASK2_H
