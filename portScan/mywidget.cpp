#include "mywidget.h"
#include "ui_mywidget.h"
#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    my_thread=new mythread;
    thread=new QThread(this);
    my_thread->moveToThread(thread);

    connect(my_thread,SIGNAL(my_Tsignal(QString,int)),this,SLOT(dealmyTread(QString,int)));
    connect(this,&MyWidget::startThread,my_thread,&mythread::myTarget);
    connect(my_thread,SIGNAL(isDone()),this,SLOT(dealisDone()));
    connect(this,&MyWidget::destroyed,this,&MyWidget::dealClose);

    qDebug() <<"主线程开始了"<<QThread::currentThread();

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::dealmyTread(QString ip,int port)
{
    QString msg="Ip: "+ip+" Port "+QString::number(port)+" is Open!";
    ui->listWidget->insertItem(0,msg);
    ui->listWidget->setWrapping(false);

}

void MyWidget::dealisDone()
{
    ui->listWidget->insertItem(0,"扫描完成");
}

void MyWidget::dealClose()
{
    on_pushButton_2_clicked();
}


void MyWidget::on_pushButton_clicked()
{

    IpStringlist.clear();
    ipRange.clear();
    Ip1=ui->lineEdit->text();
    Ip2=ui->lineEdit_2->text();
    IpStringlist<<Ip1;


    QStringList Ip1List=Ip1.split(".");
    QStringList Ip2List=Ip2.split(".");
    qDebug() <<"Ip1List is"<<Ip1List;
    qDebug() <<"Ip2List is"<<Ip2List;

    int i;
    int j=Ip2List[3].toInt();
    for(i=Ip1List[3].toInt();i<=j;i++)
    {
        QString addIp=Ip1List[0]+"."+Ip1List[1]+"."+Ip1List[2]+"."+QString::number(i);
        ipRange<<addIp;
    }
    qDebug() <<"ipRange"<<ipRange;



    ui->listWidget->clear();
    ui->listWidget->insertItem(0,"扫描开始");

    qDebug() <<"IpStringlist is"<<IpStringlist;

    my_thread->setIp(ipRange);
    thread->start();

    emit startThread();
}

void MyWidget::on_pushButton_2_clicked()
{
    my_thread->setFlag(false);
    thread->quit();
    thread->wait();
}


