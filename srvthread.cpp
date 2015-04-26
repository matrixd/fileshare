#include "srvthread.h"
#include "server.h"

SrvThread::SrvThread(QObject *parent) :
    QThread(parent)
{
}

void SrvThread::run()
{
    Server* srv = new Server(this);

    exec();
}
