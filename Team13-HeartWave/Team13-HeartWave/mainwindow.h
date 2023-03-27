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

#include "menu.h"

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

    QListWidget *activeQListWidget;
    bool powerStatus;
    double batteryLvl;

    //rectangles for the coherence level and list of colors
    QList<QRect> coherence_rectangles;
    QList<QColor> colors;


    void updateMenu(const QString, const QStringList);
    void initializeMainMenu(Menu*);
    void changePowerStatus();

private slots:
    void powerChange();
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void navigateToMainMenu();
    void navigateBack();







};
#endif // MAINWINDOW_H
