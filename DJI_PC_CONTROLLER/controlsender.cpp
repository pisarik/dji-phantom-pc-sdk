#include "controlsender.h"

#include <QDataStream>

ControlSender::ControlSender()
    : socket_type("CONTROL_TYPE"),
     ip("0.0.0.0"), port(1212)
{
    socket = nullptr;
}

ControlSender::~ControlSender()
{
    if (socket != nullptr){
        delete socket;
    }
}

void ControlSender::setAddress(QString ip, quint16 port)
{
    this->ip = ip;
    this->port = port;
}

void ControlSender::start()
{
    socket = new QTcpSocket();
    socket->connectToHost(ip, port, QTcpSocket::ReadWrite,
                         QAbstractSocket::IPv4Protocol);

    if (socket->waitForConnected(3000)){
        socket->write(socket_type.toLocal8Bit());
        socket->write("\n");
        socket->flush();
    }
    else{
        qDebug() << "cannot connect to " << ip << ":" << port;
        emit finished();
    }
}

void ControlSender::interrupt()
{
    qDebug() << "ControlSender: disconnecting";

    socket->disconnectFromHost();
    emit finished();
}

void ControlSender::sendCommand(Direction dir, double velocity)
{
    if (isSocketGood()){
        QDataStream stream(socket);

        qDebug() << "Direction: " << (int)char(dir);
        qDebug() << "Velocity: " << velocity;
        stream << (char)dir << velocity;
        socket->flush();
    }
}
