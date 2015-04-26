#ifndef SRVTHREAD_H
#define SRVTHREAD_H

#include <QThread>

class SrvThread : public QThread
{
    Q_OBJECT
public:
    explicit SrvThread(QObject *parent = 0);
    void run();

signals:

public slots:

};

#endif // SRVTHREAD_H
