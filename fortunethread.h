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

#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
//class QTcpSocket;

class FortuneTcpSocket : public QTcpSocket
{
    enum Command{SET_RANGE = 25,
                RANDOM_CAPTURE = 61,
                CONTINUE_CAPTURE_1 = 59,
                CONTINUE_CAPTURE_2 = 60,
                STOP_CAPTURE = 56,
                SAY_BYE = 57};

    enum RangeCode{_2pot5VN = 0,  //+- 2.5v
                   _5VN,  //+-5v
                   _6pot25VN, //+-6.25v
                   _10VN, //+-10v
                   _12pot5VN, //+-12.5v
                   _5v,      //0 - 5 v
                   _10v,     //0 - 10v
                   _12pot5v  //0 - 12.5v
    };
    Q_OBJECT
public:
    FortuneTcpSocket(QObject* parent = nullptr);
    void quit();
    void setRange(RangeCode code, quint8 firstChannel, quint8 channels);
    void testCapture(quint8 channels);
    void continueCapture_1(quint8 channels, quint16 freq,
                           quint16 blockSize, quint8 quitCode = 1);
    void continueCapture_2(quint8 channels, quint16 freq,
                           quint16 blockSize = 0, quint8 blockMultiple = 1,
                           quint8 quitCode = 1);
    void stop();

    void setClockFlag(int flag){_clockFlag = flag;}
    int channels()const{return 8;}
protected slots:
    void sendCommand();
signals:
    void command();
private:
    QByteArray _outputBuffer;

    quint8 _quitCode;
    quint8 _firstChannel; //首通道
    quint8 _channels;     //通道数
    quint16 _frequence;  //频率设定
    quint16 _blockSize;
    quint8 _freqCode;  //频率调整标志
    quint8 _clockFlag;  //时钟标志
    quint8 _command;   //命令
    quint8 _gainCode; //增益码

    static const int _OUTPUTBUFFERSIZE = 20;
};

//! [0]
class FortuneThread : public QThread
{
    Q_OBJECT

public:
    FortuneThread(int _socketDescriptor, QObject *parent = nullptr);
    virtual ~FortuneThread();
    void closeClient();
    void send();
protected:
    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);
    void clientStateChanged(QString, quint16, QString);
protected slots:
    void readyRead();
public slots:
    void on_socketDisconnected();
private:
    int _socketDescriptor;
    FortuneTcpSocket *_tcpSocket = nullptr;
    QDataStream _ds;
    quint8 _output[20] = {0};
    QString _peerAddr;
    quint16 _peerPort;
};
//! [0]

#endif
