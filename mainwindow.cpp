#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fortuneserver.h"
#include <QToolBar>
#include <QLabel>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,_server(new FortuneServer(this))
{
    ui->setupUi(this);

    QListIterator<QNetworkInterface> it = QNetworkInterface::allInterfaces();
    while(it.hasNext())
    {
        QNetworkInterface interface = it.next();
        qDebug()<<interface.humanReadableName()<<interface.flags();

        if(interface.type() & QNetworkInterface::Ethernet)
        {
            QListIterator<QNetworkAddressEntry> entryIt = interface.addressEntries();
            while (entryIt.hasNext()) {
                QNetworkAddressEntry entry = entryIt.next();
                if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    ui->comboBoxHostIP->addItem(entry.ip().toString());
                }
            }
        }
    }

    ui->toolBar->addWidget(new QLabel(QString::fromLocal8Bit("主机IP:")));
    ui->toolBar->addWidget(ui->comboBoxHostIP);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(ui->spinBoxPort);
    ui->spinBoxPort->setMaximum(10000);
    ui->spinBoxPort->setValue(3333);
    ui->spinBoxPort->setPrefix(QString::fromLocal8Bit("端口:"));

    connect(_server, &FortuneServer::clientStateChanged,
            this, &MainWindow::on_clientStateChanged);

//    connect(_server, &FortuneServer::newConnection,
//            this, &MainWindow::on_clientStateChanged);
}

MainWindow::~MainWindow()
{
    _server->closeServer();  //强关闭服务
    delete ui;
}

void MainWindow::on_clientStateChanged(QString peerAddr, quint16 peerPort,
                                       bool state, qintptr descriptr)
{
    //delete ui 会报错
    if(state)
    {
        _descriptors.insert(descriptr);
    }
    else
        _descriptors.remove(descriptr);
    QString strInfo = QString("%1:%2-%3(%4)").arg(peerAddr)
            .arg(peerPort).arg(state ? "Connected" : "Disconnected").arg(descriptr);
    ui->statusbar->showMessage(strInfo);
}

void MainWindow::on_actConnect_toggled(bool arg1)
{
    _server->closeServer();
    if(arg1)
    {
        QHostAddress addr(ui->comboBoxHostIP->currentText());

        if(!_server->listen(addr, ui->spinBoxPort->value()))
        {
            return;
        }

        ui->comboBoxHostIP->setEnabled(false);
        qDebug()<<_server->serverAddress().toString()<<_server->serverPort();
    }
    else
    {
        ui->comboBoxHostIP->setEnabled(true);
    }
}
