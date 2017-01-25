#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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

private slots:
  void on_connect_btn_clicked();

  void log_connected();

  void stateChanged( QAbstractSocket::SocketState state )
    {
    qDebug() << "State changed:" << state;
    }

  void on_get_video_btn_clicked();

  void on_interrupt_video_btn_clicked();

private:
  Ui::MainWindow *ui;
  QTcpSocket socket;

  VideoGetter getter;
};

#endif // MAINWINDOW_H
