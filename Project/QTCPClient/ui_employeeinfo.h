/********************************************************************************
** Form generated from reading UI file 'employeeinfo.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYEEINFO_H
#define UI_EMPLOYEEINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EmployeeInfo
{
public:
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *pushButton;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *wr;
    QLabel *eeqwe;
    QLabel *ewr;
    QLabel *Phone;
    QLabel *ebfghf;
    QLabel *werewr;
    QLabel *showEmpName_2;
    QLabel *showEmpFather_2;
    QLabel *showEmpGender_2;
    QLabel *showEmpEmail_2;
    QLabel *showEmpPhone_2;
    QLabel *showEmpName_3;
    QLabel *showEmpName_4;
    QLabel *showEmpName_5;
    QLabel *showEmpName_6;
    QLabel *showEmpName_7;
    QLabel *showEmpName_8;
    QLabel *showEmpName;
    QLabel *showEmpDOB;
    QLabel *showEmpFather;
    QLabel *showEmpPhone;
    QLabel *showEmpEmail;
    QLabel *showEmpAddress;
    QLabel *showEmpName_15;
    QLabel *showEmpName_16;
    QLabel *showEmpName_17;
    QLabel *showEmpName_18;
    QLabel *showEmpName_19;
    QLabel *dept;
    QLabel *design;
    QLabel *doj;
    QLabel *salary;
    QLabel *type;

    void setupUi(QWidget *EmployeeInfo)
    {
        if (EmployeeInfo->objectName().isEmpty())
            EmployeeInfo->setObjectName(QString::fromUtf8("EmployeeInfo"));
        EmployeeInfo->resize(1080, 600);
        EmployeeInfo->setStyleSheet(QString::fromUtf8("#EmployeeInfo\n"
"{\n"
"	background:#2D2D2D;\n"
"}"));
        label = new QLabel(EmployeeInfo);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(-1, 540, 71, 61));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Images/bottom.png")));
        label->setScaledContents(true);
        label_3 = new QLabel(EmployeeInfo);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1029, 559, 51, 41));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/Images/bottom right.png")));
        label_3->setScaledContents(true);
        label_2 = new QLabel(EmployeeInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(400, -5, 280, 50));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/Images/top_bar.png")));
        label_2->setScaledContents(true);
        label_4 = new QLabel(EmployeeInfo);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(120, 90, 400, 260));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/Images/personal_card.png")));
        label_4->setScaledContents(true);
        label_5 = new QLabel(EmployeeInfo);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(560, 90, 400, 260));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/Images/company_card.png")));
        label_5->setScaledContents(true);
        label_6 = new QLabel(EmployeeInfo);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(410, 530, 331, 101));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/Images/Corporate Management.png")));
        label_6->setScaledContents(true);
        pushButton = new QPushButton(EmployeeInfo);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(450, 380, 180, 36));
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
        label_7 = new QLabel(EmployeeInfo);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(430, 0, 221, 34));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8("color:white;"));
        label_8 = new QLabel(EmployeeInfo);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(130, 92, 381, 40));
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        label_8->setFont(font2);
        label_8->setStyleSheet(QString::fromUtf8("#label_8\n"
"{\n"
"	color:white;\n"
"}"));
        label_9 = new QLabel(EmployeeInfo);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(570, 92, 381, 40));
        label_9->setFont(font2);
        label_9->setStyleSheet(QString::fromUtf8("#label_9\n"
"{\n"
"	color:white;\n"
"}"));
        wr = new QLabel(EmployeeInfo);
        wr->setObjectName(QString::fromUtf8("wr"));
        wr->setGeometry(QRect(150, 150, 91, 21));
        wr->setFont(font2);
        wr->setStyleSheet(QString::fromUtf8("color:white;"));
        eeqwe = new QLabel(EmployeeInfo);
        eeqwe->setObjectName(QString::fromUtf8("eeqwe"));
        eeqwe->setGeometry(QRect(150, 180, 101, 21));
        eeqwe->setFont(font2);
        eeqwe->setStyleSheet(QString::fromUtf8("color:white;"));
        ewr = new QLabel(EmployeeInfo);
        ewr->setObjectName(QString::fromUtf8("ewr"));
        ewr->setGeometry(QRect(150, 210, 91, 21));
        ewr->setFont(font2);
        ewr->setStyleSheet(QString::fromUtf8("color:white;"));
        Phone = new QLabel(EmployeeInfo);
        Phone->setObjectName(QString::fromUtf8("Phone"));
        Phone->setGeometry(QRect(150, 240, 91, 21));
        Phone->setFont(font2);
        Phone->setStyleSheet(QString::fromUtf8("color:white;"));
        ebfghf = new QLabel(EmployeeInfo);
        ebfghf->setObjectName(QString::fromUtf8("ebfghf"));
        ebfghf->setGeometry(QRect(150, 270, 91, 21));
        ebfghf->setFont(font2);
        ebfghf->setStyleSheet(QString::fromUtf8("color:white;"));
        werewr = new QLabel(EmployeeInfo);
        werewr->setObjectName(QString::fromUtf8("werewr"));
        werewr->setGeometry(QRect(150, 300, 91, 21));
        werewr->setFont(font2);
        werewr->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_2 = new QLabel(EmployeeInfo);
        showEmpName_2->setObjectName(QString::fromUtf8("showEmpName_2"));
        showEmpName_2->setGeometry(QRect(590, 150, 95, 21));
        showEmpName_2->setFont(font2);
        showEmpName_2->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpFather_2 = new QLabel(EmployeeInfo);
        showEmpFather_2->setObjectName(QString::fromUtf8("showEmpFather_2"));
        showEmpFather_2->setGeometry(QRect(590, 210, 111, 21));
        showEmpFather_2->setFont(font2);
        showEmpFather_2->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpGender_2 = new QLabel(EmployeeInfo);
        showEmpGender_2->setObjectName(QString::fromUtf8("showEmpGender_2"));
        showEmpGender_2->setGeometry(QRect(590, 180, 95, 21));
        showEmpGender_2->setFont(font2);
        showEmpGender_2->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpEmail_2 = new QLabel(EmployeeInfo);
        showEmpEmail_2->setObjectName(QString::fromUtf8("showEmpEmail_2"));
        showEmpEmail_2->setGeometry(QRect(590, 270, 95, 21));
        showEmpEmail_2->setFont(font2);
        showEmpEmail_2->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpPhone_2 = new QLabel(EmployeeInfo);
        showEmpPhone_2->setObjectName(QString::fromUtf8("showEmpPhone_2"));
        showEmpPhone_2->setGeometry(QRect(590, 240, 95, 21));
        showEmpPhone_2->setFont(font2);
        showEmpPhone_2->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_3 = new QLabel(EmployeeInfo);
        showEmpName_3->setObjectName(QString::fromUtf8("showEmpName_3"));
        showEmpName_3->setGeometry(QRect(250, 150, 16, 21));
        showEmpName_3->setFont(font2);
        showEmpName_3->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_4 = new QLabel(EmployeeInfo);
        showEmpName_4->setObjectName(QString::fromUtf8("showEmpName_4"));
        showEmpName_4->setGeometry(QRect(250, 180, 16, 21));
        showEmpName_4->setFont(font2);
        showEmpName_4->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_5 = new QLabel(EmployeeInfo);
        showEmpName_5->setObjectName(QString::fromUtf8("showEmpName_5"));
        showEmpName_5->setGeometry(QRect(250, 210, 16, 21));
        showEmpName_5->setFont(font2);
        showEmpName_5->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_6 = new QLabel(EmployeeInfo);
        showEmpName_6->setObjectName(QString::fromUtf8("showEmpName_6"));
        showEmpName_6->setGeometry(QRect(250, 240, 16, 21));
        showEmpName_6->setFont(font2);
        showEmpName_6->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_7 = new QLabel(EmployeeInfo);
        showEmpName_7->setObjectName(QString::fromUtf8("showEmpName_7"));
        showEmpName_7->setGeometry(QRect(250, 270, 16, 21));
        showEmpName_7->setFont(font2);
        showEmpName_7->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_8 = new QLabel(EmployeeInfo);
        showEmpName_8->setObjectName(QString::fromUtf8("showEmpName_8"));
        showEmpName_8->setGeometry(QRect(250, 300, 16, 21));
        showEmpName_8->setFont(font2);
        showEmpName_8->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName = new QLabel(EmployeeInfo);
        showEmpName->setObjectName(QString::fromUtf8("showEmpName"));
        showEmpName->setGeometry(QRect(260, 151, 231, 21));
        showEmpName->setFont(font);
        showEmpName->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpDOB = new QLabel(EmployeeInfo);
        showEmpDOB->setObjectName(QString::fromUtf8("showEmpDOB"));
        showEmpDOB->setGeometry(QRect(260, 181, 231, 21));
        showEmpDOB->setFont(font);
        showEmpDOB->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpFather = new QLabel(EmployeeInfo);
        showEmpFather->setObjectName(QString::fromUtf8("showEmpFather"));
        showEmpFather->setGeometry(QRect(260, 211, 231, 21));
        showEmpFather->setFont(font);
        showEmpFather->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpPhone = new QLabel(EmployeeInfo);
        showEmpPhone->setObjectName(QString::fromUtf8("showEmpPhone"));
        showEmpPhone->setGeometry(QRect(260, 241, 231, 21));
        showEmpPhone->setFont(font);
        showEmpPhone->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpEmail = new QLabel(EmployeeInfo);
        showEmpEmail->setObjectName(QString::fromUtf8("showEmpEmail"));
        showEmpEmail->setGeometry(QRect(260, 271, 231, 21));
        showEmpEmail->setFont(font);
        showEmpEmail->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpAddress = new QLabel(EmployeeInfo);
        showEmpAddress->setObjectName(QString::fromUtf8("showEmpAddress"));
        showEmpAddress->setGeometry(QRect(260, 301, 231, 21));
        showEmpAddress->setFont(font);
        showEmpAddress->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_15 = new QLabel(EmployeeInfo);
        showEmpName_15->setObjectName(QString::fromUtf8("showEmpName_15"));
        showEmpName_15->setGeometry(QRect(705, 150, 16, 21));
        showEmpName_15->setFont(font2);
        showEmpName_15->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_16 = new QLabel(EmployeeInfo);
        showEmpName_16->setObjectName(QString::fromUtf8("showEmpName_16"));
        showEmpName_16->setGeometry(QRect(705, 180, 16, 21));
        showEmpName_16->setFont(font2);
        showEmpName_16->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_17 = new QLabel(EmployeeInfo);
        showEmpName_17->setObjectName(QString::fromUtf8("showEmpName_17"));
        showEmpName_17->setGeometry(QRect(705, 210, 16, 21));
        showEmpName_17->setFont(font2);
        showEmpName_17->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_18 = new QLabel(EmployeeInfo);
        showEmpName_18->setObjectName(QString::fromUtf8("showEmpName_18"));
        showEmpName_18->setGeometry(QRect(705, 240, 16, 21));
        showEmpName_18->setFont(font2);
        showEmpName_18->setStyleSheet(QString::fromUtf8("color:white;"));
        showEmpName_19 = new QLabel(EmployeeInfo);
        showEmpName_19->setObjectName(QString::fromUtf8("showEmpName_19"));
        showEmpName_19->setGeometry(QRect(705, 270, 16, 21));
        showEmpName_19->setFont(font2);
        showEmpName_19->setStyleSheet(QString::fromUtf8("color:white;"));
        dept = new QLabel(EmployeeInfo);
        dept->setObjectName(QString::fromUtf8("dept"));
        dept->setGeometry(QRect(719, 151, 211, 21));
        dept->setFont(font);
        dept->setStyleSheet(QString::fromUtf8("color:white;"));
        design = new QLabel(EmployeeInfo);
        design->setObjectName(QString::fromUtf8("design"));
        design->setGeometry(QRect(719, 181, 211, 21));
        design->setFont(font);
        design->setStyleSheet(QString::fromUtf8("color:white;"));
        doj = new QLabel(EmployeeInfo);
        doj->setObjectName(QString::fromUtf8("doj"));
        doj->setGeometry(QRect(719, 211, 211, 21));
        doj->setFont(font);
        doj->setStyleSheet(QString::fromUtf8("color:white;"));
        salary = new QLabel(EmployeeInfo);
        salary->setObjectName(QString::fromUtf8("salary"));
        salary->setGeometry(QRect(719, 241, 211, 21));
        salary->setFont(font);
        salary->setStyleSheet(QString::fromUtf8("color:white;"));
        type = new QLabel(EmployeeInfo);
        type->setObjectName(QString::fromUtf8("type"));
        type->setGeometry(QRect(719, 271, 211, 21));
        type->setFont(font);
        type->setStyleSheet(QString::fromUtf8("color:white;"));

        retranslateUi(EmployeeInfo);

        QMetaObject::connectSlotsByName(EmployeeInfo);
    } // setupUi

    void retranslateUi(QWidget *EmployeeInfo)
    {
        EmployeeInfo->setWindowTitle(QCoreApplication::translate("EmployeeInfo", "Form", nullptr));
        label->setText(QString());
        label_3->setText(QString());
        label_2->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        label_6->setText(QString());
        pushButton->setText(QCoreApplication::translate("EmployeeInfo", "Back", nullptr));
        label_7->setText(QString());
        label_8->setText(QCoreApplication::translate("EmployeeInfo", "<html><head/><body><p align=\"center\">Employee Info</p></body></html>", nullptr));
        label_9->setText(QCoreApplication::translate("EmployeeInfo", "<html><head/><body><p align=\"center\">Company Info</p></body></html>", nullptr));
        wr->setText(QCoreApplication::translate("EmployeeInfo", "Name", nullptr));
        eeqwe->setText(QCoreApplication::translate("EmployeeInfo", "Date of Birth ", nullptr));
        ewr->setText(QCoreApplication::translate("EmployeeInfo", "Father ", nullptr));
        Phone->setText(QCoreApplication::translate("EmployeeInfo", "Phone ", nullptr));
        ebfghf->setText(QCoreApplication::translate("EmployeeInfo", "Email ", nullptr));
        werewr->setText(QCoreApplication::translate("EmployeeInfo", "Address", nullptr));
        showEmpName_2->setText(QCoreApplication::translate("EmployeeInfo", "Department", nullptr));
        showEmpFather_2->setText(QCoreApplication::translate("EmployeeInfo", "Date of Joining", nullptr));
        showEmpGender_2->setText(QCoreApplication::translate("EmployeeInfo", "Designation", nullptr));
        showEmpEmail_2->setText(QCoreApplication::translate("EmployeeInfo", "Type", nullptr));
        showEmpPhone_2->setText(QCoreApplication::translate("EmployeeInfo", "Salary", nullptr));
        showEmpName_3->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_4->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_5->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_6->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_7->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_8->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName->setText(QString());
        showEmpDOB->setText(QString());
        showEmpFather->setText(QString());
        showEmpPhone->setText(QString());
        showEmpEmail->setText(QString());
        showEmpAddress->setText(QString());
        showEmpName_15->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_16->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_17->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_18->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        showEmpName_19->setText(QCoreApplication::translate("EmployeeInfo", ":", nullptr));
        dept->setText(QString());
        design->setText(QString());
        doj->setText(QString());
        salary->setText(QString());
        type->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class EmployeeInfo: public Ui_EmployeeInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYEEINFO_H
