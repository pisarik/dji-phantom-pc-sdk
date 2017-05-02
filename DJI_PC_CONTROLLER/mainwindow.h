#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>

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
    void saveTelemetry(Telemetry t);

    void setMinMaxVelocities(QVector<double> velocities);

signals:
    void interrupt_video_receiving();
    void interrupt_telemetry_receiving();
    void interrupt_control_sending();

private slots:
  void on_connect_btn_clicked();

  void log_connected();

  void on_video_box_toggled(bool video_enabled);
  void videoReceivingStopped();

  void on_telemetry_box_toggled(bool telemetry_enabled);
  void telemetryReceivingStopped();

  void on_controls_box_toggled(bool control_enabled);
  void controlSendingStopped();

  void updateVelocitiesEdit();

  void on_pitch_slider_sliderReleased();
  void on_roll_slider_sliderReleased();
  void on_yaw_dial_sliderReleased();
  void on_throttle_slider_sliderReleased();

private:
  void start_video();
  void start_telemetry();
  void start_control();


private:
  Ui::MainWindow *ui;

  QGraphicsScene scene;
  QGraphicsPixmapItem cur_frame;

  bool is_video_receiving = false;
  bool is_telemetry_receiving = false;
  bool is_control_sending = false;
};

#endif // MAINWINDOW_H
