#include "server.h"
#include <iostream>
#include <QTcpSocket>

#define SRVPORT 3567

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    connections = new QMap<qintptr, ConnInfo*>();
}

Server::~Server()
{
    for(QList<ConnInfo*>::iterator iter = connections->values().begin(); iter != connections->values().end(); iter++)
        delete *iter;
    delete connections;
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
        connect(socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
        //connect(this, SIGNAL(cmd(QString), this, SLOT(parseCmd(QString));

        ConnInfo *inf = new ConnInfo;
        inf->ip = socket->peerAddress().toString();
        connections->insert(socket->socketDescriptor(), inf);

        connections->value(socket->socketDescriptor())->client = "adadas";

        qDebug() << connections->value(socket->socketDescriptor())->client;
        qDebug() << socket->peerAddress().toString();
    }
}

void Server::aboutToClose()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << "close";
    qDebug() << socket->socketDescriptor();
    qDebug() << connections->value(socket->socketDescriptor())->client;
    qDebug() << connections->value(socket->socketDescriptor())->ip;
    qDebug() << connections->value(socket->socketDescriptor())->file;

    delete connections->value(socket->socketDescriptor());
    connections->remove(socket->socketDescriptor());
}

void Server::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << "ready to read " << socket->socketDescriptor();
    while (socket->canReadLine())
    {
        QString data = socket->readLine();
        data = data.remove(data.length()-1,1);
        qDebug() << data.toUtf8();
        if(data.split(" ")[0] == "GET") {
            qDebug() << "g: " << data.split(" ")[1];
            connections->value(socket->socketDescriptor())->client = data;
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
