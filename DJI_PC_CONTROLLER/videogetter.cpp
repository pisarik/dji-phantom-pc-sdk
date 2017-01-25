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
    QFile file(QString("video.mp4"));
    file.open(QFile::WriteOnly);
    QTcpSocket socket;
    socket.setSocketDescriptor(this->socket.socketDescriptor());
    qDebug() << "Socket readable: " << socket.isReadable();
    qDebug() << "Socket is open: " << socket.isOpen();
    qDebug() << "ReadLoop\n";
    while (!isInterrupted){
        //qDebug() << "wait for reading";
        if (socket.waitForReadyRead()){
            //qDebug() << "Available: " << socket.bytesAvailable();
            QByteArray message = socket.readAll();
            file.write(message);
            /*stream_decoder.addStreamBytes(message);
            cv::cuda::GpuMat frame;
            if (stream_decoder.nextFrame(frame)){
                frame.copyTo(last_frame);
                cv::imshow("Received video", last_frame);
            }*/
            //qDebug() << message << "\n";
        }
    }
}
