#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "device.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //make the graph timer
    currentTimerCount = -1;
    graphTimer = new QTimer(this);
    connect(graphTimer, &QTimer::timeout, this, &MainWindow::updateGraph);

    //create menu tree
    masterMenu = new Menu("MAIN MENU", {"START NEW SESSION","SETTINGS","HISTORY"}, nullptr);
    mainMenuOG = masterMenu;
    device = new Device(100);

    initializeMainMenu(masterMenu);
    //set up current session to be null at first
    currentSession = NULL;

    // Initialize the main menu view
    activeQListWidget = ui->mainList;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());

    // Account for device being "off" on sim start
    powerStatus = false;
    onSkin = false;
    //hide new session menu
    ui->customPlot->setVisible(powerStatus);
    ui->parametersViewWidget->setVisible(powerStatus);
    ui->ballPacerPlaceHold->setVisible(powerStatus);
    changePowerStatus();
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerChange()));

    //SET UP BATTERY

    ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #1FE058; }");

    ui->batteryProgress->setValue(device->getBatteryLevel());

    connect(ui->chargeDevice, &QPushButton::clicked, this, [=]() {
            device->setFullCharge();
            ui->batteryProgress->setValue(device->getBatteryLevel());
        });

    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this, [=]() {
        if (powerStatus && device->getBatteryLevel() > 0){
        lowerBattery(device);
        }
    });

    // every 5 seconds the function above will be called
    batteryTimer->start(5000);


    //TODO: RIGHT & LEFT BUTTONS
    // device interface button connections
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(navigateUpMenu()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(navigateDownMenu()));
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(navigateSubMenu()));
    connect(ui->menuButton, SIGNAL(pressed()), this, SLOT(navigateToMainMenu()));
    connect(ui->backButton, SIGNAL(pressed()), this, SLOT(navigateBack()));
    //set up the apply to heart combo box
    ui->applyToHeartBox->addItem("False");
    ui->applyToHeartBox->addItem("True");
    connect(ui->applyToHeartBox, SIGNAL(currentIndexChanged(int)),this,SLOT(applyToSkin()));


    //SET UP COLOR LIGHTS
    coherence_rectangles.append(QRect(120, 65, 70, 15)); // Create the first rectangle
    coherence_rectangles.append(QRect(200, 65, 70, 15)); // Create the second rectangle
    coherence_rectangles.append(QRect(280, 65, 70, 15)); // Create the third rectangle
    colors.append(Qt::red);
    colors.append(Qt::blue);
    colors.append(Qt::green);

    QColor darkerColor;


    //darken each color
    darkerColor = colors.at(2).darker(450);
    colors.replace(2, darkerColor);

    darkerColor = colors.at(1).darker(450);
    colors.replace(1, darkerColor);

    darkerColor = colors.at(0).darker(450);
    colors.replace(0, darkerColor);


    //SET UP GRAPH
    // add new graph and set their look:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph

    //set text on y&x axis
    ui->customPlot->yAxis->setLabel("Heart Rate");
    ui->customPlot->xAxis->setLabel("Time (seconds)");

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->customPlot->graph(0)->rescaleAxes();
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //set ranges on x & y axis
    ui->customPlot->yAxis->setRange(LOW_Y,HIGH_Y);
    ui->customPlot->xAxis->setRange(LOW_X,HIGH_X);

    this->update();
}

//TODO:


//LEFT & RIGHT button initialiation
//LEFT & RIGHT button control

//Proper SubMenu Navigation in navigateSubMenu() for SESSION, SETTINGS & HISTORY
//SESSION score types displayed (coherence, level, achievement)
//HR Contact icon

//GRAPH display and update (graph module)

//LEDs behaviour (green, blue, red)

//SETTINGS implementation (change level, difficulty, reset device)

//HISTORY LOG Implemenation (view sessions, delete history)

//COHERENCE LEVEL SOUND module



MainWindow::~MainWindow()
{
    delete mainMenuOG;
    delete batteryTimer;
    delete graphTimer;
    delete device;
    delete ui;
}




//Apply the device to user to measure his stuff
void MainWindow::applyToSkin() {
    //get current index
   int status = ui->applyToHeartBox->currentIndex();
   //update onSkin requirement
   //If true text is selected apply to skin
   if(status == 1){
        onSkin = true;
    }
    //else it is false
   else{
      onSkin = false;
   }
    //update the QPixMap
    ui->heartPicLabel->setPixmap(QPixmap(onSkin ? ":/buttons/heart-ON.svg" : ":/buttons/heart-OFF.svg"));

}


void MainWindow::generateData(){
    //generate number between 40-100 put it on y
    srand (time(NULL));
    int temp = rand()%61+40;                //needs to be double
    ui->customPlot->graph(0)->addData(currentTimerCount,(double)temp);
    if(currentTimerCount > 200){
        graphTimer->stop(); //end session primarly for testing will move over to a diff spot later
    }
    currentTimerCount+= 1;//timer is every 1 seconds so plot every 1
}

void MainWindow::updateGraph(){
    //generate some data & add to graph
    generateData();
    //draw out graph again to refelect new data
    ui->customPlot->replot();
}
//extract the graph content into a given session in the params
//will be super akward cuz qCustomPlot only allows const stuff to go in-out....
void MainWindow::extractGraph(){
    //type data() returns is weird so we can cheat with auto lol
//    auto plotData = ui->customPlot->graph(0)->data();
//    QVector<double> xVals = new QVector();
//    QVector<double> yVals = new QVector();
//    for (int i = 0 ; i < plotData->size() ; ++i) {
//         double lastKey = plotData->at(i)->key;
//         double lastValue = plotData->at(i)->value;
//         qDebug () << "X:" << i << lastKey;
//         qDebug () << "Y:" << i << lastValue;
//        xVals.append(lastKey);
//        yVals.append(lastValue);
//    }
}

//draw each rectangle
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < coherence_rectangles.size(); i++) {
        painter.setBrush(colors.at(i));
        painter.setPen(Qt::NoPen);
        painter.drawRect(coherence_rectangles.at(i));
    }
}

void MainWindow::lowerBattery(Device *d)
{
    d->setBatteryLevel(d->getBatteryLevel() -1);

    if(d->getBatteryLevel() <= 100 && d->getBatteryLevel() >=50 ){
        ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #1FE058; }");
    }

    else if(d->getBatteryLevel() <= 49 && d->getBatteryLevel() >= 20 ){
        ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #C3DC23; }");
    }

    else if(d->getBatteryLevel() <= 19 && d->getBatteryLevel() >= 1 ){
        ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #EB1419; }");
    }

    //turn the device off and recharge the battery to full
    else{

        powerChange();
        d->setFullCharge();
    }
    ui->batteryProgress->setValue(d->getBatteryLevel());

}

void MainWindow::powerChange(){
    //inverse power status and update UI based of that.
    if(powerStatus){
            powerStatus = false;
        }else{
            powerStatus = true;
    }
    changePowerStatus();
}

void MainWindow::changePowerStatus() {
    activeQListWidget->setVisible(powerStatus);
    ui->menuLabel->setVisible(powerStatus);
    ui->batteryProgress->setVisible(powerStatus);
    ui->chargeDevice->setVisible(powerStatus);
//    ui->statusBarQFrame->setVisible(powerStatus);
//    ui->treatmentView->setVisible(powerStatus);
//    ui->frequencyLabel->setVisible(powerStatus);
//    ui->programViewWidget->setVisible(powerStatus);

//    //Remove this if we want the menu to stay in the same position when the power is off
//    if (powerStatus) {
//        MainWindow::navigateToMainMenu();
//        ui->applyToSkinButton->setChecked(false);
//        applyToSkin(false);
//    }

    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->leftButton->setEnabled(powerStatus);
    ui->rightButton->setEnabled(powerStatus);
    ui->menuButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->backButton->setEnabled(powerStatus);  
    ui->menuFrame->setVisible(powerStatus);
    ui->heartPicLabel->setEnabled(powerStatus);

}

void MainWindow::initializeMainMenu(Menu* m) {

//    QStringList programsList;


//    for (Therapy* p : this->programs) {
//        programsList.append(p->getName());
//    }


    Menu* session = new Menu("START NEW SESSION", {}, m);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE LEVEL","RESET DEVICE","BREATH PACER"}, m);
    Menu* history = new Menu("HISTORY", {"VIEW"}, m);

    m->addChildMenu(session);
    m->addChildMenu(settings);
    m->addChildMenu(history);


//    for (Therapy* p : this->programs) {
//        programs->addChildMenu(new Menu(p->getName(), {}, programs));
//    }

    Menu* viewHistory = new Menu("VIEW",{}, history);
    Menu* clearDevice = new Menu("RESET DEVICE", {"YES","NO"}, settings);
    Menu* challengeLvls = new Menu("CHALLENGE LEVEL", {"1","2","3","4"},settings);


    history->addChildMenu(viewHistory);
    settings->addChildMenu(challengeLvls);
    settings->addChildMenu(clearDevice);
     //TODO: Breath Pacer child menu selector that is controlled with the side arrows
}


void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}


void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}


void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    // Prevent crash if ok button is selected in view
    if (masterMenu->getName() == "VIEW") {
        return;
    }


      //Logic for when the menu is the delete menu.
//    if (masterMenu->getName() == "CLEAR") {
//        if (masterMenu->getMenuItems()[index] == "YES") {
//            db->deleteRecords();
//            allRecordings.clear();

//            for (int x = 0; x < recordings.size(); x++) {
//                delete recordings[x];

//            recordings.clear();
//            navigateBack();
//            return;
//        }
//        else {
//            navigateBack();
//            return;
//        }
//    }

    //Logic for when the menu is the settings challenge menu
    if(masterMenu->getName() == "CHALLENGE LEVEL"){
        device->setChallengeLevel(index+1);
        navigateBack();
        return;
    }
    //Logic for when reset device settings menu option is selected
    if(masterMenu->getName() == "RESET DEVICE"){
        if (masterMenu->getMenuItems()[index] == "YES") {
              currentSession = NULL;
              device->resetSettings();
              navigateBack();
              return;
         }
        else{
            navigateBack();
            return;
        }
    }

    //TODO: Logic for when the menu is the settings breath pacer menu (arrow keys and stuff)


    //If the menu is a parent and clicking on it should display more menus.
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
    //If the menu is has no items in it i.e it should start a session
    else if (masterMenu->get(index)->getMenuItems().length() == 0) {
        // && masterMenu->getName() == "START NEW SESSION"
        //TODO: Start a new session with params
        beginSession();
    }
    //If the button pressed should display the device's recordings.
    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        //MainWindow::updateMenu("RECORDINGS", allRecordings); <-- TODO: display recording modify
    }

}
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(selectedMenuItem);
}

void MainWindow::beginSession(){
   if(onSkin == false){
       return;
   }
    currentTimerCount = 0;
    graphTimer->start(1000);
    ui->customPlot->setVisible(true);
    ui->parametersViewWidget->setVisible(true);
    ui->ballPacerPlaceHold->setVisible(true);
    //Make new session object

    //Set it as the current session

    //Do CurrTime % 5 that way every 5 seconds calc stuff

}

void MainWindow::navigateToMainMenu() {
//TODO: Properly save the recording
    int minRecordingTime = 5;
    //in case of valid session save progress first before clean up
    if (currentTimerCount >= minRecordingTime) {
        //Save recording
//        if (masterMenu->getParent()->getName() == "PROGRAMS") {
//            recordings.last()->setDuration((currentTherapy->getTime())-currentTimerCount);
//            recordings.last()->setPowerLevel(maxPowerLevel);
//            db->addTherapyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
//        }
//        else {
//            recordings.last()->setDuration(currentTimerCount);
//            recordings.last()->setPowerLevel(maxPowerLevel);
//            db->addFrequencyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
//        }

//        allRecordings += recordings.last()->toString();

//        //End therapy
//        currentTimerCount = -1;
//        currentTherapy->getTimer()->stop();
//        currentTherapy->getTimer()->disconnect();
//        currentTherapy = nullptr;

    }
    //If even a non-valid session was started clean up the UI screen
    if(currentTimerCount != -1){
         cleanAfterSession();
    }

    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());

}

//hide UI buttons and clean variables after the session is finished
void MainWindow::cleanAfterSession(){
    //TODO: account for ball pacer when it is implemented to update it properly
    graphTimer->start(1000); //restarts the timer
    graphTimer->stop(); //then stop it
    ui->customPlot->setVisible(false);
    ui->parametersViewWidget->setVisible(false);
    ui->ballPacerPlaceHold->setVisible(false);
    //adjust timer
    currentTimerCount = -1;
    //adjust labels
    ui->coherenceLabel->setText("0");
    ui->achievmentLabel->setText("0");
    ui->lengthLabel->setText("0");

    ui->customPlot->graph()->data()->clear(); //clear data
    ui->customPlot->replot(); //update the graph
}

void MainWindow::navigateBack() {
//TODO: Properly save the recording

    int minRecordingTime = 5;
    if (currentTimerCount >= minRecordingTime) {
        //Save recording
    }
    //If any session even if non-valid was started clean up
    if (currentTimerCount != -1) {
         cleanAfterSession();
    }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
}
