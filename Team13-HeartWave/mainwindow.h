#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTime>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>
#include <QPainter>
#include <QDateTime>
#include<ctime>
#include "menu.h"
#include "device.h"
#include "Session.h"


/*
        ----IMPORTANT BEFORE BUILDING-----

        1. Make sure to run the command: sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev
        and see that the libraries are up to date or the graph feature will simply not work.

            1.1 Also consider adding more memory to the VM if building freezes or is really slow

        2. Building the project takes around 1 minute on my VM due to the graph files size so give it some time.

*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Menu* masterMenu;
    Menu* mainMenuOG;

    Ui::MainWindow *ui;

    Device *device; //User device
    Session *currentSession; //current recorded session to the device

    bool onSkin; //is device attached to user

    QListWidget *activeQListWidget;
    bool powerStatus;

    //rectangles for the coherence level and list of colors
    QList<QRect> coherence_rectangles;
    QList<QColor> colors;


    QTimer* batteryTimer;

    //breath pacer variables
    QTimer* breathTimer;

    bool breathInOut;

    //graph variables
    QTimer *graphTimer; //procedure timer
    double currentTimerCount;
    const int LOW_Y = 38;
    const int HIGH_Y = 110;
    const int LOW_X = 0;
    const int HIGH_X = 250;
    QVector<double> excellnetHRVs; //excellent HRV vector to feed into the graph
    QVector<double> midHRVs; //okayish HRV vector to feed into the graph (has some bad HRV and some good HRV)

//coherene scores to feed into a session
//    QVector<double> basicCoherences;
//    QVector<double> goodCoherences;
//    QVector<double> excellentCoherences;

    QStringList allSessions; //list to store all the current sessions as strings

    void updateMenu(const QString, const QStringList);
    void initializeMainMenu(Menu*);
    void changePowerStatus();
    void generateData(); //generates bad HRV score to feed into the graph that is all over the place
    void updateGraph(); //update a graph x ticks of time
    void extractGraph();
    void beginSession(); //intiazlie a session
    void cleanAfterSession();
    void darkenCoherenceLights();
    void lightenCoherenceLights(int colorIndex);
    void saveSessionData(); //save session to device
    //generate the graph data
    void generateExcellentHRV();
    void generateMidHRV();
    //generate the various coherence scores
    int generateBasicCoherence();
    int generateGoodCoherence();
    int generateExcellentCoherence();
    void runSessionSim(); //run the session simulation
    void updateSummaryScreen(); //update UI summary screen on succesful session end by user
    void showSessionGraph(int index);
private slots:
    void powerChange();
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void navigateToMainMenu();
    void navigateBack();
    void lowerBattery(Device*); //update the battery display on the screen based on the device battey status
    void applyToSkin();
    void breathPacerTimeValueChanged(int arg1);
    void breathPacerMove(int value);

};
#endif // MAINWINDOW_H
