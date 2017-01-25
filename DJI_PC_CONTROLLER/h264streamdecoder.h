#ifndef H264STREAMDECODER_H
#define H264STREAMDECODER_H

#include <QObject>

#include <opencv2/videoio.hpp>

#include "rawh264source.h"

class H264StreamDecoder : public QObject
{
    Q_OBJECT
public:
    explicit H264StreamDecoder(QObject *parent = 0);

    void addStreamBytes(const QByteArray &data);

    bool nextFrame(cv::OutputArray frame);

signals:

public slots:

private:
    cv::Ptr<cv::cudacodec::VideoReader> video_reader;
    cv::Ptr<RawH264Source> frame_source;
};

#endif // H264STREAMDECODER_H
