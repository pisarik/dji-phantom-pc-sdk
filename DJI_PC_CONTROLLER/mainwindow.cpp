#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->video_view->setScene(&scene);
  scene.addItem(&cur_frame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showRawFrame(QByteArray frame_bytes, quint32 frame_num,
                              QString format)
{
    QPixmap image;
    image.loadFromData(frame_bytes,
                       format.toLocal8Bit().data());

    cur_frame.setPixmap(image);
}

void MainWindow::on_connect_btn_clicked()
{
  QString ip_address = ui->ip_edit->text();
  quint16 port = ui->port_edit->text().toInt();
  ui->log_edit->appendPlainText("Connection to: " + ip_address + ":" + QString::number(port));

  QTcpSocket socket;

  socket.connectToHost(ip_address, port, QTcpSocket::ReadWrite,
                       QAbstractSocket::IPv4Protocol);
  if (socket.waitForConnected(3000)){
    log_connected();
  }
  else{
    ui->log_edit->appendPlainText("Cannot connect!");
  }
  socket.close();

}

void MainWindow::log_connected()
{
  ui->log_edit->appendPlainText("Connected!");
}

void MainWindow::on_get_video_btn_clicked()
{
    QString ip_address = ui->ip_edit->text();
    quint16 port = ui->port_edit->text().toInt();

    if (!isVideoReceiving){
        QThread *video_thread = new QThread();

        VideoGetter *video_getter = new VideoGetter();
        video_getter->setAddress(ip_address, port);
        video_getter->moveToThread(video_thread);


        // start process in video getter
        connect(video_thread, SIGNAL(started()), video_getter, SLOT(start()));

        // get frames from video_getter
        connect(video_getter, SIGNAL(gotFrame(QByteArray,quint32,QString)),
                this, SLOT(showRawFrame(QByteArray,quint32,QString)));

        // interrupt video receiving
        connect(this, SIGNAL(interrupt_video_receiving()),
                video_getter, SLOT(interrupt()));

        // kill thread, when finished
        connect(video_getter, SIGNAL(finished()), video_thread, SLOT(quit()));
        // delete this
        connect(video_getter, SIGNAL(finished()), video_getter, SLOT(deleteLater()));
        connect(video_thread, SIGNAL(finished()), video_thread, SLOT(deleteLater()));
        // video receiving stopped
        connect(video_getter, SIGNAL(finished()), this, SLOT(videoReceivingStopped()));

        video_thread->start();
        isVideoReceiving = true;
    }
    else{
        ui->log_edit->appendPlainText("Video already receiving");
    }


}

void MainWindow::on_interrupt_video_btn_clicked()
{
    ui->log_edit->appendPlainText("Interrupting");

    emit interrupt_video_receiving();
}
