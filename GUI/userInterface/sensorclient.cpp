

#include "sensorclient.h"



SensorClient::SensorClient(QObject *parent) :QObject(parent)
{
    connect(&client, &Client::connected, this, &SensorClient::startSendingRequests);
    connect(&client, &Client::disconnected, this, &SensorClient::stopSendingRequests);
    connect(&timer, &QTimer::timeout, this, &SensorClient::sendSensorRequest);
    connect(&client, &Client::dataReceived, this, &SensorClient::onDataReceived);


   }

void SensorClient::connectToHost( QString &host, quint16 port)
{
    client.connectToHost(host, port);
}

void SensorClient::disconnectFromHost()
{
    client.disconnectFromHost();
}

void SensorClient::startSendingRequests()
{
    timer.start(interval);
    // Initially, send a request immediately
    sendSensorRequest();
}

void SensorClient::stopSendingRequests()
{
    timer.stop();
}

void SensorClient::sendSensorRequest()
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

void SensorClient::onDataReceived(char sensorId, int value)
{
    m_sensorId = sensorId;
    m_value = value;


}

int SensorClient::getValue() const
{
    return m_value;
}

void SensorClient::setValue(int newValue)
{
    m_value = newValue;
}

char SensorClient::sensorId() const
{
    return m_sensorId;
}

void SensorClient::setSensorId(char newSensorId)
{
    m_sensorId = newSensorId;
}
