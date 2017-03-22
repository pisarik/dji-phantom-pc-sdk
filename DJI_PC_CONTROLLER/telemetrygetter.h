#ifndef TELEMETRYGETTER_H
#define TELEMETRYGETTER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

struct Telemetry;

class TelemetryGetter : public QObject
{
    Q_OBJECT

    const QString socket_type;

    QString ip;
    quint16 port;

    QTcpSocket *socket;
public:
    TelemetryGetter();
    ~TelemetryGetter();

    void setAddress(QString ip, quint16 port);

public slots:
    void start();
    void interrupt();

signals:
    void gotTelemetry(Telemetry telemetry);
    void finished();

private:
    void writeLoop();
    void readLoop();

    bool isSocketGood(){
        return socket != nullptr
               && socket->state() == QTcpSocket::SocketState::ConnectedState;
    }
};

struct Telemetry{
    int frame_num;
    double latitude;
    double longitude;
    double velocity_x;
    double velocity_y;
    double velocity_z;
    double pitch;
    double roll;
    double yaw;

    void readFrom(QIODevice *d){
        QDataStream stream(d);

        stream >> frame_num >> latitude >> longitude
               >> velocity_x >> velocity_y >> velocity_z
               >> pitch >> roll >> yaw;
    }
};

#endif // TELEMETRYGETTER_H
