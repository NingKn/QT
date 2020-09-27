#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>
#include "mythread.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private slots:
    void on_pushButton_clicked();
    void dealmyTread(QString ip,int port);
    void dealisDone();
    void on_pushButton_2_clicked();
    void dealClose();

signals:
    void startThread();
private:
    Ui::MyWidget *ui;
    QStringList IpStringlist;
    QStringList ipRange;
    QString Ip1;
    QString Ip2;
    QThread *thread;
    mythread *my_thread;
};

#endif // MYWIDGET_H
