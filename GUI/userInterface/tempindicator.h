#ifndef TEMPINDICATOR_H
#define TEMPINDICATOR_H

#include <QObject>
#include <QQmlEngine>

class TempIndicator : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int tempValue READ value WRITE setValue NOTIFY tempValueChanged )
public:
    explicit TempIndicator(QObject *parent = nullptr);


    int value() const;
    void setValue(int newValue);

signals:
    void tempValueChanged();
private:
    int m_value;
};

#endif // TEMPINDICATOR_H
