#include "mainwindow.h"
#include <QApplication>

#include "telemetrygetter.h"

int main(int argc, char *argv[])
{  
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  qRegisterMetaType<Telemetry>("Telemetry");

  return a.exec();
}
