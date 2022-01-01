#include "Gpio.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <unistd.h>

Gpio::Gpio(int pin) {
    this->setPin(pin);

    if (!this->isExported())   //입력받은 pin번호의 GPIO가 export 되어 있는지 확인
        this->Export();
 }

void Gpio::setPin(int pin) {
    this->pin = pin;
}

int Gpio::getPin() {
    return this->pin;
}
bool Gpio::setDir(GpioDirection direction) {     // 객체에서 사용중인 pin의 입/출력을 설정
    QString url;
    url.append("/sys/class/gpio/gpio");
    url.append(QString::number(this->getPin()));
    url.append("/direction");
    QFile file(url);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadWrite))
        return false;
    QString data;
    if (direction == INPUT) {
        data.append("in");
    } else
    {
        data.append("out");
    }
    file.write(data.toAscii());
    file.close();
    return true;
}

int Gpio::getDir() {
    QString url;
    url.append("/sys/class/gpio/gpio");
    url.append(QString::number(this->getPin()));
    url.append("/direction");
    QFile file(url);
    if (!file.exists())
        return NULL;
    if (!file.open(QIODevice::ReadWrite))
        return NULL;
}
bool Gpio::setStatus(GpioStatus status) {   // 객체에서 사용중인 GPIO의 OUTPUT data 설정
    QString url;
    url.append("/sys/class/gpio/gpio");
    url.append(QString::number(this->getPin()));
    url.append("/value");
    QFile file(url);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadWrite))
        return false;
    QString data;
    if (status == HIGH) {
        data.append("1");
    }
    else {
        data.append("0");
    }
    file.write(data.toAscii());
    file.close();
    return true;
}
int Gpio::getStatus() {   //객체에서 사용 중인 GPIO pin의 현재 상태 출력
    QString url;
    url.append("/sys/class/gpio/gpio");
    url.append(QString::number(this->getPin()));
    url.append("/value");
    QFile file(url);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadWrite))
        return false;

    QByteArray datas;
    datas = file.read(1);

    if (datas[0] == '0')
        return LOW;
    else
        return HIGH;
}

int Gpio::getValue() {   //객체에서 사용 중인 GPIO pin의 현재 상태 출력
    QString url;
    url.append("/sys/class/gpio/gpio");
    url.append(QString::number(this->getPin()));
    url.append("/value");
    QFile file(url);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadWrite))
        return false;

    QByteArray datas;
    datas = file.read(4);
    qDebug() << "Debug ing" << datas[0];
    return (sizeof(datas) / sizeof(QByteArray));
}

bool Gpio::isExported() {    //객체에서 사용 중인 GPIO가 export되어있는지 출력
    QString url;
    url.append("/sys/class/gpio/gpio");
    url.append(QString::number(this->getPin()));
    QDir dir(url);
    return dir.exists();
}

bool Gpio::Export() { //객체에서 사용 중인 GPIO를 export
    QString url;
    url.append("/sys/class/gpio/export");
    QFile file(url);
    if (!file.exists())
    return false;
    if (!file.open(QIODevice::WriteOnly))
    return false;
    QString data;
    data.append(QString::number(this->getPin()));
    file.write(data.toAscii());
    file.close();
    return true;
}
/*
 * if angle == 300, servo motor angle = 0
 * if angle == 2150, servo motor angle = 180
*/
bool Gpio::setAngle(int angle)
{
    /*
    for (int i = 0; i < 50; ++i)
    {
        this->setStatus(HIGH);
        usleep(angle);
        this->setStatus(LOW);
        usleep(PWM_WIDTH - 1000);
    }
    */
    this->setStatus(HIGH);
    usleep(angle);
    this->setStatus(LOW);
    usleep(PWM_WIDTH - 1000);
}
