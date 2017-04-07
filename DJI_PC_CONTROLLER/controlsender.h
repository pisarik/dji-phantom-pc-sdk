#ifndef CONTROLSENDER_H
#define CONTROLSENDER_H

#include <QObject>
#include <QTcpSocket>

class ControlSender : public QObject
{
    Q_OBJECT
public:
    enum class Direction {PITCH, ROLL, YAW, THROTTLE};

public:
    ControlSender();
    ~ControlSender();

    void setAddress(QString ip, quint16 port);

public slots:
    void start();
    void interrupt();

    void sendCommand(Direction dir, double velocity);

signals:
    void finished();

private:
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

#endif // CONTROLSENDER_H
