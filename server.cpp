/**
 * @file server.cpp
 * @brief server
 * @author Agapova Ekaterina
 */
#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    tcpS = new QTcpServer(this);

    connect(tcpS, &QTcpServer::newConnection,
            this, &Server::connectUser);

    if (tcpS->listen(QHostAddress::Any, 1337)) {
        qInfo() << "Server started";
    } else {
        qWarning() << "Unable to start server";
    }
    connect(this, &Server::serverClose,
                this, &QCoreApplication::quit);
    clients.clear();
    list.clear();
}

Server::~Server()
{
    emit serverClose();
}

void Server::connectUser()
{
    qInfo() << "We have new user!";
    foreach (client, clients) {
            QTcpSocket* clientSock = (QTcpSocket*) client;
            QTextStream str (clientSock);
            str << " 1 new user";
        }

    client = tcpS->nextPendingConnection();
    clients.push_back(client);
    this->send10Message();

    connect(client, &QTcpSocket::readyRead,
            this, &Server::runClientAction);
    connect(client, &QTcpSocket::disconnected,
                this, &Server::disconnectUser);
}

void Server::runClientAction()
{
    k++;
    QTcpSocket* clientSocket = (QTcpSocket*) sender();
    QTextStream stream (clientSocket);
    QString nickname;
    QString message;
    for (int i = 0;i < 2;i++){
        if (i == 0) stream >> nickname;
        if (i == 1) stream >> message;
    }
    qInfo() << "Get message from "<< nickname << ": " << message;

    foreach (client, clients){
        QTcpSocket* clSock = (QTcpSocket*) client;
        QTextStream str (clSock);
        str << nickname + ": " + message;
    }

    file.setFileName("file.txt");
    list.push_back(nickname + " " + message);

    if(!file.open(QIODevice::WriteOnly)){
        qInfo() << "File isn't open.";
    }

    if (k > 10){
        list.removeFirst();
    }

    QTextStream _stream(&file);
    foreach(QString str, list){
        if (str != ""){
           _stream << str << endl;
       }
    }
    file.close();
}

void Server::disconnectUser() {
    qInfo() << "We lost user!";

    foreach (client, clients) {
        QTcpSocket* clientSock = (QTcpSocket*) client;
        QTextStream str (clientSock);
        str << "    1 user left.";
    }
    clients.removeOne(client);
    client->disconnect();
    if(clients.size() == 0)
        emit serverClose();
}

void Server::send10Message() {
    file.setFileName("file.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qInfo() << "File isn't open.";
    }

    QTcpSocket* clSock = (QTcpSocket*) client;
    QTextStream str (clSock);
    QTextStream _stream(&file);
    int i = 0;
    while(!_stream.atEnd()){
        QString st;
        i++;
        _stream >> st;
        if (((i % 2) == 1)&&(st != ""))
            str << st + ": ";
        else {
            str << st;
            if(!_stream.atEnd())
                str<<endl;
        }
    }
    if(k == 0)
        k += i/2;
    file.close();
}
