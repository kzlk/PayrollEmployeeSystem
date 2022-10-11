/********************************************************************************
** Form generated from reading UI file 'techused.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TECHUSED_H
#define UI_TECHUSED_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TechUsed
{
public:
    QPushButton *pushButton;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;

    void setupUi(QWidget *TechUsed)
    {
        if (TechUsed->objectName().isEmpty())
            TechUsed->setObjectName(QString::fromUtf8("TechUsed"));
        TechUsed->resize(1080, 600);
        TechUsed->setStyleSheet(QString::fromUtf8("#TechUsed\n"
"{\n"
"	background:#2D2D2D;\n"
"}"));
        pushButton = new QPushButton(TechUsed);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(450, 510, 180, 36));
        QFont font;
        font.setPointSize(11);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("#pushButton\n"
"{\n"
"	border:none;\n"
"	background:#F33253;\n"
"	color:white;\n"
"	border-radius:16px;\n"
"}\n"
"\n"
"#pushButton:hover\n"
"{\n"
"	border:2px solid #F33253;\n"
"	background:#2D2D2D;\n"
"	color:#F33253;\n"
"	border-radius:16px;\n"
"}"));
        label_6 = new QLabel(TechUsed);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(310, 0, 460, 191));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/Images/Corporate Management.png")));
        label_6->setScaledContents(true);
        label_3 = new QLabel(TechUsed);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(830, 0, 250, 250));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/Images/top_right.png")));
        label_3->setScaledContents(true);
        label = new QLabel(TechUsed);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 250, 250));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Images/top_left.png")));
        label->setScaledContents(true);
        label_2 = new QLabel(TechUsed);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(180, 250, 115, 115));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/Images/cpp.png")));
        label_4 = new QLabel(TechUsed);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(330, 250, 115, 115));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/Images/qt.png")));
        label_5 = new QLabel(TechUsed);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(630, 250, 115, 115));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/Images/html.png")));
        label_7 = new QLabel(TechUsed);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(480, 260, 115, 94));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/Images/mysql.png")));
        label_7->setScaledContents(true);
        label_8 = new QLabel(TechUsed);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(780, 250, 115, 115));
        label_8->setPixmap(QPixmap(QString::fromUtf8(":/Images/css.png")));
        label_9 = new QLabel(TechUsed);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(160, 370, 151, 31));
        QFont font1;
        font1.setPointSize(14);
        label_9->setFont(font1);
        label_10 = new QLabel(TechUsed);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(320, 370, 151, 31));
        label_10->setFont(font1);
        label_11 = new QLabel(TechUsed);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(460, 370, 151, 31));
        label_11->setFont(font1);
        label_12 = new QLabel(TechUsed);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(600, 370, 151, 31));
        label_12->setFont(font1);
        label_13 = new QLabel(TechUsed);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(750, 370, 151, 31));
        label_13->setFont(font1);
        label_14 = new QLabel(TechUsed);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(390, 150, 311, 31));
        label_14->setFont(font1);
        label_15 = new QLabel(TechUsed);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(390, 180, 311, 31));
        QFont font2;
        font2.setPointSize(12);
        label_15->setFont(font2);

        retranslateUi(TechUsed);

        QMetaObject::connectSlotsByName(TechUsed);
    } // setupUi

    void retranslateUi(QWidget *TechUsed)
    {
        TechUsed->setWindowTitle(QCoreApplication::translate("TechUsed", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("TechUsed", "Back", nullptr));
        label_6->setText(QString());
        label_3->setText(QString());
        label->setText(QString());
        label_2->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        label_7->setText(QString());
        label_8->setText(QString());
        label_9->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">C++ Language</span></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">Qt Framework</span></p></body></html>", nullptr));
        label_11->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">MySQL DB</span></p></body></html>", nullptr));
        label_12->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">HTML</span></p></body></html>", nullptr));
        label_13->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">CSS</span></p></body></html>", nullptr));
        label_14->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">Employee Management System</span></p></body></html>", nullptr));
        label_15->setText(QCoreApplication::translate("TechUsed", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">Technologies Used</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TechUsed: public Ui_TechUsed {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TECHUSED_H
