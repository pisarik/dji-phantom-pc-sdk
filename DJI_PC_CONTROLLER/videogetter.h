#ifndef VIDEOGETTER_H
#define VIDEOGETTER_H

#include <thread>
#include <QTcpSocket>

#include <opencv2/core.hpp>
#include "h264streamdecoder.h"


class VideoGetter
{
public:
    VideoGetter();

    void start(QString ip, quint16 port);
    void interrupt();

private:
    void writeLoop();
    void readLoop();

    QTcpSocket socket;
    bool isInterrupted;

    std::thread writingThread;
    std::thread readingThread;

    //H264StreamDecoder stream_decoder;
    cv::Mat last_frame;
};

#endif // VIDEOGETTER_H
