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

#include "fortuneserver.h"
#include "fortunethread.h"

#include <QRandomGenerator>
#include <QDebug>

#include <stdlib.h>

//! [0]
FortuneServer::FortuneServer(QObject *parent)
    : QTcpServer(parent)
{
    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");
}

FortuneServer::~FortuneServer()
{
    closeServer();
    qDebug()<<"Server Destroy";
}

void FortuneServer::startServer()
{
    if(!this->listen(QHostAddress::Any, 3333))
    {
        qDebug()<<"Could not start server";
    }
    else
    {
        qDebug()<<"Listening...";
    }
}

void FortuneServer::closeServer()
{
    close();
    QHashIterator<qintptr, FortuneThread*> it(_descriptorMap);
    while (it.hasNext()) {
        it.next();
        if(it.value() != nullptr)
            it.value()->on_socketDisconnected();
    }
    _descriptorMap.clear();
}

QList<qintptr> FortuneServer::updateClientId()
{
    return _descriptorMap.keys();
}
//! [0]

//! [1]
void FortuneServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<socketDescriptor;

    //不使用this作为parent object：关闭服务前需要先关闭客户端的socket连接
    FortuneThread *thread = new FortuneThread(socketDescriptor, nullptr);
    _descriptorMap.insert(socketDescriptor, thread);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, &FortuneThread::finished,
            [=]{qDebug()<<"remove"<<socketDescriptor;
        _descriptorMap.remove(socketDescriptor);
        qDebug()<<_descriptorMap.count()<<_descriptorMap[socketDescriptor];
    });
    connect(thread, &FortuneThread::clientStateChanged,
            this, &FortuneServer::clientStateChanged);

    thread->start();
}
//! [1]
