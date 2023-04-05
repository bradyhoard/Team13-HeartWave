#ifndef SESSION_H
#define SESSION_H
#include <qcustomplot.h>

class Session{

public:                         //Date as well
    Session(int challengeLevel);
    ~Session();
    //getters
    int getSessionTime();
    double getAchievement();
    double getAvgScore();
    int getChallengeLvl();
    int getPTimeInLow();
    int getPTimeInMed();
    int getPTimeInHigh();
    //get graph
    //get date

    //setters
    void setLength(int seconds);
    void setAchieved(int achievement);
    void setScore(double avgScore);
    void setPTimeInLow(int pLow);
    void setPTimeInMed(int pMed);
    void setPTimeInHigh(int pHigh);
    //save graph data for x and y

private:
    int length; //in seconds
    double achievement;
    double avgCoherenceScore;
    int challengeLvl;
    int pTimeLow; //% time in low coherence
    int pTimeMed; //% time in med coherence
    int pTimeHigh; //% time in high coherence
//    const QVector<double>& graphXvalues;
//    const QVector<double>& graphYvalues;
//Date of session -> look denas and yoink from there
};



#endif // SESSION_H
