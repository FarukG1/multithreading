#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>

// LichtSensor Class
class LichtSensor {
    private: 
        int error_management(){
            if(fd == -1) {
		        std::cout << "Fehler, Licht Sensor nicht verfügbar" << "\n";
		        return -1;
	        }
		    else std::cout << "Licht Sensor erfolgreich eingerichtet" << "\n";
            return 1;
        }

    public:
        int fd;
        void init(){
            fd = wiringPiI2CSetup(0x23);
            wiringPiI2CWrite(fd, 0x01);
            wiringPiI2CWrite(fd, 0x10);
            error_management();
        }
        int read(){
            return wiringPiI2CRead(fd);
        }
};