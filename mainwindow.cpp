/**
 * @file mainwindow.cpp
 * @brief client
 * @author Agapova Ekaterina
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sockCl = new QTcpSocket(this);
    sockCl->connectToHost("localhost", 1337, QTcpSocket::ReadWrite);
    if(sockCl->waitForConnected(30000)) {
        qInfo() << "Connected!";
        qInfo() << sockCl->peerAddress();
    } else{
         QMessageBox::information(NULL,QObject::tr("Информация"),tr("Нет подключения к серверу."));
    }

    connect(sockCl, &QTcpSocket::readyRead,
            this, &MainWindow::readSocket);
    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::execButtonAction);
    connect(ui->setNick, &QPushButton::clicked,
            this, &MainWindow::setNick);
    connect(sockCl, &QTcpSocket::disconnected,
            this, &MainWindow::serverDisconnect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::execButtonAction()
{
    if(ui->nickname->text().toUtf8() == "" || nick == " "){
         QMessageBox::information(NULL,QObject::tr("Ошибка"),tr("Введите ник"));
         return;
    } else if (ui->message->text().toUtf8() == ""){
         QMessageBox::information(NULL,QObject::tr("Ошибка"),tr("Введите сообщение"));
         return;
    }else
        sockCl->write(ui->nickname->text().toUtf8()
                        + " " + ui->message->text().toUtf8());
}

void MainWindow::readSocket()
{
    QString data = QString(sockCl->readAll());
    ui->textEdit->append(data);
}

void MainWindow::setNick(){
    nick = ui->nickname->displayText();
    ui->nickname->setReadOnly("True");
}

void MainWindow::serverDisconnect()
{
    QMessageBox::information(NULL,QObject::tr("Информация"),tr("Сервер был отключен"));
}
