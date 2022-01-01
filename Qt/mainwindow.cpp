#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Gpio.h"
#include <unistd.h>
#include <stdio.h>
#include <QDebug>
#include "I2c.h"

#define GPIO(BANK, IO) ((BANK-1)*32+IO)

Gpio* led;
Gpio* conveyorBelt;

Gpio* trig;
Gpio* echo;
I2c* weightSensor;

Gpio* signal1;
Gpio* signal2;
Gpio* signal3;
Gpio* validSignal;

unsigned int curPwm = 1;

float measureDistance();
int moveByConveyorBelt();
int grabFromConveyorBelt();
int moveToWeightSensor();
int measureWeight();
int readSignal();
int grabFromWeightSensor();
int moveToDestination(int measuredWeight);
void listen();
void waitForEndSignal();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    led = new Gpio(21);
    led->setDir(OUTPUT);


    conveyorBelt = new Gpio(GPIO(1, 27));
    conveyorBelt->setDir(OUTPUT);

    // for ultra sonic sensor
    trig = new Gpio(GPIO(1, 26));
    trig->setDir(OUTPUT);

    echo = new Gpio(GPIO(5, 21));
    echo->setDir(INPUT);

    weightSensor = new I2c(0x48);

    signal1 = new Gpio(GPIO(1, 28));
    signal1->setDir(OUTPUT);

    signal2 = new Gpio(GPIO(1, 29));
    signal2->setDir(OUTPUT);

    signal3= new Gpio(GPIO(1, 30));
    signal3->setDir(OUTPUT);

    validSignal = new Gpio(GPIO(5, 20));
    validSignal->setDir(OUTPUT);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete led;
    delete conveyorBelt;

    delete signal1;
    delete signal2;
    delete signal3;
    delete validSignal;

    delete trig;
    delete echo;
    delete weightSensor;

}

void MainWindow::on_smartFactoryStart_clicked()
{
    int cnt = 3;
    ui->label->setText("why");
    while (cnt--)
    {
        ui->state->setText("Moved by Conveyor Belt");
        qDebug() << "Moved by Conveyor Belt";
        moveByConveyorBelt();

        ui->state->setText("Grab From Conveyor Belt start");
        qDebug() << "Grab From Conveyor Belt start";
        grabFromConveyorBelt();
        qDebug() << "Grab From Conveyor Belt complete";

        ui->state->setText("Move to Weight Sensor");
        qDebug() << "Move to Weight Sensor start";
        moveToWeightSensor();
        qDebug() << "Move to Weight Sensor complete";

        ui->state->setText("Weight Meaurement");
        int measuredWeight = measureWeight();
        qDebug() << "measured weight: " << measuredWeight;

        qDebug() << "Move to Final Destination start";
        if (measuredWeight < 0) {
            ui->state->setText("Move to A");
        } else {
            ui->state->setText("Move to B");
        }
        moveToDestination(measuredWeight);
        qDebug() << "Move to Final Destination complete";
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->state->setText("Grapper");
    qDebug() << "Grapper";
    ui->state->setText("Moved by Conveyor Belt");
    qDebug() << "Moved by Conveyor Belt";
    moveByConveyorBelt();

    ui->state->setText("Grab From Conveyor Belt start");
    qDebug() << "Grab From Conveyor Belt start";
    grabFromConveyorBelt();
    qDebug() << "Grab From Conveyor Belt complete";

    ui->state->setText("Move to Weight Sensor");
    qDebug() << "Move to Weight Sensor start";
    moveToWeightSensor();
    qDebug() << "Move to Weight Sensor complete";

    ui->state->setText("Weight Meaurement");
    int measuredWeight = measureWeight();
    qDebug() << "measured weight: " << measuredWeight;

    qDebug() << "Move to Final Destination start";
    if (measuredWeight < 0) {
        ui->state->setText("Move to A");
    } else {
        ui->state->setText("Move to B");
    }
    moveToDestination(measuredWeight);
    qDebug() << "Move to Final Destination complete";

}

void MainWindow::on_pwm2_clicked()
{
    qDebug() << "Conveyor Belt start";
    ui->label->setText("Conveyor Belt");
    moveByConveyorBelt();
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "grab From Conveyor Belt";
    ui->label->setText("grab From Conveyor Belt");
    grabFromConveyorBelt();
    qDebug() << "grabFromConveyorBelt complete";
}

void MainWindow::on_pwm4_clicked()
{
    qDebug() << "move to weight sensor";
    ui->label->setText("move to weight sensor");
    moveToWeightSensor();
    qDebug() << "move to weight sensor complete";
}

void MainWindow::on_grabFromWeightSensor_clicked()
{
    qDebug() << "grab from weight sensor";
    ui->label->setText("grab from weight sensor");
    grabFromWeightSensor();
    qDebug() << "grab from weight sensor complete";
}



void MainWindow::on_pushButton_7_clicked()
{
    qDebug() << "move to Destination A";
    ui->label->setText("move to Destination");
    moveToDestination(-100);
    qDebug() << "move to Destination A complete";
}


void MainWindow::on_moveToB_clicked()
{
    qDebug() << "move to Destination B";
    ui->label->setText("move to Destination");
    moveToDestination(1000);
    qDebug() << "move to Destination B complete";
}



void MainWindow::on_moveToC_clicked()
{
    qDebug() << "move to Destination C";
    ui->label->setText("move to Destination");
    moveToDestination(30000);
    qDebug() << "move to Destination C complete";
}


void MainWindow::on_pushButton_5_clicked()
{
    qDebug() << "Mode change to anaglog 2";
    weightSensor->modeChange(2);
}

void MainWindow::on_ultraSonic_clicked()
{
    qDebug() << "Ultra sonic start";
    ui->label->setText("Ultra sonic");
    float distance = 0.0;
    distance = measureDistance();
    qDebug() << "distance: " << distance << "mm";
}

void MainWindow::on_measure_clicked()
{
    qDebug() << "weight sensor";
    int measuredWeight = weightSensor->measure();
    qDebug() << "measured: " << measuredWeight;
    if (measuredWeight < -500) {
        ui->label->setText("Move to A!");
    } else {
        ui->label->setText("Move to B!");
    }
}

float measureDistance()
{
    float duration;
    float distance;
    int flag = 0;
    clock_t start, end;

    //for debugging
    /*
    if (echo->getStatus()) {
        qDebug() << "echo on";
    } else {
        qDebug() << "echo off";
    }*/

    trig->setStatus(LOW);
    usleep(5);
    start = clock();
    trig->setStatus(HIGH);
    usleep(30);
    trig->setStatus(LOW);

    while (1)
    {
        if ((echo->getStatus() == 1) && flag == 0)
        {
            qDebug() << "echo success";
            flag = 1;
        }
        if ((echo->getStatus() == LOW) && flag == 1)
        {
            end = clock();

            break;
        }

        if (clock() - start > 30000) {
            qDebug() << "echo time out";
            break;
        }
    }
    duration = (float)(end - start);
    distance = (340 * duration / 1000) / 2;
    usleep(2500);
    return distance;
}

int moveByConveyorBelt()
{
    int cnt = 3;
    int measuredDistance = 0;
    while (1)
    {
        conveyorBelt->setAngle(1600);
        measuredDistance = measureDistance();
        if (measuredDistance < 350 && cnt > 0) {
            --cnt;
        } else if (cnt == 0) {
            qDebug() << measuredDistance;
            break;
        }
    }
    return 0;
}

void listen() {
    clock_t start, end;
    weightSensor->modeChange(2);
    start = clock();
    int result = readSignal();
    while (result < 100) {
        end = clock();
        result = readSignal();
        if (end - start > 300000) {
            qDebug() << "time out";
            break;
        }
    }
    weightSensor->modeChange(1);
    signal1->setStatus(LOW);
    signal2->setStatus(LOW);
    signal3->setStatus(LOW);
    validSignal->setStatus(LOW);
    sleep(5);
}

void waitForEndSignal() {
    weightSensor->modeChange(2);
    int result = readSignal();
    while (result < 100) {
        result = readSignal();
    }
    weightSensor->modeChange(1);
    signal1->setStatus(LOW);
    signal2->setStatus(LOW);
    signal3->setStatus(LOW);
    validSignal->setStatus(LOW);
    sleep(5);
}

int grabFromConveyorBelt()
{
    signal1->setStatus(LOW);
    signal2->setStatus(LOW);
    signal3->setStatus(HIGH);
    usleep(30000);
    validSignal->setStatus(HIGH);
    listen();
    waitForEndSignal();
    return 0;
}

int moveToWeightSensor()
{
    signal1->setStatus(LOW);
    signal2->setStatus(HIGH);
    signal3->setStatus(LOW);
    usleep(30000);
    validSignal->setStatus(HIGH);
    listen();
    waitForEndSignal();
    return 0;
}

int measureWeight()
{
    int sum = 0;
    int cnt = 10;
    for (int i = 0; i < cnt; ++i) {
        sum += weightSensor->measure();
    }
    return sum / cnt;
}

int readSignal()
{
    return weightSensor->measure();
}

int grabFromWeightSensor()
{
    signal1->setStatus(LOW);
    signal2->setStatus(HIGH);
    signal3->setStatus(HIGH);
    usleep(30000);
    validSignal->setStatus(HIGH);
    listen();
    waitForEndSignal();
    return 0;
}


int moveToDestination(int measuredWeight)
{
    if (measuredWeight < 0) // move to A
    {
        signal1->setStatus(HIGH);
        signal2->setStatus(LOW);
        signal3->setStatus(LOW);
    } else if (measuredWeight < 20000) // move to B
    {
        signal1->setStatus(HIGH);
        signal2->setStatus(LOW);
        signal3->setStatus(HIGH);
    } else // move to C
    {
        signal1->setStatus(HIGH);
        signal2->setStatus(HIGH);
        signal3->setStatus(LOW);
    }
    usleep(30000);
    validSignal->setStatus(HIGH);
    listen();
    waitForEndSignal();
    return 0;
}

void MainWindow::on_angle200_clicked()
{
    qDebug() << "angle 200";
    if (curPwm == 1) {

    } else if (curPwm == 2) {
        conveyorBelt->setAngle(200);
    } else if (curPwm == 3) {
        signal1->setAngle(200);
    } else {
        signal2->setAngle(200);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << "angle 300";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(300);
    } else if (curPwm == 3) {
        signal1->setAngle(300);
    } else {
        signal2->setAngle(300);
    }
}

void MainWindow::on_angle400_clicked()
{
    qDebug() << "angle 400";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(400);
    } else if (curPwm == 3) {
        signal1->setAngle(400);
    } else {
        signal2->setAngle(400);
    }
}

void MainWindow::on_angle500_clicked()
{
    qDebug() << "angle 500";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(500);
    } else if (curPwm == 3) {
        signal1->setAngle(500);
    } else {
        signal2->setAngle(500);
    }
}

void MainWindow::on_angle600_clicked()
{
    qDebug() << "angle 600";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(600);
    } else if (curPwm == 3) {
        signal1->setAngle(600);
    } else {
        signal2->setAngle(600);
    }
}

void MainWindow::on_angle700_clicked()
{
    qDebug() << "angle 700";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(700);
    } else if (curPwm == 3) {
        signal1->setAngle(700);
    } else {
        signal2->setAngle(700);
    }
}

void MainWindow::on_angle800_clicked()
{
    qDebug() << "angle 800";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(800);
    } else if (curPwm == 3) {
        signal1->setAngle(800);
    } else {
        signal2->setAngle(800);
    }
}

void MainWindow::on_angle900_clicked()
{
    qDebug() << "angle 900";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(900);
    } else if (curPwm == 3) {
        signal1->setAngle(900);
    } else {
        signal2->setAngle(900);
    }
}

void MainWindow::on_angle1000_clicked()
{
    qDebug() << "angle 1000";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1000);
    } else if (curPwm == 3) {
        signal1->setAngle(1000);
    } else {
        signal2->setAngle(1000);
    }
}

void MainWindow::on_angle1100_clicked()
{
    qDebug() << "angle 1100";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1100);
    } else if (curPwm == 3) {
        signal1->setAngle(1100);
    } else {
        signal2->setAngle(1100);
    }
}

void MainWindow::on_angle1200_clicked()
{
    qDebug() << "angle 1200";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1200);
    } else if (curPwm == 3) {
        signal1->setAngle(1200);
    } else {
        signal2->setAngle(1200);
    }
}

void MainWindow::on_angle1300_clicked()
{
    qDebug() << "angle 1300";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1300);
    } else if (curPwm == 3) {
        signal1->setAngle(1300);
    } else {
        signal2->setAngle(1300);
    }
}

void MainWindow::on_angle1400_clicked()
{
    qDebug() << "angle 1400";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1400);
    } else if (curPwm == 3) {
        signal1->setAngle(1400);
    } else {
        signal2->setAngle(1400);
    }
}

void MainWindow::on_angle1500_clicked()
{
    qDebug() << "angle 1500";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1500);
    } else if (curPwm == 3) {
        signal1->setAngle(1500);
    } else {
        signal2->setAngle(1500);
    }
}

void MainWindow::on_angle1600_clicked()
{
    qDebug() << "angle 1600";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1600);
    } else if (curPwm == 3) {
        signal1->setAngle(1600);
    } else {
        signal2->setAngle(1600);
    }
}

void MainWindow::on_angle1700_clicked()
{
    qDebug() << "angle 1700";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1700);
    } else if (curPwm == 3) {
        signal1->setAngle(1700);
    } else {
        signal2->setAngle(1700);
    }
}

void MainWindow::on_angle1800_clicked()
{
    qDebug() << "angle 1800";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1800);
    } else if (curPwm == 3) {
        signal1->setAngle(1800);
    } else {
        signal2->setAngle(1800);
    }
}

void MainWindow::on_angle1900_clicked()
{
    qDebug() << "angle 1900";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(1900);
    } else if (curPwm == 3) {
        signal1->setAngle(1900);
    } else {
        signal2->setAngle(1900);
    }
}

void MainWindow::on_angle2000_clicked()
{
    qDebug() << "angle 2000";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(2000);
    } else if (curPwm == 3) {
        signal1->setAngle(2000);
    } else {
        signal2->setAngle(2000);
    }
}

void MainWindow::on_angle2100_clicked()
{
    qDebug() << "angle 2100";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(2100);
    } else if (curPwm == 3) {
        signal1->setAngle(2100);
    } else {
        signal2->setAngle(2100);
    }
}

void MainWindow::on_angle2200_clicked()
{
    qDebug() << "angle 2200";
    if (curPwm == 1) {
    } else if (curPwm == 2) {
        conveyorBelt->setAngle(2200);
    } else if (curPwm == 3) {
        signal1->setAngle(2200);
    } else {
        signal2->setAngle(2200);
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "led button";
    if (led->getStatus() == LOW) { // led가 LOW 상태이면 HIGH로 만들고, 아니면 LOW로 만듬
        led->setStatus(HIGH);
        ui->label->setText("LED ON!");
    }
    else {
        led->setStatus(LOW);
        ui->label->setText("LED OFF!");
    }
}


