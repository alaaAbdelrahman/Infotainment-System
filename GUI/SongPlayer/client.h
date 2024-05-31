#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~ Client();

    char sensorId() const;
    void setSensorId(char newSensorId);

    char value() const;
    void setValue(int newValue);

signals:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void dataReceived(char sensorId, char value);

public slots:
    void connectToHost(const QString &host, quint16 port);
    void disconnectFromHost();
    void sendData(char sensorId);
     //void startSendingRequests( );


private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onDataReceived();
  //  void sendSensorRequest();


private:
    QTcpSocket socket;
    QTimer timer;
    //int interval{30};
    //char currentSensorId;
    char m_sensorId ;
     char m_value  ;

};

#endif // CLIENT_H
