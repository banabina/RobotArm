#ifndef GPIO_H
#define GPIO_H

typedef enum _GpioDirection {INPUT, OUTPUT} GpioDirection;
typedef enum _GpioStatus {LOW, HIGH} GpioStatus;
#define PWM_WIDTH (20000)

class Gpio
{
public:
    Gpio(int pin);
    int pin;
    void setPin(int pin);
    int getPin();
    bool setDir(GpioDirection direction);
    int getDir();
    bool setStatus(GpioStatus status);
    int getStatus();
    int getValue();
    bool isExported();
    bool Export();
    bool setAngle(int angle);

};

#endif // GPIO_H
