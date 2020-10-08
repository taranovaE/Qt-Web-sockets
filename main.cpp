/**
 * @file main.cpp
 * @author Agapova Ekaterina
 */

#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server serv;
    return a.exec();
}
