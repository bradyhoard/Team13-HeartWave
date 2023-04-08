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
    ui->breath_pacer->setTextVisible(powerStatus);
    changePowerStatus();
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerChange()));

    //SET UP BATTERY

    ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #1FE058; }");

    ui->batteryProgress->setValue(device->getBatteryLevel());

    connect(ui->chargeDevice, &QPushButton::clicked, this, [=]() {
            device->setFullCharge();
            ui->batteryProgress->setValue(device->getBatteryLevel());
            ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #1FE058; }");
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
    //to increase the breath pacer time
    connect(ui->breath_pacer_time,SIGNAL(valueChanged(int )),this,SLOT(breathPacerTimeValueChanged(int )));

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

    //set up the coherence level box so user can select vals from it
    ui->coherenceVal_Box->addItem("Poor");
    ui->coherenceVal_Box->addItem("Good");
    ui->coherenceVal_Box->addItem("Excellent");

     //hide session summary view on load
    ui->summaryWidget->setVisible(powerStatus);

    this->update();
    // time for breath pacer movment

    breathTimer = new QTimer(this);
    // breath in / out variable
    breathInOut = true;

}


//TODO:

//LEFT & RIGHT button initialiation
//LEFT & RIGHT button control

//Proper SubMenu Navigation in navigateSubMenu() for SETTINGS & HISTORY
//SESSION score types displayed (coherence, level, achievement)

//GRAPH saving

//SETTINGS implementation (change level, difficulty, reset device)

//HISTORY LOG Implemenation (view sessions, delete history)

//COHERENCE LEVEL SOUND module



MainWindow::~MainWindow()
{
    delete mainMenuOG;
    delete batteryTimer;
    delete breathTimer;
    delete graphTimer;
    delete device;
    delete ui;
}




//Apply the device to user to measure his vitals
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

    QString coherenceLevel = ui->coherenceVal_Box->currentText();

    //red
    if (coherenceLevel == "Poor"){
         lightenCoherenceLights(0);
    }

    //blue
    else if (coherenceLevel == "Good"){
         lightenCoherenceLights(1);
    }

    //green
    else{
         lightenCoherenceLights(2);
    }

}

void MainWindow::updateGraph(){
    //generate some data & add to graph
    generateData();
    //draw out graph again to refelect new data
    ui->customPlot->replot();
}


//Extracts all the points X (keys) and Y (values) from the graph and saves them
//to the session vectors that hold all points X and Y recorded by the device to be displayed later
//in history logs to the user.
void MainWindow::extractGraph(){
    //type data() returns is weird so we can cheat with auto keyword lol
    auto plotData = ui->customPlot->graph(0)->data();
    for (int i = 0 ; i < plotData->size() ; ++i) {
         double lastKey = plotData->at(i)->key;
         double lastValue = plotData->at(i)->value;
         qDebug () << "X:" << i << lastKey;
         qDebug () << "Y:" << i << lastValue;
        currentSession->addPointX(lastKey);
        currentSession->addPointY(lastValue);
    }
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

//darken each color , this is called when a session is currently not in progress
void MainWindow::darkenCoherenceLights(){
    int red = colors.at(0).red();
    int blue = colors.at(1).blue();
    int green = colors.at(2).green();
    QColor darkerColor;


   if (red == 255){
       darkerColor = colors.at(0).darker(450);
       colors.replace(0, darkerColor);
   }

   if (blue == 255){
       darkerColor = colors.at(1).darker(450);
       colors.replace(1, darkerColor);
   }

   if (green == 255){
       darkerColor = colors.at(2).darker(450);
       colors.replace(2, darkerColor);
   }
   update();

}

/* int colorIndex = color to be lightened up
 * 0 - Red
 * 1 - Blue
 * 2 - Green
*/
void MainWindow::lightenCoherenceLights(int colorIndex){
    int red = colors.at(0).red();
    int blue = colors.at(1).blue();
    int green = colors.at(2).green();


    //light red
    if (colorIndex == 0 && red != 255){

        QColor lighterColor = colors.at(0).lighter(450);
        colors.replace(0, lighterColor);

        QColor darkerColor;

        if (blue == 255){
            darkerColor = colors.at(1).darker(450);
            colors.replace(1, darkerColor);
        }

        if (green == 255){
            darkerColor = colors.at(2).darker(450);
            colors.replace(2, darkerColor);
        }

    }

    //light blue
    else if (colorIndex == 1  && blue != 255){
        QColor lighterColor = colors.at(1).lighter(450);
        colors.replace(1, lighterColor);

        QColor darkerColor;

        if (red == 255){
            darkerColor = colors.at(0).darker(450);
            colors.replace(0, darkerColor);
        }


        if (green == 255){
            darkerColor = colors.at(2).darker(450);
            colors.replace(2, darkerColor);
        }
    }

    //light green
    else if (colorIndex == 2 && green != 255){
        QColor lighterColor = colors.at(2).lighter(450);
        colors.replace(2, lighterColor);

        QColor darkerColor;


        if (red == 255){
            darkerColor = colors.at(0).darker(450);
            colors.replace(0, darkerColor);
        }


        if (blue == 255){
            darkerColor = colors.at(1).darker(450);
            colors.replace(1, darkerColor);
        }
    }

    update();


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
         ui->batteryProgress->setStyleSheet("QProgressBar::chunk { background-color: #1FE058; }");
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
    //Currently if turning device off during a session just hide components and thats it.
    ui->summaryWidget->setVisible(false);
    ui->parametersViewWidget->setVisible(false);
    ui->customPlot->setVisible(false);
    ui->breath_pacer->setVisible(false);
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
    //If the menu is has no items and the timer is not started -> it should start a session
    else if (masterMenu->get(index)->getMenuItems().length() == 0 && currentTimerCount == -1) {
        beginSession();

    }//If the menu is has no items BUT current timer is started it means session is in progress and user wants to stop it.
    else if(masterMenu->get(index)->getMenuItems().length() == 0 && currentTimerCount != -1){
        ui->summaryWidget->setVisible(true); //show extra summary info
        ui->ballPacerPlaceHold->setVisible(false);//hide the breath paser
        int minRecordingTime = 5;
        if (currentTimerCount >= minRecordingTime) {
            //Save session here as well
            saveSessionData();
        }
        //Otherwise just stop the data feed and show summary view to user
        //Assumed: User will then use go back or menu button to return out of the session
        graphTimer->start(1000); //restarts the timer
        graphTimer->stop(); //then stop it
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
    ui->breath_pacer->setVisible(true);
    //Make new session object
    Session* s = new Session(device->getChallengeLevel(),QDateTime::currentDateTime());
    //Set it as the current session
    currentSession = s;
    //Call functions tied to timer that update UI parameters and later will be extracted to save a session.
    //start the breath pacer
    ui->breath_pacer->setRange(0,30);
    ui->breath_pacer->setMinimum(0);
    ui->breath_pacer->setMaximum(device->getBreathPacer());
    ui->breath_pacer->setTextVisible(false);
    ui->breath_pacer->setValue(1);


    connect(breathTimer, &QTimer::timeout, this, [=]() {

        breathPacerMove(1);

        }
  );

    // every 1 seconds the function above will be called

    breathTimer->start(1000);



}


void MainWindow::navigateToMainMenu() {
//TODO: Properly save the recording
    int minRecordingTime = 5;
    //in case of valid session save progress first before clean up
    if (currentTimerCount >= minRecordingTime && currentSession != NULL) {
        //Save recording
        saveSessionData();
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
    //stop the breath timer and reset the breath time to 10 seconds.
    breathTimer->start(1000);
    breathTimer->stop();
    device->setBreathPacer(10);

    ui->customPlot->setVisible(false);
    ui->parametersViewWidget->setVisible(false);
    ui->ballPacerPlaceHold->setVisible(false);
    ui->summaryWidget->setVisible(false);
    ui->breath_pacer->setVisible(false);
    //adjust timer
    currentTimerCount = -1;
    //adjust labels
    ui->coherenceLabel->setText("0");
    ui->achievmentLabel->setText("0");
    ui->lengthLabel->setText("0");
    ui->challengeLvl_label->setText("0");
    ui->avgCoherence_label->setText("0");

    ui->pTimeLow_label->setText("0");
    ui->pTimeHigh_label->setText("0");
    ui->pTimeMed_label->setText("0");

    ui->customPlot->graph()->data()->clear(); //clear data
    ui->customPlot->replot(); //update the graph

    //darken the colors
    darkenCoherenceLights();
}

void MainWindow::navigateBack() {
//TODO: Properly save the recording

    int minRecordingTime = 5;
    if (currentTimerCount >= minRecordingTime && currentSession != NULL) {
        //Save recording
        saveSessionData();
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
//Save all the current session parameters to the device
void MainWindow::saveSessionData(){
    //save session gathered parameters (either take from UI or have it gathering and saving every 5 seconds atutomatically)

    //save graph data
    extractGraph();
    //append current session to the device vector that will handle de-alloc
    device->addSession(currentSession);
    //set current session back to null
    currentSession = NULL;
}
//change the value of breath time
void MainWindow::breathPacerTimeValueChanged(int arg1)
{

    device->setBreathPacer(arg1);

}
// the breath ball going back and forth
void MainWindow::breathPacerMove(int value)
{

    int pacer_value = ui->breath_pacer->value();

    //the ball moves forth in breath in  for half of breath time

    if(pacer_value<=(device->getBreathPacer()-1)/2 && breathInOut){

        ui->breath_pacer->setValue(ui->breath_pacer->value()+value);
        QTime endTime1 = QTime::currentTime().addSecs(1);
        while(QTime::currentTime()<endTime1){

        }

    }// the breath out start
    if (pacer_value>(device->getBreathPacer()-1)/2) {

        breathInOut = false;

    // the ball ,obe back in breath out for the half of braeth time
    }if(pacer_value>0 && (breathInOut==false)){
            ui->breath_pacer->setValue(ui->breath_pacer->value()- value);

            QTime endTime2 = QTime::currentTime().addSecs(1);
            while(QTime::currentTime()<endTime2){

    }
    }else{// breath in satrt again
                 breathInOut = true;

          }
        }






