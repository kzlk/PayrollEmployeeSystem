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
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginForm
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QWidget *page;
    QFrame *frame;
    QLineEdit *lineEdit_login;
    QLineEdit *lineEdit_password;
    QPushButton *pushButton_login;
    QLabel *label_6;
    QPushButton *minimizeButton;
    QPushButton *closeButton;
    QWidget *page_2;
    QLabel *label;
    QPushButton *closeButton_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *loginForm)
    {
        if (loginForm->objectName().isEmpty())
            loginForm->setObjectName(QString::fromUtf8("loginForm"));
        loginForm->resize(517, 335);
        loginForm->setStyleSheet(QString::fromUtf8("#statusbar\n"
"{\n"
"	color: white;\n"
"	border-radius: 15px;\n"
"}\n"
"#centralwidget\n"
"{\n"
"	border-radius: 15px;\n"
"}\n"
"#loginForm\n"
"{\n"
"\n"
"background:#2D2D2D;\n"
"border-radius: 15px;\n"
"	\n"
"}\n"
"\n"
"#frame\n"
"{\n"
"	background:#2D2D2D;\n"
"	border-radius: 15px;\n"
"}\n"
"\n"
"\n"
"\n"
"#toolButton\n"
"{\n"
"	background: red;\n"
"	border-radius:30px;\n"
"}\n"
"\n"
"#label_5\n"
"{\n"
"	color: white;\n"
"background: transparent;\n"
"	font-family: century gothic;\n"
"	font-size: 24px\n"
"}\n"
"#label_6\n"
"{\n"
"	color: white;\n"
"background: transparent;\n"
"	font-family: century gothic;\n"
"	font-size: 24px\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
""));
        centralwidget = new QWidget(loginForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 521, 601));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        stackedWidget->addWidget(page_3);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        frame = new QFrame(page);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 521, 341));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        lineEdit_login = new QLineEdit(frame);
        lineEdit_login->setObjectName(QString::fromUtf8("lineEdit_login"));
        lineEdit_login->setGeometry(QRect(60, 87, 391, 24));
        lineEdit_login->setStyleSheet(QString::fromUtf8("*{\n"
"background: transparent;\n"
"border: none;\n"
"color: #717072;\n"
"border-bottom: 1px solid  #717072;\n"
"	font-family: century gothic;\n"
"	font-size: 24px\n"
"}\n"
"\n"
""));
        lineEdit_password = new QLineEdit(frame);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setGeometry(QRect(60, 149, 391, 31));
        lineEdit_password->setStyleSheet(QString::fromUtf8("*{\n"
"background: transparent;\n"
"border: none;\n"
"color: #717072;\n"
"border-bottom: 1px solid  #717072;\n"
"	font-family: century gothic;\n"
"	font-size: 24px\n"
"}\n"
"\n"
""));
        lineEdit_password->setEchoMode(QLineEdit::Password);
        pushButton_login = new QPushButton(frame);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));
        pushButton_login->setGeometry(QRect(120, 250, 291, 51));
        pushButton_login->setStyleSheet(QString::fromUtf8("\n"
"#pushButton_login\n"
"{\n"
"	color: white;\n"
"	border-radius: 15px;\n"
"	background: red;\n"
"	font-family: century gothic;\n"
"	font-size: 24px\n"
"\n"
"}\n"
"\n"
"#pushButton_login:hover\n"
"{\n"
"	color: #333;\n"
"	border-radius: 15px;\n"
"	background: #49ebff;\n"
"	font-family: century gothic;\n"
"	font-size: 24px\n"
"\n"
"}\n"
"\n"
""));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(180, 37, 151, 20));
        minimizeButton = new QPushButton(frame);
        minimizeButton->setObjectName(QString::fromUtf8("minimizeButton"));
        minimizeButton->setGeometry(QRect(430, 0, 30, 40));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        minimizeButton->setFont(font);
        minimizeButton->setCursor(QCursor(Qt::PointingHandCursor));
        minimizeButton->setStyleSheet(QString::fromUtf8("\n"
"#minimizeButton\n"
"{\n"
"	border:none;\n"
"color:white;\n"
"}\n"
"\n"
"#minimizeButton:hover\n"
"{\n"
"	border:none;\n"
"color:#EB0000;\n"
"}"));
        closeButton = new QPushButton(frame);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(460, 0, 40, 40));
        closeButton->setFont(font);
        closeButton->setCursor(QCursor(Qt::PointingHandCursor));
        closeButton->setStyleSheet(QString::fromUtf8("#closeButton\n"
"{\n"
"	border:none;\n"
"color:white;\n"
"}\n"
"\n"
"#closeButton:hover\n"
"{\n"
"	border:none;\n"
"color:#EB0000;\n"
"}"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        label = new QLabel(page_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(79, 20, 411, 201));
        closeButton_2 = new QPushButton(page_2);
        closeButton_2->setObjectName(QString::fromUtf8("closeButton_2"));
        closeButton_2->setGeometry(QRect(470, 0, 40, 40));
        closeButton_2->setFont(font);
        closeButton_2->setCursor(QCursor(Qt::PointingHandCursor));
        closeButton_2->setStyleSheet(QString::fromUtf8("#closeButton_2\n"
"{\n"
"	border:none;\n"
"color:white;\n"
"}\n"
"\n"
"#closeButton_2:hover\n"
"{\n"
"	border:none;\n"
"color:#EB0000;\n"
"}"));
        stackedWidget->addWidget(page_2);
        loginForm->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(loginForm);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        loginForm->setStatusBar(statusbar);

        retranslateUi(loginForm);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(loginForm);
    } // setupUi

    void retranslateUi(QMainWindow *loginForm)
    {
        loginForm->setWindowTitle(QCoreApplication::translate("loginForm", "MainWindow", nullptr));
        lineEdit_login->setText(QString());
        lineEdit_login->setPlaceholderText(QCoreApplication::translate("loginForm", "Username", nullptr));
        lineEdit_password->setPlaceholderText(QCoreApplication::translate("loginForm", "Password", nullptr));
        pushButton_login->setText(QCoreApplication::translate("loginForm", "Login", nullptr));
        label_6->setText(QCoreApplication::translate("loginForm", "LOGIN HERE", nullptr));
        minimizeButton->setText(QCoreApplication::translate("loginForm", "\342\210\222", nullptr));
        closeButton->setText(QCoreApplication::translate("loginForm", "\303\227", nullptr));
        label->setText(QCoreApplication::translate("loginForm", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffffff;\">Non connected</span></p></body></html>", nullptr));
        closeButton_2->setText(QCoreApplication::translate("loginForm", "\303\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginForm: public Ui_loginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
