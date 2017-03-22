#ifndef VIDEOGETTER_H
#define VIDEOGETTER_H

#include <QObject>
#include <QTcpSocket>

class VideoGetter : public QObject
{
    Q_OBJECT

    const QString socket_type;

    QString ip;
    quint16 port;

    QTcpSocket *socket;
public:
    VideoGetter();
    ~VideoGetter();

    void setAddress(QString ip, quint16 port);

public slots:
    void start();
    void interrupt();

signals:
    void gotFrame(QByteArray raw_frame_bytes, quint32 frame_num,
                  QString format = "JPG");
    void finished();

private:
    void writeLoop();
    void readLoop();

    bool isSocketGood(){
        return socket != nullptr
               && socket->state() == QTcpSocket::SocketState::ConnectedState;
    }
};

#endif // VIDEOGETTER_H
