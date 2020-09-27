#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>


class mythread : public QThread
{
    Q_OBJECT
public:
    explicit mythread(QObject *parent = 0);
    void setIp(QStringList ipList);
    void setFlag(bool flag);

signals:
    void my_Tsignal(QString ip,int port);
    void isDone();

public slots:
    void myTarget();

private:
    QTcpSocket *my_tcpSocket;
    QStringList ipList;
    QString ip;
    QList<int> portList;
    bool Flag;
};

#endif // MYTHREAD_H
