#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      新的连接
******************************************************************************/
void TcpServer::incomingConnection(qintptr handle)
{
    emit NewGuest(handle);
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
