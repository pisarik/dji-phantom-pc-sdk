#ifndef VIDEOGETTER_H
#define VIDEOGETTER_H

#include <thread>
#include <QTcpSocket>

#include <opencv2/core.hpp>


class VideoGetter : public QObject
{
    Q_OBJECT

    const QString socket_type;
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

    QString ip;
    quint16 port;

    QTcpSocket *socket;
};

#endif // VIDEOGETTER_H
