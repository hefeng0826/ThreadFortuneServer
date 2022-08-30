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

#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QHash>
#include <QHostAddress>

class QThread;
class FortuneThread;
//! [0]
class FortuneServer : public QTcpServer
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
    FortuneServer(QObject *parent = 0);
    virtual ~FortuneServer();

    void startServer();
    void closeServer();

    void send();

    QList<qintptr> updateClientId();
signals:
    void clientStateChanged(QString, quint16, bool, qintptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
private:
    QStringList fortunes;
    QHash<qintptr, FortuneThread*> _descriptorMap;
};
//! [0]

#endif
