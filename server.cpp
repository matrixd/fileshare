#include "server.h"
#include <iostream>
#include <QTcpSocket>

#define SRVPORT 3567

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::start()
{
    if(this->listen(QHostAddress::Any, SRVPORT))
    {
        std::cout << "server is running on " << SRVPORT << " port" << std::endl;
    } else {
        std::cout << "failed to run a server on port " << SRVPORT << std::endl;
        //std::cout << this->errorString().toUtf8() << endl;
        exit(-1);
    }
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::newConnection()
{
    while(this->hasPendingConnections())
    {
        QTcpSocket *socket = this->nextPendingConnection();
        qDebug() << "new connection " << socket->socketDescriptor();
        connect(socket, SIGNAL(readyRead()), this,  SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this,  SLOT(disconnected()));
        //connect(this, SIGNAL(cmd(QString), this, SLOT(parseCmd(QString));

        qDebug() << socket->peerAddress().toString();
    }
}

void Server::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << "ready to read " << socket->socketDescriptor();
    while (socket->canReadLine())
    {
        QString data = socket->readLine();
        qDebug() << data.toUtf8();
        if(data.split(" ")[0] == "GET") {
            qDebug() << "g: " << data.split(" ")[1];
        }
        if(data == "\n")
            qDebug() << "end";
        //socket->write("sadasd");
        //emit cmd(data);
    }

}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->deleteLater();
}
