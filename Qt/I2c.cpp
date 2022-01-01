#include "I2c.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <QDebug>

I2c::I2c(int address)
{
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus. \n");
        exit(1);
    }
    // Get I2C device, ADS1115 I2C address is 0x48(72)
    ioctl(file, I2C_SLAVE, address); // address = 0x48 in ads1115

    // Select configuration register(0x01)
    // AINP = AIN0 and AINN = AIN1, +/- 2.048V
    // Continuous conversion mode, 128 SPS(0x84, 0x83)
    char config[3] = {0};
    config[0] = 0x01;
    config[1] = 0x84;   // default mode analog0
    config[2] = 0x83;
    write(file, config, 3);
    sleep(1);
}

I2c::~I2c()
{
    close(file);
}

void I2c::modeChange(int mode)
{
    char config[3] = {0};
    config[0] = 0x01;
    if (mode == 1) {
        config[1] = 0x84;
    } else if (mode == 2) {
        config[1] = 0xb2;
    }
    else {
        return;
    }
    config[2] = 0x83;
    write(file, config, 3);
    sleep(1);
}

int I2c::measure()
{
    // Read 2 bytes of data from register(0x00)
    // raw_adc msb, raw_adc lsb
    char reg[1] = {0x00};
    write(file, reg, 1);

    char data[2]={0};
    if(read(this->file, data, 2) != 2)
    {
        qDebug() << "Error : Input/Output Error \n";
    }
    else
    {
        // Convert the data
        int raw_adc = (data[0] * 256 + data[1]);
        if (raw_adc > 32767)
        {
            raw_adc -= 65535;
        }

        // Output data to screen
        //printf("Digital Value of Analog Input: %d \n", raw_adc);
        return raw_adc;
    }
    return -1;
}
