#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>

// TempSensor Class
class TempSensor {
    private: 
        void power(){
            //wiringPiI2CWrite(fd, 0x01);
        }

    public:
        int fd;
        void init(){
            fd = wiringPiI2CSetup(0x76);
        }
        int read(){
            //return wiringPiI2CRead(fd);
            return 1234;
        }
};