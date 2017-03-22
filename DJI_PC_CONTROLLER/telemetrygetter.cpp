#include "telemetrygetter.h"

#include <chrono>

#include <QDebug>
#include <QCoreApplication>

TelemetryGetter::TelemetryGetter()
   : socket_type("TELEMETRY_TYPE"),
     ip("0.0.0.0"), port(1212)
{
    socket = nullptr;
}

TelemetryGetter::~TelemetryGetter()
{
    if (socket != nullptr){
        delete socket;
    }
}

void TelemetryGetter::setAddress(QString ip, quint16 port)
{
    this->ip = ip;
    this->port = port;
}

void TelemetryGetter::start()
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

void TelemetryGetter::interrupt()
{
    qDebug() << "TelemetryGetter: disconnecting";
    socket->disconnectFromHost();
}

void TelemetryGetter::readLoop()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;

    auto start_time = high_resolution_clock::now();

    while (isSocketGood()){
        if (socket->waitForReadyRead(3000)){
            while (isSocketGood()
                   && socket->bytesAvailable() < sizeof(Telemetry))
                QCoreApplication::processEvents( QEventLoop::AllEvents, 1 );

            if (isSocketGood()){
                Telemetry telemetry;
                telemetry.readFrom(socket);

                emit gotTelemetry(telemetry);
            }
        }
        else{
            qDebug() << "Not ready socket for read";
        }
        QCoreApplication::processEvents( QEventLoop::AllEvents, 1 );
    }
    auto finish_time = high_resolution_clock::now();
    qDebug() << "Receiving telemetry time: "
             << duration_cast<milliseconds>(finish_time - start_time).count()
             << "ms";
}
