#ifndef DEVICE_H
#define DEVICE_H





/* Class Purpose: Saves battery level and power level inbetween uses of the app
 *
 * Data Members:
 * - double batteryLevel: a double representation of the battery level
 * - int powerLvl: a integer representation of the power level
 * - int id: a unique identifier
 *
 * Class Functions:
 * - Getters and Setters
 */

class Device {

public:
    Device(int);
    int getBatteryLevel();
    void setBatteryLevel(int);
    void setFullCharge();

private:
    int m_batteryLevel;

};

#endif // DEVICE_H
