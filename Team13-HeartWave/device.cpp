#include "device.h"

Device::Device(int batteryLevel) {

    m_batteryLevel = batteryLevel;
}


// getters
int Device::getBatteryLevel() { return m_batteryLevel; }


// setters
void Device::setBatteryLevel(int newBatteryLevel) { m_batteryLevel = newBatteryLevel; }
void Device::setFullCharge() { m_batteryLevel = 100.0; }
