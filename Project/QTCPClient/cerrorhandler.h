#ifndef CERRORHANDLER_H
#define CERRORHANDLER_H
#include <QAbstractSocket>
#include <QMessageBox>

class CErrorHandler
{
public:
    CErrorHandler();
    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // CERRORHANDLER_H
