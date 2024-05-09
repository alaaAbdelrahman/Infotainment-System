#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{
    connect(&server.&QTcpServer::newConnection,this,Server::newConnection);
}

void Server::start()
{
    if(server.listen(QHostAddress::Any,2020)){
        qWarning()<<server.errorString();
        return;
    }
    qInfo()<<"Listening ....";

}

void Server::quit()
{
    server.close();
}

void Server::newConnection()
{
    QTcpSocket * socket = server.nextPendingConnection();

    // connect created socket to disconnected slot and ready read

    connect(socket.&QTcpSocket::diconnected,this,Server::diconnected);
    connect(socket.&QTcpSocket::readyRead,this,Server::readyRead);

    qInfo()<<"connected"<<socket;


}

void Server::diconnected()
{

    QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
    qInfo()<<"disconnected"<<socket;
    qInfo()<<"parent"<<socket->parent();

    socket->deleteLater();


}

void Server::readyRead()
{
QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());

    qInfo()<<"ready"<<socket;
qInfo()<<"data: "<<socket->readAll();




}
