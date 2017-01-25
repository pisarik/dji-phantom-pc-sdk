#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QObject::connect(
                  &socket,
                  SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                  this,
                  SLOT(stateChanged(QAbstractSocket::SocketState))
                  );
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_connect_btn_clicked()
{
  QString ip_address = ui->ip_edit->text();
  quint16 port = ui->port_edit->text().toInt();
  ui->log_edit->appendPlainText("Connection to: " + ip_address + ":" + QString::number(port));
  socket.connectToHost(ip_address, port, QTcpSocket::ReadWrite,
                       QAbstractSocket::IPv4Protocol);
  if (socket.waitForConnected(3000)){
    log_connected();
    QString message;
    socket.write(QString("VIDEO_TYPE").toLocal8Bit());
    socket.write("\n");
  }
  else{
    ui->log_edit->appendPlainText("Cannot connect!");
  }

}

void MainWindow::log_connected()
{
  ui->log_edit->appendPlainText("Connected!");
}

void MainWindow::on_get_video_btn_clicked()
{
    QString ip_address = ui->ip_edit->text();
    quint16 port = ui->port_edit->text().toInt();

    getter.start(ip_address, port);
}

void MainWindow::on_interrupt_video_btn_clicked()
{
    ui->log_edit->appendPlainText("Interrupting");
    getter.interrupt();
}
