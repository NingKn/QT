#include "mythread.h"



mythread::mythread(QObject *parent) : QThread(parent)
{
    my_tcpSocket=new QTcpSocket(this);

}

void mythread::setIp(QStringList ipList)
{
    this->ipList=ipList;
}


void mythread::myTarget()
{
    qDebug() <<"子线程开始了"<<mythread::currentThread();
    qDebug() <<"ipList is"<<this->ipList;
    int port;
    Flag=true;
    portList.clear();
    portList<<21<<22<<23<<25<<53<<135<<136<<137<<138<<139<<161<<88<<80<<81<<110<<161<<1025<<2049<<8080<<389<<443<<445<<1521<<3389<<3306<<1433;

        foreach (ip,ipList)
        {
            foreach(port,portList)
            {
                if(Flag==true){
                    my_tcpSocket->abort();
                    my_tcpSocket->connectToHost(ip,port);
                    if(my_tcpSocket->waitForConnected(500))
                    {
                        emit my_Tsignal(ip,port);
                        qDebug() <<"port is "<<port;
                    }
                    qDebug() <<"current ip is"<<ip;
                }else{return;}
            }
        }
    emit isDone();
}

void mythread::setFlag(bool flag)
{
    this->Flag=flag;
}
