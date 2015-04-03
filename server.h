#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QMap>;

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
    void aboutToClose();

    private:
    class ConnInfo
    {
    public:
        QString client;
        unsigned int file;
        QString ip;
    };
    QMap<qintptr, ConnInfo*> *connections;
};

#endif // SERVER_H
