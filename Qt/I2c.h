#ifndef I2C_H
#define I2C_H


class I2c
{
public:
    I2c(int address);
    ~I2c();
    int measure();
    void modeChange(int mode);
private:
    int file;
};

#endif // I2C_H
