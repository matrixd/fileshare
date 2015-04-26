#include "server.h"
#include <iostream>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QByteArray>

#define SRVPORT 3567

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    connections = new QMap<QTcpSocket*, ConnInfo*>();
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

        ConnInfo *inf = new ConnInfo;
        inf->ip = socket->peerAddress().toString();
        connections->insert(socket, inf);

        connections->value(socket)->client = "adadas";

        qDebug() << connections->value(socket)->client;
        qDebug() << socket->peerAddress().toString();
    }
}

void Server::write(qint64 q)
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

    //sending a file
    QByteArray fb = connections->value(socket)->file.read(1024);
    if(fb.size()){
        socket->write(fb);
        fb = connections->value(socket)->file.read(1024);
    } else {
        connections->value(socket)->file.close();
        socket->write("\r\n");
        socket->close();
    }
}

void Server::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << "ready to read " << socket->socketDescriptor();
    while (socket->canReadLine())
    {
        QString data = socket->readLine();
        qDebug() << QString(data[data.length()-1]).toInt();
        //data = data.remove(data.length()-1,1);
        for(int i = data.length()-1; i>-1; i--){
            if(!data.at(i).isSpace()){
                data = data.left(i+1);
                break;
            }
        }
        qDebug() << data.toUtf8();

        //parsing get
        if(data.split(" ")[0].compare("get", Qt::CaseInsensitive) == 0) {
            qDebug() << "g: " << data.split(" ")[1];
            QString link = data.split(" ")[1].remove('/');
            if(link.isEmpty())
                return;
            QSqlQuery query;
            //int id = QByteArray::fromBase64(link.toUtf8()).toInt();

            unsigned int id = link.toInt();
            query.exec(QString("select * from files where id = %1").arg(id));

            if(!query.next()){
                socket->abort();
                return;
            }

            connections->value(socket)->fileid = id;

            qDebug() << query.value(0).toInt();

            connections->value(socket)->file.setFileName(
                    query.value(query.record().indexOf("path")).toString());
            if (!connections->value(socket)->file.open(QIODevice::ReadOnly)){
                    socket->close();
                    qDebug() << "failed to open";
                    return;
            }

            QDateTime d = QDateTime::currentDateTime();

            QString qd = QString("Date: %1 GMT\r\n").arg(d.toString("ddd, dd MMM yyyy HH:mm:ss"));

            qDebug() << qd;

            QString s = QString("Content-Length: %1\r\n").arg(connections->value(socket)->file.size());

            QString f = QString("filename=\"%1\"\r\n\r\n").arg(
                        query.value(query.record().indexOf("fname")).toString());

            qDebug() << f;

            socket->write("HTTP/1.1 200 OK\r\n");
            socket->write(qd.toUtf8());
            socket->write("Connection: keep-alive\r\n");
            socket->write("Accept-Ranges: bytes\r\n");
            socket->write("Content-Type: application/octet-stream\r\n");
            socket->write(s.toUtf8());
            socket->write("Content-Disposition: attachment; ");
            socket->write(f.toUtf8());

            connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(write(qint64)));

        }

        //parsing user-agent
        if(data.split(" ")[0].compare("User-Agent:", Qt::CaseInsensitive) == 02) {
            qDebug() << "client: " << data.mid(12);
            connections->value(socket)->client = data;
        }

        //eof
        if(data.at(0).isSpace())
            qDebug() << "end";
        //socket->write("sadasd");
        //emit cmd(data);
    }

}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << socket;
    if(connections->value(socket)->file.isOpen())
        connections->value(socket)->file.close();
    delete connections->value(socket);
    connections->remove(socket);
    socket->deleteLater();
}
