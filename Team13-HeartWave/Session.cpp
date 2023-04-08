#include "Session.h"

Session::Session(int challengeLevel,const QDateTime& date): challengeLvl(challengeLevel), startTime(date){
    this->pTimeLow = 0;
    this->pTimeHigh = 0;
    this->pTimeMed = 0;
    this->totalCoherenceScore = 0;
    this->achievement = 0;
    this->length = 0;
}
Session::~Session(){}



//getters
int Session::getSessionTime(){return length;}
double Session::getAchievement(){return achievement; }
double Session::getAvgScore(){return (totalCoherenceScore/length);}
double Session::getTotalScore(){return totalCoherenceScore;}
int Session::getChallengeLvl(){return challengeLvl;}
double Session::getPTimeInLow(){return pTimeLow;}
double Session::getPTimeInMed(){return pTimeMed;}
double Session::getPTimeInHigh(){return pTimeHigh;}
QDateTime Session::getStartTime() { return startTime; }

//get graph
//get date

//setters
void Session::setLength(int seconds){
    length = seconds;
}
void Session::setAchieved(int achievement){
    this->achievement = achievement;
}
void Session::setScore(double score){
    totalCoherenceScore = score;
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


void Session::addPointX(double point){
    graphXvalues.append(point);
}
void Session::addPointY(double point){
    graphYvalues.append(point);
}
