#include "client.h"

Client::Client(QObject *parent) :QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
   // connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &Client::onErrorOccurred);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::onDataReceived);
    //connect(&timer, &QTimer::timeout, this, &Client::sendSensorRequest);
}

Client::~Client()
{
    disconnectFromHost();
    socket.close();
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
    /*
  if(socket.bytesAvailable() == 3) {
        QByteArray data = socket.read(3);
        qInfo()<<data;
       m_sensorId = data[0];

        m_value    = data[2];
       //qInfo()<<m_sensorId;Z

       qInfo()<<m_value;
        emit dataReceived(m_sensorId, m_value);

    qInfo()<<"data";
*/

        QByteArray data = socket.read(3);
        qInfo() << "Received data:" << data;

        m_sensorId = data[0];
        m_value = data[2];

        qInfo() << "Sensor ID:" << m_sensorId;
        qInfo() << "Value:" << m_value;

        emit dataReceived(m_sensorId, m_value);

        // Reset buffer
        socket.readAll(); // Clear the buffer


}

/*D:
void Client::startSendingRequests()
{
    timer.start(interval);
    // Initially, send a request immediately
    sendSensorRequest();
}*/

/*
void Client::sendSensorRequest()
{
    client.sendData(currentSensorId);
    // Switch to the next sensor ID
    if (currentSensorId == 'D') {
        currentSensorId = 'T';
    } else if (currentSensorId == 'T') {
        currentSensorId = 'B';
    } else {
        currentSensorId = 'D';
    }
}
*/
char Client::value() const
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

