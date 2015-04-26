#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QMap>
#include <QThread>
#include <QFile>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void start();
signals:

public slots:

protected:

protected slots:
    void disconnected();
    void readyRead();
    void newConnection();
    void write(qint64 q);

    private:
    class ConnInfo
    {
    public:
        QString client;
        QFile file;
        QString ip;
        unsigned int fileid;
    };
    QMap<QTcpSocket*, ConnInfo*> *connections;
};

#endif // SERVER_H
