#include "videogetter.h"

#include <chrono>

#include <QDebug>
#include <QCoreApplication>
#include <QDataStream>

VideoGetter::VideoGetter()
    : socket_type("VIDEO_JPG_TYPE"),
      ip("0.0.0.0"), port(1212)
{
    socket = nullptr;
}

VideoGetter::~VideoGetter()
{
    if (socket != nullptr){
        delete socket;
    }
}

void VideoGetter::setAddress(QString ip, quint16 port)
{
    this->ip = ip;
    this->port = port;
}

void VideoGetter::start()
{
    socket = new QTcpSocket();
    socket->connectToHost(ip, port, QTcpSocket::ReadWrite,
                         QAbstractSocket::IPv4Protocol);

    if (socket->waitForConnected(3000)){
        socket->write(socket_type.toLocal8Bit());
        socket->write("\n");
        socket->flush();

        readLoop();

        socket->close();
    }
    else{
        qDebug() << "cannot connect to " << ip << ":" << port;
    }

    emit finished();
}

void VideoGetter::interrupt()
{
    qDebug() << "VideoGetter: disconnecting";
    socket->disconnectFromHost();
}

void VideoGetter::readLoop()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;

    qDebug() << "Socket readable: " << socket->isReadable();
    qDebug() << "Socket is open: " << socket->isOpen();
    qDebug() << "ReadLoop\n";

    auto start_time = high_resolution_clock::now();

    while (socket->state() == QTcpSocket::SocketState::ConnectedState){
        if (socket->waitForReadyRead(3000)){
            QDataStream stream(socket);
            qDebug() << "Available: " << socket->bytesAvailable();

            while (socket->bytesAvailable() < 8)
                socket->waitForReadyRead();
            uint frameNumber = 0;
            uint frameByteSize = 0;
            stream >> frameNumber >> frameByteSize;
            qDebug() << "Frame number: " << frameNumber;
            qDebug() << "Frame byte size: " << frameByteSize;

            QByteArray message;
            message.resize(frameByteSize);
            uint received = 0;

            while (received != frameByteSize){
                uint remaining = frameByteSize - received;
                if (socket->waitForReadyRead()){
                    uint readed = socket->read(message.data() + received,
                                                       remaining);
                    received += readed;
                }
            }

            emit gotFrame(message, frameNumber);
        }
        else{
            qDebug() << "Not ready socket for read";
        }
        QCoreApplication::processEvents( QEventLoop::AllEvents, 5 );
    }
    auto finish_time = high_resolution_clock::now();
    qDebug() << "Receiving time: " << duration_cast<milliseconds>(finish_time - start_time)
                                      .count() << "ms";
}
