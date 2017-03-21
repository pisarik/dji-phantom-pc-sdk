#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "videogetter.h"

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

signals:
    void interrupt_video_receiving();

private slots:
  void on_connect_btn_clicked();

  void log_connected();

  void on_get_video_btn_clicked();

  void on_interrupt_video_btn_clicked();

  void videoReceivingStopped() {    qDebug() << "Thread finished"; isVideoReceiving = false; }

private:
  Ui::MainWindow *ui;

  QGraphicsScene scene;
  QGraphicsPixmapItem cur_frame;

  bool isVideoReceiving = false;
};

#endif // MAINWINDOW_H
