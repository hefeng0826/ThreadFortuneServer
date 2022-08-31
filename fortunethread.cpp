/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "fortunethread.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

//! [0]
FortuneThread::FortuneThread(int socketDescriptor, QObject *parent)
    : QThread(parent), _socketDescriptor(socketDescriptor)
{
}

FortuneThread::~FortuneThread()
{
 //   on_socketDisconnected();
//    closeClient();
    quit();
    wait();
    qDebug()<<"Thread Destroyed";
}

void FortuneThread::quit()
{
    if(isRunning())
        _tcpSocket->quit();
}

void FortuneThread::setRange(FortuneTcpSocket::RangeCode code, quint8 firstChannel, quint8 channels)
{
    if(isRunning())
        _tcpSocket->setRange(code, firstChannel, channels);
}

void FortuneThread::testCapture(quint8 channels)
{
    if(isRunning())
        _tcpSocket->testCapture(channels);
}

void FortuneThread::continueCapture_1(quint8 channels, quint16 freq, quint16 blockSize, quint8 quitCode)
{
    if(isRunning())
        _tcpSocket->continueCapture_1(channels, freq, blockSize, quitCode);
}

void FortuneThread::continueCapture_2(quint8 channels, quint16 freq, quint16 blockSize, quint8 blockMultiple, quint8 quitCode)
{
    if(isRunning())
        _tcpSocket->continueCapture_2(channels, freq, blockSize, blockMultiple, quitCode);
}

void FortuneThread::stop()
{
    if(isRunning())
        _tcpSocket->stop();
}

void FortuneThread::setClockFlag(int flag)
{

}

//! [0]

//! [1]
void FortuneThread::run()
{
    qDebug()<<"Starting thread";
    _tcpSocket = new FortuneTcpSocket;   //不可以使用this作为父对象

    if(!_tcpSocket->setSocketDescriptor(_socketDescriptor))
    {
        emit error(_tcpSocket->error());
        return;
    }
    _peerAddr = _tcpSocket->peerAddress().toString();
    _peerPort = _tcpSocket->peerPort();

    emit clientStateChanged(_peerAddr, _peerPort, true, _socketDescriptor);
//    _ds.setDevice(_tcpSocket);
//    _ds.setVersion(QDataStream::Qt_5_10);
//    _output[0] = 25;
//    memset(_output + 1, 1, 10);
//    _ds.writeRawData((const char*)_output, 20);

//    _output[0] = 61;
//    _output[2] = 0;
//    _output[3] = 8;
//    _ds.writeRawData((const char*)_output, 20);
//    _output[0] = 57;
//    _tcpSocket->write((const char*)_output, 20);

//    connect(_tcpSocket, &FortuneTcpSocket::stateChanged,
//            [=](QAbstractSocket::SocketState state){qDebug()<<state;});



    connect(_tcpSocket, &QTcpSocket::readyRead,
                  this, &FortuneThread::readyRead, Qt::DirectConnection);
    //不同线程的对象间的消息响应：Qt::AutoConnect = QT::QueueConnect
    /*
     * 这里使用Qt::DirectConnect 槽函数在当前线程中响应，
     * 避免了QThread::~QThread()中wait()阻塞主线程消息循环，使槽函数不得响应
     * QTcpSocket::readyRead  同理
    */
    connect(_tcpSocket, &QTcpSocket::disconnected,
            this, &FortuneThread::on_socketDisconnected, Qt::DirectConnection);
    connect(this, &FortuneThread::finished,
            _tcpSocket, &FortuneTcpSocket::deleteLater);
//    connect(_tcpSocket, &FortuneTcpSocket::disconnected,
//            [=]{emit clientStateChanged(_peerAddr, _peerPort, "Disconnected");});
    qDebug()<<_socketDescriptor<<"Client Connected";
    qDebug()<<_tcpSocket->localAddress().toString();
    qDebug()<<_tcpSocket->peerAddress().toString()<<_tcpSocket->peerPort()<<_tcpSocket->peerName();
    exec();
    qDebug()<<"Thread Finished";
}

void FortuneThread::readyRead()
{
    qDebug()<<currentThreadId()<<_tcpSocket->readAll();
    return;
    _ds.startTransaction();
    qint16 buffer[64][8] = {0};

    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            _ds>>buffer[i][j];
        }
    }

//    char buf[8] = {0};

//    int len = _ds.readRawData(buf, 8);
//    qDebug()<<"received: "<<len;

    if(!_ds.commitTransaction())
    {
        return;
    }

    for(int i = 0; i < 64; i++)
    {
        qDebug()<<buffer[i][0]<<buffer[i][1]
                <<buffer[i][2]<<buffer[i][3]
                <<buffer[i][4]<<buffer[i][5]
                <<buffer[i][6]<<buffer[i][7];
    }
}

void FortuneThread::on_socketDisconnected()
{
//    if(!isRunning()) return;
//    _tcpSocket->quit();   //采集卡关闭指令
//    _tcpSocket->deleteLater();
    qDebug()<<_socketDescriptor<<"Disconnected"<<currentThreadId();
    emit clientStateChanged(_peerAddr, _peerPort, false, _socketDescriptor);
    exit(0);
}
//! [4]

FortuneTcpSocket::FortuneTcpSocket(QObject *parent)
    :QTcpSocket(parent)
{
    _outputBuffer.resize(_OUTPUTBUFFERSIZE);
    connect(this, SIGNAL(command()),
            this, SLOT(sendCommand()));
}

FortuneTcpSocket::~FortuneTcpSocket()
{
    qDebug()<<"Fortune TcpSocket Destroyed";
}

void FortuneTcpSocket::quit()
{
    qDebug()<<"Main Thread:"<<QThread::currentThreadId();
    _outputBuffer.insert(0, char(SAY_BYE));
    emit command();
//    QThread::sleep(1);
}

void FortuneTcpSocket::setRange(RangeCode code, quint8 firstChannel, quint8 channels)
{
    if(firstChannel > 8) return;
    if(channels > 8) return;
    _outputBuffer.insert(0, char(SET_RANGE));
    _outputBuffer.insert(2 + firstChannel, channels, char(code));
    emit command();
}

void FortuneTcpSocket::continueCapture_1(quint8 channels, quint16 freq,
                                    quint16 blockSize, quint8 quitCode)
{
    _outputBuffer.clear();
    QDataStream ds(&_outputBuffer, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds<<(quint8)CONTINUE_CAPTURE_1<<quint8(0)<<quint8(0)
     <<(quint8)channels<<quitCode<<freq<<blockSize<<quint8(freq > 500 ? 2 : 1)
    <<quint8(0)<<quint8(0)<<_clockFlag;
    _outputBuffer.append(6, char(0));
    emit command();
}

void FortuneTcpSocket::continueCapture_2(quint8 channels, quint16 freq,
                                    quint16 blockSize, quint8 blockMultiple,
                                    quint8 quitCode)
{
    _outputBuffer.clear();
    QDataStream ds(&_outputBuffer, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds<<(quint8)CONTINUE_CAPTURE_1<<quint8(0)<<quint8(0)
     <<(quint8)channels<<quitCode<<freq<<blockSize<<blockMultiple
    <<quint8(0)<<quint8(0)<<_clockFlag;
    _outputBuffer.append(6, char(0));
    emit command();
}

void FortuneTcpSocket::testCapture(quint8 channels)
{
    _outputBuffer.insert(0, char(RANDOM_CAPTURE));
    _outputBuffer.insert(2, char(0));
    _outputBuffer.insert(3, char(channels));
    emit command();
}

void FortuneTcpSocket::stop()
{
    _outputBuffer.insert(0, char(STOP_CAPTURE));
    emit command();
}

void FortuneTcpSocket::sendCommand()
{
    qDebug()<<"Socket Thread"<<QThread::currentThreadId();
    write(_outputBuffer);
}
