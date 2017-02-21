#include "videogetter.h"

#include <QDebug>
#include <QFile>

#include <opencv2/highgui.hpp>
#include <opencv2/core/opengl.hpp>
#include <opencv2/cudacodec.hpp>

#include "h264streamdecoder.h"

VideoGetter::VideoGetter()
{
}

void VideoGetter::start(QString ip, quint16 port)
{
    socket.connectToHost(ip, port, QTcpSocket::ReadWrite,
                         QAbstractSocket::IPv4Protocol);

    if (socket.waitForConnected(3000)){
        socket.write(QString("VIDEO_YUV_TYPE").toLocal8Bit());
        socket.write("\n");
        socket.flush();

        readingThread = std::thread(&VideoGetter::readLoop, this);
    }

    isInterrupted = false;
}

void VideoGetter::interrupt()
{
    socket.close();
    isInterrupted = true;

    readingThread.join();
    qDebug() << "INTERRUPTED!";
}

void VideoGetter::writeLoop()
{
    /*while (socket.isOpen()){
    }*/
}

void VideoGetter::readLoop()
{
    QTcpSocket socket;
    socket.setSocketDescriptor(this->socket.socketDescriptor());

    qDebug() << "Socket readable: " << socket.isReadable();
    qDebug() << "Socket is open: " << socket.isOpen();
    qDebug() << "ReadLoop\n";
    while (!isInterrupted){
        //qDebug() << "wait for reading";
        if (socket.waitForReadyRead()){
            qDebug() << "Available: " << socket.bytesAvailable();
            int frameNumber = readInt(socket);
            qDebug() << "Frame number: " << frameNumber;

            int frameByteSize = readInt(socket);
            qDebug() << "Frame byte size: " << frameByteSize;

            QByteArray message;
            message.resize(frameByteSize);
            int received = 0;

            while (received != frameByteSize){
                int remaining = frameByteSize - received;
                if (socket.waitForReadyRead() &&
                        socket.bytesAvailable() > 4){
                    int readed = socket.read(message.data() + received, remaining);
                    received += readed;
                    qDebug() << "readed: " << readed << " received: " << received;
                }
            }


            QFile file(QString::number(frameNumber) + ".jpg");
            file.open(QFile::WriteOnly);
            file.write(message);
            file.close();
        }
    }
}

int VideoGetter::readInt(QTcpSocket &socket)
{
    int result = 0;
    //QTcpSocket socket;
    //socket.setSocketDescriptor(this->socket.socketDescriptor());

    while (socket.bytesAvailable() < 4)
        socket.waitForReadyRead(10);

    unsigned char intData[4] = {0, 0, 0, 0};
    socket.read((char*)intData, 4);

    qDebug() << "Readed int\nAvailable: " << socket.bytesAvailable();
    qDebug() << "Bytes: " << (int)intData[0] << " " << (int)intData[1] << " "
               << (int)intData[2] << " " << (int)intData[3];

    result = intData[0] << 24 |
             intData[1] << 16 |
             intData[2] << 8  |
             intData[3];

    return result;
}
