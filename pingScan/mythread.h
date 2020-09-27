#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QDebug>

class mythread : public QThread
{
    Q_OBJECT
public:
    explicit mythread(QObject *parent = 0);
    void run();
    QStringList ipRange;

signals:
    void commandSuccess(QString ip);
    void commandFailed(QString ip);
    void isDone();

public slots:
    void setIp(QStringList ipRange);
};

#endif // MYTHREAD_H
