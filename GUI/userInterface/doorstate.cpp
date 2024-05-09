#include "doorstate.h"

DoorState::DoorState(QObject *parent)
    : QObject{parent}
{}

int DoorState::door(){
    return m_doorState;
}

void DoorState::setDoor(int state)
{
    if (m_doorState == state)
        return;
    m_doorState = state;

    emit doorChanged();

}
