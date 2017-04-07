#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "telemetrygetter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
    void showRawFrame(QByteArray frame_bytes, quint32 frame_num, QString format);
    void showTelemetry(Telemetry t);

signals:
    void interrupt_video_receiving();
    void interrupt_telemetry_receiving();

private slots:
  void on_connect_btn_clicked();

  void log_connected();

  void on_video_box_toggled(bool video_enabled);
  void videoReceivingStopped();

  void on_telemetry_box_toggled(bool telemetry_enabled);
  void telemetryReceivingStopped();

private:
  void start_telemetry();
  void start_video();


private:
  Ui::MainWindow *ui;

  QGraphicsScene scene;
  QGraphicsPixmapItem cur_frame;

  bool is_video_receiving = false;
  bool is_telemetry_receiving = false;
};

#endif // MAINWINDOW_H
