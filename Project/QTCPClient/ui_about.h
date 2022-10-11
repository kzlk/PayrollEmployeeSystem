/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QLabel *label_6;
    QLabel *label_3;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_14;
    QLabel *label_15;
    QTextEdit *textEdit;

    void setupUi(QWidget *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(1080, 600);
        About->setStyleSheet(QString::fromUtf8("#About\n"
"{\n"
"	background:#2D2D2D;\n"
"}"));
        label_6 = new QLabel(About);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(310, 0, 460, 191));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/Images/Corporate Management.png")));
        label_6->setScaledContents(true);
        label_3 = new QLabel(About);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(830, 0, 250, 250));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/Images/top_right.png")));
        label_3->setScaledContents(true);
        pushButton = new QPushButton(About);
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
        label = new QLabel(About);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 250, 250));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Images/top_left.png")));
        label->setScaledContents(true);
        label_14 = new QLabel(About);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(390, 150, 311, 31));
        QFont font1;
        font1.setPointSize(14);
        label_14->setFont(font1);
        label_15 = new QLabel(About);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(390, 180, 311, 31));
        QFont font2;
        font2.setPointSize(12);
        label_15->setFont(font2);
        textEdit = new QTextEdit(About);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setEnabled(false);
        textEdit->setGeometry(QRect(210, 240, 651, 191));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Segoe UI")});
        font3.setPointSize(14);
        textEdit->setFont(font3);
        textEdit->setStyleSheet(QString::fromUtf8("color:#fff;\n"
"border:none;\n"
"background:#2D2D2D;"));
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QWidget *About)
    {
        About->setWindowTitle(QCoreApplication::translate("About", "Form", nullptr));
        label_6->setText(QString());
        label_3->setText(QString());
        pushButton->setText(QCoreApplication::translate("About", "Back", nullptr));
        label->setText(QString());
        label_14->setText(QCoreApplication::translate("About", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">Employee Management System</span></p></body></html>", nullptr));
        label_15->setText(QCoreApplication::translate("About", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">About</span></p></body></html>", nullptr));
        textEdit->setHtml(QCoreApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:14pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">An Employee management system is designed to simplify the process of record maintenance of employees in an organisation. It helps in managing the information of employees for HR functions.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kak"
                        "u Gothic Pro,Meiryo,serif'; font-size:12pt; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; font-weight:600; color:#00aaff;\">Features of this app</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">1) Clean and User Friendly Interface</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom"
                        ":0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">2) Record can be searched/viewed just by double click</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">3) Auto generate ID</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">4) Insert/Update/Delete record just by few clicks</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-"
                        "indent:0; text-indent:0px; font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">This project is developed under the supervision of </span><span style=\" font-family:'q_serif,Georgia,Times,Times New Roman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; font-weight:600; color:#ffffff;\">Dr. Anupriya Jain</span><span style=\" font-family:'q_serif,Georgia,Times,Times New R"
                        "oman,Hiragino Kaku Gothic Pro,Meiryo,serif'; font-size:12pt; color:#ffffff;\">.</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
