#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QSqlError>
#include <QString>
#include <QWidget>
#include <QtSql/QSqlDatabase>
// network
#include <QAbstractSocket>
#include <QHostAddress>
#include <QTcpSocket>

#include "CMessage.h"
#include "cfoldersetting.h"
#include <QTableWidget>
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint mousePoint;

  signals:

    void sendHeader(quint8 header, QVariantList list = {});

  private slots:
    void on_closeButton_clicked();

    void loadTotalData(int lastID, QString dept, QString designTtl);

    void on_minimizeButton_clicked();

    void on_pushButton_clicked();

    void on_searchButton_clicked();

    void on_addEmpButton_clicked();

    void on_updateEmpButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_deleteEmpButton_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_searchTextBox_returnPressed();

    void on_pushButton_6_clicked();

    void selectedPushButton(QPushButton *button);

    void deselectedPushButton(QPushButton *button);

    void on_techButton_clicked();

    void readSocket();

    void on_empDept_currentTextChanged(const QString &arg1);

    void on_button_applyAutoPilot_clicked();

    void on_settingsButton_clicked();

    void appanedDataToMainTable(QDataStream &socketStream);

    void appendDataToDeleteTable(QDataStream &socketStream);

    void appendDataToUpdatePage(QDataStream &socketStream);

  public slots:
    void receiveSocket(QTcpSocket *socket, quint64 &myuniqueId);

  private:
    Ui::MainWindow *ui;
    bool isMouseDown = false;
    QTcpSocket *socket{};

    QMap<int, QString> idEmp;
    CFolderSetting folderSetting{};
    msg::header hd{};
    msg::status st{};
    QString folderPath{};
    quint64 myUniqueID{};

    void sendHeaderToServer(quint8 header, QVariantList list = {});

    void sendDempNameForDesign(QString deptname);

    void appendDataToReportPagePeriod(QDataStream &socketStream);

    void appendDataToDetailReportPage(QDataStream &socketStream);

    void appendDataToEmploeeLabel(QVariantList list);

    void setDataInSettingWindow(QVariant start = {}, QVariant end = {},
                                QVariant next = {}, QVariant freq = {},
                                bool autopilot = {});

    void appendDataToLineEditUpdate(QVariantList list);

    bool checkFolder(QString &path);

    void savePdf(QString &html, QString &pdfName);

    /*AutoPayment*/
    QDateTime calculateStartPayPeriod(QDateTime datetime, QString box);
    QDateTime calculateNextPaymentPeriod(QDateTime datetime, QString box);
    qint8 converAutoPilotToInteger(QString box);
    qint8 converFrequencyToInteger(QString box);
    QTimer *timer{};

  private slots:

    void on_reportButton_clicked();
    void on_tableWidget_payment_period_doubleClicked(const QModelIndex &index);
    void on_biutton_back_period_clicked();
    void on_biutton_save_pdf_clicked();
    void on_btn_changeFolder_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_button_back_info_emp_clicked();
    void on_tableWidget_delete_doubleClicked(const QModelIndex &index);
    void on_button_refresh_clicked();
    void on_tableWidget_update_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
