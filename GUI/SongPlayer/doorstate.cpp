#include "doorstate.h"

DoorState::DoorState(QObject *parent)
    : QObject{parent}
{}

int DoorState::door(){
    return m_doorState;
}

void DoorState::setDoor(char state)
{
    if(state =='0'){
        m_doorState == 0;

    }
    else if(state =='1'){
    m_doorState = 1;
    }
    emit doorChanged();

}
