#include "mainwindow.h"
#include <QApplication>

#include "telemetrygetter.h"

#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char *argv[])
{  
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  qRegisterMetaType<Telemetry>("Telemetry");

  /*cv::VideoCapture cap(R"(video.mp4)");

  cv::Mat frame;

  while (cap.read(frame)){
      cv::imshow("Lol", frame);
      cv::waitKey(20);
  }
  return 0;*/

  return a.exec();
}
