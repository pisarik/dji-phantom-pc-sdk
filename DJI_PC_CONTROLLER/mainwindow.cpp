#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "videogetter.h"
#include "telemetrygetter.h"
#include "controlsender.h"

#include <QDebug>
#include <QThread>
#include <QSlider>
#include <QDial>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->video_view->setScene(&scene);
  scene.addItem(&cur_frame);

  ui->right_vlayout->layout()->setAlignment(Qt::AlignRight);
  ui->center_vlayout->layout()->setAlignment(Qt::AlignHCenter);

  connect(ui->pitch_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocitiesEdit()));
  connect(ui->roll_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocitiesEdit()));
  connect(ui->yaw_dial, SIGNAL(valueChanged(int)), this, SLOT(updateVelocitiesEdit()));
  connect(ui->throttle_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocitiesEdit()));

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
    static quint64 last_time = t.time;
    ui->velocity_x_edit->setText(QString::number(t.velocity_x));
    ui->velocity_y_edit->setText(QString::number(t.velocity_y));
    ui->velocity_z_edit->setText(QString::number(t.velocity_z));

    ui->pitch_edit->setText(QString::number(t.pitch));
    ui->roll_edit->setText(QString::number(t.roll));
    ui->yaw_edit->setText(QString::number(t.yaw));

    ui->latitude_edit->setText(QString::number(t.latitude));
    ui->longitude_edit->setText(QString::number(t.longitude));
    ui->altitude_edit->setText(QString::number(t.altitude));

    ui->dtime_edit->setText(QString::number(t.time-last_time));
    last_time = t.time;
}

void MainWindow::saveTelemetry(Telemetry t)
{
    t.writeToFile("telemetry.txt");
}

void MainWindow::setMinMaxVelocities(QVector<double> velocities)
{
    ui->pitch_slider->setMinimum(velocities[0]);
    ui->pitch_slider->setMaximum(velocities[1]);
    ui->roll_slider->setMinimum(velocities[2]);
    ui->roll_slider->setMaximum(velocities[3]);
    ui->yaw_dial->setMinimum(velocities[4]);
    ui->yaw_dial->setMaximum(velocities[5]);
    ui->throttle_slider->setMinimum(velocities[6]);
    ui->throttle_slider->setMaximum(velocities[7]);
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

void MainWindow::on_video_box_toggled(bool video_enabled)
{
    if (video_enabled){
        start_video();
    }
    else{
        ui->log_edit->appendPlainText("Interrupting video");
        emit interrupt_video_receiving();
    }
}

void MainWindow::videoReceivingStopped() {
    qDebug() << "VideoThread finished";
    is_video_receiving = false;
    ui->video_box->setChecked(false);
}

void MainWindow::on_telemetry_box_toggled(bool telemetry_enabled)
{
    if (telemetry_enabled){
        start_telemetry();
    }
    else{
        ui->log_edit->appendPlainText("Interrupting telemetry");
        emit interrupt_telemetry_receiving();
    }
}

void MainWindow::telemetryReceivingStopped() {
    qDebug() << "Telemetry thread finished";
    is_telemetry_receiving = false;
    ui->telemetry_box->setChecked(false);
}

void MainWindow::on_controls_box_toggled(bool control_enabled)
{
    if (control_enabled){
        start_control();
    }
    else{
        ui->log_edit->appendPlainText("Interrupting control");
        emit interrupt_control_sending();
    }
}

void MainWindow::controlSendingStopped()
{
    qDebug() << "Control object deleted";
    is_control_sending = false;
    ui->controls_box->setChecked(false);
}

void MainWindow::updateVelocitiesEdit()
{
    QString line = "pitch: %1 | roll: %2 | yaw: %3 | throttle: %4 m/s";

    int pitch = ui->pitch_slider->value();
    int roll = ui->roll_slider->value();
    int yaw = ui->yaw_dial->value();
    int throttle = ui->throttle_slider->value();

    ui->velocities_edit->setText(line.arg(QString::number(pitch),
                                          QString::number(roll),
                                          QString::number(yaw),
                                          QString::number(throttle)));
}

void MainWindow::start_video()
{
    QString ip_address = ui->ip_edit->text();
    quint16 port = ui->port_edit->text().toInt();

    if (!is_video_receiving){
        QThread *video_thread = new QThread();

        VideoGetter *video_getter = new VideoGetter();
        video_getter->setAddress(ip_address, port);
        video_getter->moveToThread(video_thread);


        // start process in video getter
        connect(video_thread, SIGNAL(started()),
                video_getter, SLOT(start()));

        // get frames from video_getter
        connect(video_getter, SIGNAL(gotFrame(QByteArray,quint32,QString)),
                this, SLOT(showRawFrame(QByteArray,quint32,QString)));

        // interrupt video receiving
        connect(this, SIGNAL(interrupt_video_receiving()),
                video_getter, SLOT(interrupt()));

        // kill thread, when finished
        connect(video_getter, SIGNAL(finished()),
                video_thread, SLOT(quit()));
        // delete this
        connect(video_getter, SIGNAL(finished()),
                video_getter, SLOT(deleteLater()));
        connect(video_thread, SIGNAL(finished()),
                video_thread, SLOT(deleteLater()));
        // video receiving stopped
        connect(video_getter, SIGNAL(finished()),
                this, SLOT(videoReceivingStopped()));

        is_video_receiving = true;
        video_thread->start();
    }
    else{
        ui->log_edit->appendPlainText("Video already receiving");
    }
}

void MainWindow::start_telemetry()
{
    QString ip_address = ui->ip_edit->text();
    quint16 port = ui->port_edit->text().toInt();

    if (!is_telemetry_receiving){
        QThread *telemetry_thread = new QThread();

        TelemetryGetter *telemetry_getter = new TelemetryGetter();
        telemetry_getter->setAddress(ip_address, port);
        telemetry_getter->moveToThread(telemetry_thread);


        // start process in telemetry getter
        connect(telemetry_thread, SIGNAL(started()),
                telemetry_getter, SLOT(start()));

        // get frames from telemetry_getter
        connect(telemetry_getter, SIGNAL(gotTelemetry(Telemetry)),
                this, SLOT(showTelemetry(Telemetry)));
        connect(telemetry_getter, SIGNAL(gotTelemetry(Telemetry)),
                this, SLOT(saveTelemetry(Telemetry)));

        // interrupt telemetry receiving
        connect(this, SIGNAL(interrupt_telemetry_receiving()),
                telemetry_getter, SLOT(interrupt()));

        // kill thread, when finished
        connect(telemetry_getter, SIGNAL(finished()),
                telemetry_thread, SLOT(quit()));
        // delete this
        connect(telemetry_getter, SIGNAL(finished()),
                telemetry_getter, SLOT(deleteLater()));
        connect(telemetry_thread, SIGNAL(finished()),
                telemetry_thread, SLOT(deleteLater()));
        // telemetry receiving stopped
        connect(telemetry_getter, SIGNAL(finished()),
                this, SLOT(telemetryReceivingStopped()));

        is_telemetry_receiving = true;
        telemetry_thread->start();
    }
    else{
        ui->log_edit->appendPlainText("Telemetry already receiving");
    }
}

void MainWindow::start_control()
{
    QString ip_address = ui->ip_edit->text();
    quint16 port = ui->port_edit->text().toInt();

    if (!is_control_sending){
        ControlSender *control_sender = new ControlSender();
        control_sender->setAddress(ip_address, port);

        // receive min max velocities
        connect(control_sender, SIGNAL(minMaxVelocities(QVector<double>)),
                this, SLOT(setMinMaxVelocities(QVector<double>)));

        // send commands
        using Direction = ControlSender::Direction;
        connect(ui->pitch_slider, &QSlider::valueChanged, control_sender,
                [control_sender](int value){
                    control_sender->sendCommand(Direction::PITCH, value);
                });
        connect(ui->roll_slider, &QDial::valueChanged, control_sender,
                [control_sender](int value){
                    control_sender->sendCommand(Direction::ROLL, value);
                });
        connect(ui->yaw_dial, &QSlider::valueChanged, control_sender,
                [control_sender](int value){
                    control_sender->sendCommand(Direction::YAW, value);
                });
        connect(ui->throttle_slider, &QSlider::valueChanged, control_sender,
                [control_sender](int value){
                    control_sender->sendCommand(Direction::THROTTLE, value);
                });

        // interrupt telemetry receiving
        connect(this, SIGNAL(interrupt_control_sending()),
                control_sender, SLOT(interrupt()));

        // delete this
        connect(control_sender, SIGNAL(finished()),
                control_sender, SLOT(deleteLater()));
        // controls sending stopped
        connect(control_sender, SIGNAL(finished()),
                this, SLOT(controlSendingStopped()));

        is_control_sending = true;
        control_sender->start();
    }
    else{
        ui->log_edit->appendPlainText("Control already sending");
    }
}

void MainWindow::on_pitch_slider_sliderReleased()
{
    ui->pitch_slider->setValue(0);
}

void MainWindow::on_roll_slider_sliderReleased()
{
    ui->roll_slider->setValue(0);
}

void MainWindow::on_yaw_dial_sliderReleased()
{
    ui->yaw_dial->setValue(0);
}

void MainWindow::on_throttle_slider_sliderReleased()
{
    ui->throttle_slider->setValue(0);
}
