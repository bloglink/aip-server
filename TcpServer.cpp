#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}

void TcpServer::incomingConnection(qintptr handle)
{
    emit InitSocket(handle);
}

