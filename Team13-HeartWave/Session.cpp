#include "Session.h"

Session::Session(int challengeLevel): challengeLvl(challengeLevel){}
Session::~Session(){}



//getters
int Session::getSessionTime(){return length;}
double Session::getAchievement(){return achievement; }
double Session::getAvgScore(){return avgCoherenceScore;}
int Session::getChallengeLvl(){return challengeLvl;}
int Session::getPTimeInLow(){return pTimeLow;}
int Session::getPTimeInMed(){return pTimeMed;}
int Session::getPTimeInHigh(){return pTimeHigh;}
//get graph
//get date

//setters
void Session::setLength(int seconds){
    length = seconds;
}
void Session::setAchieved(int achievement){
    this->achievement = achievement;
}
void Session::setScore(double avgScore){
    avgCoherenceScore = avgScore;
}
void Session::setPTimeInLow(int pLow){
    pTimeLow = pLow;
}
void Session::setPTimeInMed(int pMed){
    pTimeMed = pMed;
}
void Session::setPTimeInHigh(int pHigh){
    pTimeHigh = pHigh;
}

