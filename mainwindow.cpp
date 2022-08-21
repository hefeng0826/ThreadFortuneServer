#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fortuneserver.h"
#include <QToolBar>
#include <QLabel>
#include <QNetworkInterface>
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

    ui->toolBar->addWidget(new QLabel(QString::fromLocal8Bit("Ö÷»úIP:")));
    ui->toolBar->addWidget(ui->comboBoxHostIP);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(ui->spinBoxPort);
    ui->spinBoxPort->setMaximum(10000);
    ui->spinBoxPort->setValue(3333);
    ui->spinBoxPort->setPrefix(QString::fromLocal8Bit("¶Ë¿Ú:"));

    connect(_server, &FortuneServer::newConnection,
            this, &MainWindow::on_newClientConnected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newClientConnected()
{

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
