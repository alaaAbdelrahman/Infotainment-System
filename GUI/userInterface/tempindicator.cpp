#include "tempindicator.h"

TempIndicator::TempIndicator(QObject *parent)
    : QObject{parent}
{}

int TempIndicator::value() const
{
    return m_value;
}

void TempIndicator::setValue(int newValue)
{
    m_value = newValue;
}



