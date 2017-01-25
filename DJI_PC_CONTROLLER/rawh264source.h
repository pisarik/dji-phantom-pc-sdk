#ifndef RAWH264SOURCE_H
#define RAWH264SOURCE_H

#include <QByteArray>
#include <QQueue>
#include <vector>
#include <array>

#include <opencv2/cudacodec.hpp>

class RawH264Source : public cv::cudacodec::RawVideoSource
{
public:
    const std::array<unsigned char, 6> FRAME_START;

    RawH264Source();

    void pushData(const QByteArray &appendix);

    // RawVideoSource interface
public:
    bool getNextPacket(unsigned char **data, int *size, bool *endOfFile);
    cv::cudacodec::FormatInfo format() const;

private:
    //QByteArray buffer;
    std::vector<unsigned char> buffer;

    unsigned char *last_frame_pos = 0;
    int last_frame_size = 0;
    unsigned int readed_bytes = 0;

    bool end_of_file = false;

    cv::cudacodec::FormatInfo f;
};

#endif // RAWH264SOURCE_H
