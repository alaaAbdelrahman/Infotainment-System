#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    char sensorId() const;
    void setSensorId(char newSensorId);

    int value() const;
    void setValue(int newValue);

signals:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void dataReceived(char sensorId, int value);

public slots:
    void connectToHost(const QString &host, quint16 port);
    void disconnectFromHost();
    void sendData(char sensorId);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onDataReceived();

private:
    QTcpSocket socket;
    char m_sensorId ;
        int m_value  ;

};

#endif // CLIENT_H
