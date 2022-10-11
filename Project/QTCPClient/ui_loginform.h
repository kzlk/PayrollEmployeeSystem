/********************************************************************************
** Form generated from reading UI file 'loginform.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFORM_H
#define UI_LOGINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginForm
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *ilinEdit_pAdress;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QLineEdit *lineEdit_port;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_login_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_login;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_password;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_login;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *loginForm)
    {
        if (loginForm->objectName().isEmpty())
            loginForm->setObjectName(QString::fromUtf8("loginForm"));
        loginForm->resize(343, 420);
        centralwidget = new QWidget(loginForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        ilinEdit_pAdress = new QLineEdit(groupBox_2);
        ilinEdit_pAdress->setObjectName(QString::fromUtf8("ilinEdit_pAdress"));

        horizontalLayout_3->addWidget(ilinEdit_pAdress);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        lineEdit_port = new QLineEdit(groupBox_2);
        lineEdit_port->setObjectName(QString::fromUtf8("lineEdit_port"));

        horizontalLayout_4->addWidget(lineEdit_port);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_2);

        pushButton_login_3 = new QPushButton(groupBox_2);
        pushButton_login_3->setObjectName(QString::fromUtf8("pushButton_login_3"));

        verticalLayout_2->addWidget(pushButton_login_3);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit_login = new QLineEdit(groupBox);
        lineEdit_login->setObjectName(QString::fromUtf8("lineEdit_login"));

        horizontalLayout->addWidget(lineEdit_login);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_password = new QLineEdit(groupBox);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));

        horizontalLayout_2->addWidget(lineEdit_password);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_3);

        pushButton_login = new QPushButton(groupBox);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));

        verticalLayout->addWidget(pushButton_login);


        gridLayout->addWidget(groupBox, 1, 1, 1, 1);

        loginForm->setCentralWidget(centralwidget);
        menubar = new QMenuBar(loginForm);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 343, 26));
        loginForm->setMenuBar(menubar);
        statusbar = new QStatusBar(loginForm);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        loginForm->setStatusBar(statusbar);

        retranslateUi(loginForm);

        QMetaObject::connectSlotsByName(loginForm);
    } // setupUi

    void retranslateUi(QMainWindow *loginForm)
    {
        loginForm->setWindowTitle(QCoreApplication::translate("loginForm", "MainWindow", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("loginForm", "\320\237\321\226\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\275\321\217 \320\264\320\276 \321\201\320\265\321\200\320\262\320\265\321\200\320\260", nullptr));
        label_3->setText(QCoreApplication::translate("loginForm", "\320\206P-\320\260\320\264\321\200\320\265\321\201", nullptr));
        label_4->setText(QCoreApplication::translate("loginForm", "\320\237\320\276\321\200\321\202", nullptr));
        pushButton_login_3->setText(QCoreApplication::translate("loginForm", "\320\237\321\226\320\264'\321\224\320\264\320\275\320\260\321\202\320\270\321\201\321\217", nullptr));
        groupBox->setTitle(QCoreApplication::translate("loginForm", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\321\226\321\217", nullptr));
        label->setText(QCoreApplication::translate("loginForm", "\320\233\320\276\320\263\321\226\320\275", nullptr));
        label_2->setText(QCoreApplication::translate("loginForm", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        pushButton_login->setText(QCoreApplication::translate("loginForm", "\320\243\320\262\321\226\320\271\321\202\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginForm: public Ui_loginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
