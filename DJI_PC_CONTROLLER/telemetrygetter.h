#ifndef TELEMETRYGETTER_H
#define TELEMETRYGETTER_H

#include <QObject>
#include <QFile>
#include <QTcpSocket>
#include <QDataStream>

struct Telemetry;

class TelemetryGetter : public QObject
{
    Q_OBJECT
public:
    TelemetryGetter();
    ~TelemetryGetter();

    void setAddress(QString ip, quint16 port);

public slots:
    void start();
    void interrupt();

signals:
    void gotTelemetry(Telemetry t);
    void finished();

private:
    void writeLoop();
    void readLoop();

    bool isSocketGood(){
        return socket != nullptr
               && socket->state() == QTcpSocket::SocketState::ConnectedState;
    }

private:
    const QString socket_type;

    QString ip;
    quint16 port;

    QTcpSocket *socket;
};

struct Telemetry{
    int frame_num;
    unsigned long long time; //in ms from epoch
    double latitude;
    double longitude;
    double altitude;
    double velocity_x;
    double velocity_y;
    double velocity_z;
    double pitch;
    double roll;
    double yaw;

    void readFrom(QIODevice *d){
        QDataStream stream(d);

        stream >> frame_num >> time
               >> latitude >> longitude >> altitude
               >> velocity_x >> velocity_y >> velocity_z
               >> pitch >> roll >> yaw;
    }

    void writeToFile(QString filename){
        QFile file(filename);

        if (file.open(QIODevice::WriteOnly | QIODevice::Append)){
            QTextStream in(&file);
            in << frame_num << " " << time << " "
               << latitude << " " << longitude << " " << altitude << " "
               << velocity_x << " " << velocity_y << " " << velocity_z << " "
               << pitch << " " << roll << " " << yaw << "\n";
        }
    }
};

#endif // TELEMETRYGETTER_H
