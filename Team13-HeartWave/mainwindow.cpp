#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //create menu tree
    masterMenu = new Menu("MAIN MENU", {"START NEW SESSION","SETTINGS","HISTORY"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    // Initialize the main menu view
    activeQListWidget = ui->mainList;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());

    // Account for device being "off" on sim start
    powerStatus = false;
    changePowerStatus();
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerChange()));

    //TODO: BATTERY

    //TODO: RIGHT & LEFT BUTTONS
    // device interface button connections
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(navigateUpMenu()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(navigateDownMenu()));
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(navigateSubMenu()));
    connect(ui->menuButton, SIGNAL(pressed()), this, SLOT(navigateToMainMenu()));
    connect(ui->backButton, SIGNAL(pressed()), this, SLOT(navigateBack()));


}

//TODO:

//Battery Initilaziation
//Battery Level display
//Batter level control

//LEFT & RIGHT button initialiation
//LEFT & RIGHT button control

//Proper SubMenu Navigation in navigateSubMenu() for SESSION, SETTINGS & HISTORY
//SESSION score types displayed (coherence, level, achievement)
//HR Contact icon

//GRAPH display and update (graph module)

//LEDs behaviour (green, blue, red)

//Icons on the buttons

//SETTINGS implementation (change level, difficulty, reset device)

//HISTORY LOG Implemenation (view sessions, delete history)

//COHERENCE LEVEL SOUND module



MainWindow::~MainWindow()
{
    delete mainMenuOG;
    delete ui;
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
//    ui->applyToSkinButton->setEnabled(powerStatus);
//    ui->applyToSkinAdminBox->setEnabled(powerStatus);
}

void MainWindow::initializeMainMenu(Menu* m) {

//    QStringList frequenciesList;
//    QStringList programsList;

//    for (Therapy* f : this->frequencies) {
//        frequenciesList.append(f->getName());
//    }
//    for (Therapy* p : this->programs) {
//        programsList.append(p->getName());
//    }

    Menu* session = new Menu("SESSION", {}, m);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE LEVEL","BREATH PACER"}, m);
    Menu* history = new Menu("HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(session);
    m->addChildMenu(settings);
    m->addChildMenu(history);

//    for (Therapy* f : this->frequencies) {
//        frequencies->addChildMenu(new Menu(f->getName(), {}, frequencies));
//    }

//    for (Therapy* p : this->programs) {
//        programs->addChildMenu(new Menu(p->getName(), {}, programs));
//    }

    Menu* viewHistory = new Menu("VIEW",{}, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);
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

//    //Logic for when the menu is the delete menu.
//    if (masterMenu->getName() == "CLEAR") {
//        if (masterMenu->getMenuItems()[index] == "YES") {
//            db->deleteRecords();
//            allRecordings.clear();

//            for (int x = 0; x < recordings.size(); x++) {
//                delete recordings[x];
//            }

//            recordings.clear();
//            navigateBack();
//            return;
//        }
//        else {
//            navigateBack();
//            return;
//        }
//    }

    //If the menu is a parent and clicking on it should display more menus.
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());


    }
    //If the menu is not a parent and clicking on it should start a therapy
    else if (masterMenu->get(index)->getMenuItems().length() == 0 && masterMenu->getName() == "SESSION") {
        if (masterMenu->getName() == "SESSION") {
            //Update new menu info
            masterMenu = masterMenu->get(index);
            //MainWindow::updateMenu(programs[index]->getName(), {});
            //MainWindow::beginTherapy(programs[index]);
        }
//        else if (masterMenu->getName() == "FREQUENCIES") {
//            masterMenu = masterMenu->get(index);
//            MainWindow::updateMenu(frequencies[index]->getName(), {});
//            MainWindow::beginTherapy(frequencies[index]);
//        }
    }
    //If the button pressed should display the device's recordings.
    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        //MainWindow::updateMenu("RECORDINGS", allRecordings);
    }
}
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(selectedMenuItem);
}


void MainWindow::navigateToMainMenu() {
//    if (currentTimerCount != -1) {
//        //Save recording
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
//    }

    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
//    ui->programViewWidget->setVisible(false);
//    ui->electrodeLabel->setVisible(false);
}


void MainWindow::navigateBack() {

    ui->rightButton->blockSignals(true);
    ui->leftButton->blockSignals(true);

//    if (currentTimerCount != -1) {
//        //Save recording
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

//        //Stop therapy
//        currentTimerCount = -1;
//        currentTherapy->getTimer()->stop();
//        currentTherapy->getTimer()->disconnect();
//        currentTherapy = nullptr;
//    }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

//    ui->programViewWidget->setVisible(false);
//    ui->electrodeLabel->setVisible(false);
}
