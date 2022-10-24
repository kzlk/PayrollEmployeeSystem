#include "cgeneratepdf.h"

CGeneratePdf::CGeneratePdf()
{
}

QString CGeneratePdf::generateReportInfo(QVariantList &dataList)
{
    QString html{};
    QFile file(QDir::currentPath() + "/../QTCPServer/for_pdf.htm");
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Cannot open file for reading");
        return html;
    }

    if (dataList.empty())
        return html;

    if (dataList.size() < 18)
        return html;

    QByteArray data = file.readAll();
    file.close();
    html = QString::fromUtf8(data).arg(
        dataList.at(0).toString(), dataList.at(1).toString(),
        dataList.at(2).toString(), dataList.at(3).toString(),
        dataList.at(4).toString(), dataList.at(5).toString(),
        dataList.at(6).toString(), dataList.at(7).toString(),
        dataList.at(8).toString(), dataList.at(9).toString(),
        dataList.at(10).toString(), dataList.at(11).toString(),
        dataList.at(12).toString(), dataList.at(13).toString(),
        dataList.at(14).toString(), dataList.at(15).toString(),
        dataList.at(16).toString(), dataList.at(17).toString(),
        dataList.at(18).toString());

    return html;
}
