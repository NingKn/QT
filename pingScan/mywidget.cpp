#include "mywidget.h"
#include "ui_mywidget.h"
#include <QMessageBox>
#include <QtNetwork>


MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);



    //点击开始，进行ping测试
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(startScan()));

    //获取本地IP
    QString localIP=this->get_localmachine_ip();
    ui->label_4->setText("本地IP："+localIP);
    //ping自动c段ip地址
    QStringList localiplist=localIP.split(".");
    ui->lineEdit->setText(localiplist[0]+"."+localiplist[1]+"."+localiplist[2]+".1");
    ui->lineEdit_4->setText(localiplist[0]+"."+localiplist[1]+"."+localiplist[2]+".254");
    //窗口显示区域配置
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,120);
    //ui->tableWidget->setColumnWidth(1,120);

    QStringList header;
    header.append(QObject::tr("IP"));
    header.append(QObject::tr("Connected?"));
    header.append("");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setShowGrid(false);//此属性保存是否显示网格。
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//最后一个标题占用所有空间

    onlinecount=0;

    //窗口2显示区配置
    ui->tableWidget_2->setColumnCount(1);

    QStringList header2;
    header2.append(QObject::tr("Port?"));
    ui->tableWidget_2->setHorizontalHeaderLabels(header2);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);











}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::onCommandSuccess(QString ip)
{
    qDebug() <<"主线程槽函数执行success";
    int hostIndex=scanhosts.indexOf(ip);
    qDebug() <<"hostIndex is"<<hostIndex;
    //设置图片以及显示位置
    QTableWidgetItem *statusItem=new QTableWidgetItem();
    statusItem->setIcon(QIcon(":/img/online_icon.png"));
    ui->tableWidget->setItem(hostIndex,1,statusItem);

    onlinecount++;
    ui->online_label->setText(QString::number(onlinecount));

}

void MyWidget::onCommandFailed(QString ip)
{
    qDebug() <<"主线程槽函数执行failed";
    int hostIndex=scanhosts.indexOf(ip);
    qDebug() <<"hostIndex is"<<hostIndex;
    //设置图片以及显示位置
    QTableWidgetItem *statusItem=new QTableWidgetItem();
    statusItem->setIcon(QIcon(":/img/offline_icon.png"));
    ui->tableWidget->setItem(hostIndex,1,statusItem);
}


void MyWidget::startScan()
{
    //清空tabWidget的内容
    ui->tableWidget->clearContents();
    QString ip1=ui->lineEdit->text();
    QString ip2=ui->lineEdit_4->text();
    //切割成数组
    QStringList ip1List=ip1.split(".");
    QStringList ip2List=ip2.split(".");
    qDebug() <<"数组的值："<<ip1List;
    qDebug() <<"数组2的值："<<ip2List;

    //判断ip的范围
    if(ip1List[2].toInt()>ip2List[2].toInt())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("告警");
        msgBox.setText("范围不正确，请重新输入！");
        msgBox.exec();
        ui->lineEdit_4->setFocus();
        return;
    }else if(ip1List[2].toInt()== ip2List[2].toInt())
    {
        if(ip1List[3].toInt()> ip2List[3].toInt())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("告警");
            msgBox.setText("范围不正确，请重新输入！");
            msgBox.exec();
            ui->lineEdit_4->setFocus();
            return;
        }else
        {

        }
    }
    //判断ip的第三个参数
    if(ip1List[3].toInt()>255 || ip2List[3].toInt()>255)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("告警");
        msgBox.setText("范围不正确，请重新输入！");
        msgBox.exec();
        ui->lineEdit_4->setFocus();
        return;
    }

    onlinecount=0;

    QStringList ipRange;
    //所有ip地址放在1个数组中
    if(ip1List[2].toInt()== ip2List[2].toInt())
    {
        int startIPNumber = ip1List[3].toInt();
        int endIPNumber   = ip2List[3].toInt();
        for(int i=startIPNumber;i<=endIPNumber;++i )
        {
            QString iptest=ip1List[0] + "." + ip1List[1] + "." + ip1List[2] + "." + QString::number(i);
            ipRange << iptest;
        }
        qDebug() <<"ip的范围"<<ipRange;
    }
    this->scanhosts=ipRange;
    //多线程前，分配ip组
    QStringList ip_Thread[10];
    int ipSize=ipRange.size();//记录所有ip的个数
    int y=ipSize/10;
    int i=0;
    int j=0;
    //ip数大于10个以上，多线程分配任务，每个线程分配10个ip
    for(i=0;i<10;++i)
    {
        for(j=i*y;j<(i+1)*y;j++)
        {
            ip_Thread[i]<<ipRange[j];
        }
        qDebug() <<"ip_Thread is"<<i<<ip_Thread[i];
    }
    qDebug() <<i;
    qDebug() <<j;
    //扫描数不够10个或少于10个，执行下面的循环
    for(int z=j,thread_index=0;z<ipSize;z++)
    {
        ip_Thread[thread_index]<<ipRange[z];
        qDebug() <<"thread_index"<<ip_Thread[thread_index];
        thread_index++;
    }


    //设置显示IP以及显示位置
    ui->tableWidget->setRowCount(ipRange.size());

    for(int i=0;i<ipRange.size();++i)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(ipRange[i]));
    }
    //10个线程开始处理
    for(int i=0;i<10;i++)
    {
        thread=new mythread(this);

        connect(thread,SIGNAL(commandSuccess(QString)),this,SLOT(onCommandSuccess(QString)));
        connect(thread,SIGNAL(commandFailed(QString)),this,SLOT(onCommandFailed(QString)));
        thread->setIp(ip_Thread[i]);
        thread->start();
    }
    //扫描完成，仅提示一次完成即可
    connect(thread,SIGNAL(isDone()),this,SLOT(dealDone()));

}

QString MyWidget::get_localmachine_ip()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();//方便函数返回在主机上找到的所有IP地址
    //数字形式返回IPv4地址,是ipv4协议，不是169开头的ip地址
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()&& !ipAddressesList.at(i).toString().startsWith("169")) {
            ipAddress = ipAddressesList.at(i).toString();//已数字行驶返回地址
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return ipAddress;
}
//扫描完成后的槽函数处理
void MyWidget::dealDone()
{
    QMessageBox msg;
    msg.setWindowTitle("结果");
    msg.setText("扫描完成！");
    msg.exec();
}



