#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "videogetter.h"
#include "telemetrygetter.h"

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

void MainWindow::showTelemetry(Telemetry t)
{
    ui->velocity_x_edit->setText(QString::number(t.velocity_x));
    ui->velocity_y_edit->setText(QString::number(t.velocity_y));
    ui->velocity_z_edit->setText(QString::number(t.velocity_z));

    ui->pitch_edit->setText(QString::number(t.pitch));
    ui->roll_edit->setText(QString::number(t.roll));
    ui->yaw_edit->setText(QString::number(t.yaw));

    ui->latitude_edit->setText(QString::number(t.latitude));
    ui->longitude_edit->setText(QString::number(t.longitude));
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

    if (!is_video_receiving){
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
        is_video_receiving = true;
    }
    else{
        ui->log_edit->appendPlainText("Video already receiving");
    }


}

void MainWindow::on_interrupt_video_btn_clicked()
{
    ui->log_edit->appendPlainText("Interrupting video");

    emit interrupt_video_receiving();
}

void MainWindow::videoReceivingStopped() {
    qDebug() << "VideoThread finished";
    is_video_receiving = false;
}

void MainWindow::on_get_telemetry_btn_clicked()
{
    QString ip_address = ui->ip_edit->text();
    quint16 port = ui->port_edit->text().toInt();

    if (!is_telemetry_receiving){
        QThread *telemetry_thread = new QThread();

        TelemetryGetter *telemetry_getter = new TelemetryGetter();
        telemetry_getter->setAddress(ip_address, port);
        telemetry_getter->moveToThread(telemetry_thread);


        // start process in telemetry getter
        connect(telemetry_thread, SIGNAL(started()), telemetry_getter, SLOT(start()));

        // get frames from telemetry_getter
        connect(telemetry_getter, SIGNAL(gotTelemetry(Telemetry)),
                this, SLOT(showTelemetry(Telemetry)));

        // interrupt telemetry receiving
        connect(this, SIGNAL(interrupt_telemetry_receiving()),
                telemetry_getter, SLOT(interrupt()));

        // kill thread, when finished
        connect(telemetry_getter, SIGNAL(finished()), telemetry_thread, SLOT(quit()));
        // delete this
        connect(telemetry_getter, SIGNAL(finished()), telemetry_getter, SLOT(deleteLater()));
        connect(telemetry_thread, SIGNAL(finished()), telemetry_thread, SLOT(deleteLater()));
        // telemetry receiving stopped
        connect(telemetry_getter, SIGNAL(finished()), this, SLOT(telemetryReceivingStopped()));

        telemetry_thread->start();
        is_telemetry_receiving = true;
    }
    else{
        ui->log_edit->appendPlainText("Telemetry already receiving");
    }

}

void MainWindow::on_interrupt_telemetry_btn_clicked()
{
    ui->log_edit->appendPlainText("Interrupting telemetry");

    emit interrupt_telemetry_receiving();
}

void MainWindow::telemetryReceivingStopped() {
    qDebug() << "Telemetry thread finished";
    is_telemetry_receiving = false;
}
