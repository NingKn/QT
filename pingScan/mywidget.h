#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
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
    void startScan();

public slots:
    void onCommandSuccess(QString ip);
    void onCommandFailed(QString ip);
    void dealDone();

private:
    Ui::MyWidget *ui;
    mythread *thread;
    mythread *PortThread;
    QString get_localmachine_ip();

    QStringList scanhosts;
    QStringList to_scanhosts;

    int onlinecount;
};

#endif // MYWIDGET_H
