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

//for the history view
QString Session::toString(){
    QString newString =
            startTime.toString("ddd h:mm ap") + "\n"
            + "   Average Coherence: " + QString::number(getAvgScore()) + "\n"
            + "   Achievment Score: " + QString::number(std::ceil(getAchievement()* 100) /100.0) + "\n"
            + "   Challenge Level: " + getChallengeLvl() + "\n"
            + "   Time in Low: " + QString::number(std::ceil(pTimeLow/length * 10000) /100.0) +"%" + "\n"
            + "   Time in Medium: " + QString::number(std::ceil(pTimeMed/length * 10000) /100.0) +"%" + "\n"
            + "   Time in High: " + QString::number(std::ceil(pTimeHigh/length * 10000) /100.0) +"%" + "\n"
            + "   Duration: " + QString::number(length/60) + ((length%60 < 10) ? + ":0" + QString::number(length%60) : + ":" + QString::number(length%60)) + " minutes";

    return newString;
}

//getters
int Session::getSessionTime(){return length;}
double Session::getAchievement(){return achievement; }
double Session::getAvgScore(){return (totalCoherenceScore/length);}
double Session::getTotalScore(){return totalCoherenceScore;}

QString Session::getChallengeLvl(){
    QString chalLvl = "";
    switch(challengeLvl){
        case 2:
             chalLvl = "Adept";
            break;
        case 3:
            chalLvl = "Intermediate";
            break;
        case 4:
            chalLvl = "Advanced";
            break;
        default:
            chalLvl = "Beginner";
    }
    return chalLvl;
}

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
