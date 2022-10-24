#ifndef CGENERATEPDF_H
#define CGENERATEPDF_H

#include <QDir>
#include <QFile>

class CGeneratePdf
{
  public:
    CGeneratePdf();
    static QString generateReportInfo(QVariantList &dataList);
};

#endif // CGENERATEPDF_H
