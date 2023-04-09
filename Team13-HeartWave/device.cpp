#include "device.h"


//settings here as well
Device::Device(int batteryLevel, int lvl, int secs): m_batteryLevel(batteryLevel), challengeLevel(lvl), breathTimeInterval(secs){}

Device::~Device(){}



// getters
int Device::getBatteryLevel() { return m_batteryLevel; }
int Device::getChallengeLevel(){return challengeLevel; }
int Device::getBreathPacer(){ return breathTimeInterval; }

// setters
void Device::setBatteryLevel(int newBatteryLevel) { m_batteryLevel = newBatteryLevel; }
void Device::setFullCharge() { m_batteryLevel = 100.0; }

//sets the challenge level of the device
void Device::setChallengeLevel(int lvl){
    int maxChall = 4;
    int minChall = 1;
    if(lvl > maxChall){
        challengeLevel = maxChall;
    }
    if(lvl < minChall){
        challengeLevel = minChall;
    }
    else{
        challengeLevel = lvl;
    }
}
//sets the breath pace of the device
void Device::setBreathPacer(int secs){
        breathTimeInterval = secs;
}

void Device::resetSettings(){
    int defaultTime = 10;
    int defaultChallenge = 1;

    breathTimeInterval = defaultTime;
    challengeLevel = defaultChallenge;
    sessions.clear();
}

void Device::addSession(Session* s){
    sessions.append(s);
}

void Device::getSession(int index, Session** s){
    if(index >= 0 && index < sessions.length()){
        *s = sessions[index];
        return;
    }
    *s = NULL;
}
