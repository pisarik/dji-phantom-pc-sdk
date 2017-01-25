#include "h264streamdecoder.h"

H264StreamDecoder::H264StreamDecoder(QObject *parent) : QObject(parent)
{
    frame_source = new RawH264Source();
    video_reader = cv::cudacodec::createVideoReader(frame_source);
}

void H264StreamDecoder::addStreamBytes(const QByteArray &data)
{
    frame_source->pushData(data);
}

bool H264StreamDecoder::nextFrame(cv::OutputArray frame)
{
    return video_reader->nextFrame(frame);
}
