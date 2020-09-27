#include "mythread.h"
#include <QtCore>
#include <QMessageBox>

mythread::mythread(QObject *parent) : QThread(parent)
{

}

void mythread::setIp(QStringList ipRange)
{
    this->ipRange=ipRange;
}

void mythread::run()
{
    QString ip;

    foreach (ip, ipRange) {
        int exitCode;
        #ifdef Q_OS_WIN
            QString strArg = "ping " + ip + " -w 900 -n 2";
            exitCode = QProcess::execute(strArg);
        #else
            exitCode = QProcess::execute("ping", QStringList() << " -w 900 -n 2"<< ip);
        #endif

             if (0 == exitCode) {
                 // it's alive
                 qDebug() << "shell ping " + ip + " success";
                 emit commandSuccess(ip);
             } else {
                 qDebug() << "shell ping " + ip + " failed";
                 emit commandFailed(ip);
             }
    }

    emit isDone();
    qDebug() <<"ping"<<ipRange;

}

