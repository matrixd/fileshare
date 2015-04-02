#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    void start();
signals:

public slots:

protected:

protected slots:
    void disconnected();
    void readyRead();
    void newConnection();
};

#endif // SERVER_H
