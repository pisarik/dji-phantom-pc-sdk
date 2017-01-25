#include "rawh264source.h"

#include <QDebug>

#include <algorithm>

RawH264Source::RawH264Source()
    : FRAME_START({0, 0, 0, 1})
{
    f.chromaFormat = cv::cudacodec::YUV420;
    f.codec = cv::cudacodec::H264;
    f.width = 3840;
    f.height = 2160;
}

void RawH264Source::pushData(const QByteArray &appendix)
{
    QDebug debug = qDebug();
    //debug << "New bytes: ";
    for (int i = 0; i < appendix.size(); i++){
        buffer.push_back(appendix[i]);
        //debug << (int)buffer.back();
    }
}

bool RawH264Source::getNextPacket(unsigned char **data, int *size, bool *endOfFile)
{
    qDebug() << "NextPacket executed!!";
    bool is_complete_frame = false;
    data = nullptr;
    size = nullptr;
    endOfFile = nullptr;
    if (buffer.size() > 4){
        auto next_frame_it = std::search(buffer.begin() + readed_bytes + 3, buffer.end(),
                                         FRAME_START.begin(), FRAME_START.end());
        int next_frame_pos = next_frame_it - buffer.begin();

        is_complete_frame = next_frame_it != buffer.end();
        if (is_complete_frame){
            last_frame_size = next_frame_pos - readed_bytes;
            last_frame_pos = &buffer[readed_bytes];

            data = &last_frame_pos;
            size = &last_frame_size;

            readed_bytes = next_frame_pos;
        }

        endOfFile = &end_of_file;
    }

    return true;
}

cv::cudacodec::FormatInfo RawH264Source::format() const
{
    return f;
}
