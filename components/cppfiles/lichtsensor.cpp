#include <wiringPiI2C.h>
#include "../headerfiles/lichtsensor.h"

// Initialisierung vom Licht Sensor
void LichtSensor::init(){
    fd = wiringPiI2CSetup(0x23);
    wiringPiI2CWrite(fd, 0x01);
    wiringPiI2CWrite(fd, 0x10);
}

// Lese den Helligkeitswert aus
long LichtSensor::read(){
    long var = wiringPiI2CRead(fd);
    long lux = ((var << 8) | var)/ 1.2;
    return lux;
}