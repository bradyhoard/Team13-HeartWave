#ifndef DEVICE_H
#define DEVICE_H
#include <QVector>
#include "Session.h"



/* Class Purpose: Saves battery level and power level inbetween uses of the app, save current device settings
 *
 * Data Members:
 * - double batteryLevel: a double representation of the battery level
 * - int powerLvl: a integer representation of the power level
 *
 * Class Functions:
 * - Getters and Setters
 */

class Device {

public:
    Device(int batteryLevel, int lvl = 1, int secs = 10);
    ~Device();

    int getBatteryLevel();
    int getChallengeLevel();
    int getBreathPacer();

    void setBatteryLevel(int);
    void setFullCharge();
    void setChallengeLevel(int lvl);
    void setBreathPacer(int secs);
    void resetSettings();
    void addSession(Session* s);

    void getSession(int index, Session** s);

private:
    int m_batteryLevel;
    QVector<Session*> sessions; //History of device pointers to all user sessions
    //Device settings
    int challengeLevel;
    int breathTimeInterval;

};

#endif // DEVICE_H
