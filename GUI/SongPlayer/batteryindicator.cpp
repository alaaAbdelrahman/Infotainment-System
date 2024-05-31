#include "batteryindicator.h"

BatteryIndicator::BatteryIndicator(QObject *parent)
    : QObject{parent}
{}

int BatteryIndicator::batValue()
{
    return m_value;

}

void BatteryIndicator::setBatValue(int value)
{

}
