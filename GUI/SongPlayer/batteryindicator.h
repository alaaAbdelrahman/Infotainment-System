#ifndef BATTERYINDICATOR_H
#define BATTERYINDICATOR_H

#include <QObject>

class BatteryIndicator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int  batValue READ batValue WRITE setBatValue NOTIFY batValueChanged )
public:
    explicit BatteryIndicator(QObject *parent = nullptr);


    int batValue();
    void setBatValue(int value);
signals:
    void batValueChanged();


private:
    int m_value{0};
};

#endif // BATTERYINDICATOR_H
