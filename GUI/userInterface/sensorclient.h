#ifndef SENSORCLIENT_H
#define SENSORCLIENT_H

#include <QObject>
#include <QTimer>

#include "client.h"


class SensorClient : public QObject
{
    Q_OBJECT
public:
    explicit SensorClient(QObject *parent = nullptr);

    char sensorId() const;
    void setSensorId(char newSensorId);

    int getValue() const;
    void setValue(int newValue);

signals:
    void dataReceived(char sensorId, int value);

public slots:
    void connectToHost( QString &host, quint16 port);
    void disconnectFromHost();
    void startSendingRequests( );
    void stopSendingRequests();

private slots:
    void sendSensorRequest();
    void onDataReceived(char sensorId, int value); // New slot to handle received data

private:
    Client client;
    QTimer timer;
    int interval{30};
    char currentSensorId{'D'};
    char m_sensorId;
    int m_value;


};

#endif // SENSORCLIENT_H
