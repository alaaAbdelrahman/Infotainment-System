#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
   // connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &Client::onErrorOccurred);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::onDataReceived);
}

void Client::connectToHost(const QString &host, quint16 port)
{
    socket.connectToHost(host, port);
}

void Client::disconnectFromHost()
{
    socket.disconnectFromHost();
}

void Client::sendData(char sensorId)
{
    QByteArray data;
    data.append(sensorId);
    socket.write(data);
}

void Client::onConnected()
{
    emit connected();
}

void Client::onDisconnected()
{
    emit disconnected();
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    emit errorOccurred(socketError);
}

void Client::onDataReceived()
{
    while (socket.bytesAvailable() >= 4) {
        QByteArray data = socket.read(4);
       m_sensorId = data[0];
        m_value    = (static_cast<unsigned char>(data[2]) << 8) | static_cast<unsigned char>(data[3]);
        emit dataReceived(m_sensorId, m_value);
    }
}

int Client::value() const
{
    return m_value;
}

void Client::setValue(int newValue)
{
    m_value = newValue;
}

char Client::sensorId() const
{
    return m_sensorId;
}

void Client::setSensorId(char newSensorId)
{
    m_sensorId = newSensorId;
}
