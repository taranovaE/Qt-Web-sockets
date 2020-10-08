/**
 * @file mainwindow.h
 * @brief client
 * @author Agapova Ekaterina
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    /**
     * @brief socket of connection
     */
    QTcpSocket *sockCl;
    Ui::MainWindow *ui;
    QString nick = " ";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief send messages
     */
    void execButtonAction();
    /**
     * @brief read Socket
     */
    void readSocket();
    /**
     * @brief set nickname of user
     */
    void setNick();
    /**
     * @brief serverDisconnect
     */
    void serverDisconnect();
};
#endif // MAINWINDOW_H
