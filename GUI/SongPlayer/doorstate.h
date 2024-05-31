#ifndef DOORSTATE_H
#define DOORSTATE_H


#include <QObject>
#include <qqml.h>

class DoorState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int  door READ door WRITE setDoor NOTIFY doorChanged )
public:
    explicit DoorState(QObject *parent = nullptr);

    int door();
    void setDoor(char state);
signals:
    void doorChanged();


private:
    int m_doorState =0;
};

#endif // DOORSTATE_H
