#ifndef SESSION_H
#define SESSION_H
#include <qcustomplot.h>
#include <QDateTime>
#include <QTime>
#include <QVector>

class Session{

public:
    Session(int challengeLevel, const QDateTime& date);
    ~Session();

    QString toString();
    //getters
    int getSessionTime();
    double getAchievement();
    double getAvgScore();
    QString getChallengeLvl();
    double getPTimeInLow();
    double getPTimeInMed();
    double getPTimeInHigh();
    double getTotalScore();
    QDateTime getStartTime();
    //get graph
    //get date

    //setters
    void setLength(int seconds);
    void setAchieved(int achievement);
    void setScore(double score);
    void setPTimeInLow(int pLow);
    void setPTimeInMed(int pMed);
    void setPTimeInHigh(int pHigh);
    //save graph data for x and y
    void addPointX(double point);
    void addPointY(double point);


private:
    int length; //in seconds
    double achievement;
    double totalCoherenceScore;
    int challengeLvl;
    double pTimeLow; //% time in low coherence
    double pTimeMed; //% time in med coherence
    double pTimeHigh; //% time in high coherence
    QVector<double> graphXvalues;
    QVector<double> graphYvalues;
    QDateTime startTime;
};



#endif // SESSION_H
