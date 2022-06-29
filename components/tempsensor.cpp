#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>

// TempSensor Class
class TempSensor {
    private:
        int error_management(){
            if(fd == -1) {
		        std::cout << "Fehler, Temperatur Sensor nicht verfügbar" << "\n";
		        return -1;
	        }
		    else std::cout << "Temperatur Sensor erfolgreich eingerichtet" << "\n";
            return 1;
        }

    public:
        int fd;
        void init(){
            fd = wiringPiI2CSetup(0x76);
            wiringPiI2CWriteReg8(fd, 0xF4, 0x23);
            wiringPiI2CWriteReg8(fd, 0xF5, 0);
            error_management();
        }
        int read(){
            int dig_t1 = (wiringPiI2CReadReg8(fd,0x89) << 8) + wiringPiI2CReadReg8(fd,0x88);
            int dig_t2 = (wiringPiI2CReadReg8(fd,0x8B) << 8) + wiringPiI2CReadReg8(fd,0x8A);
            int dig_t3 = (wiringPiI2CReadReg8(fd,0x8D) << 8) + wiringPiI2CReadReg8(fd,0x8C);
            if(dig_t2 > 32767) {
		        dig_t2 -= 65536;
	        }
	        if(dig_t3 > 32767) {
		        dig_t3 -= 65536;
	        }
            int t_msb1 = wiringPiI2CReadReg8(fd,0xF7);
            int t_msb = wiringPiI2CReadReg8(fd,0xF8);
            int t_lsb = wiringPiI2CReadReg8(fd,0xF9);
            long adc_t = (((long)t_msb1 * 65536) + ((long)t_msb * 256) + (long)(t_lsb & 0xF0)) / 16;
            double var1 = (((double)adc_t) / 16384.0 - ((double)dig_t1) / 1024.0) * ((double)dig_t2);
	        double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_t1) / 8192.0) *(((double)adc_t)/131072.0 - ((double)dig_t1)/8192.0)) * ((double)dig_t3);
	        double cTemp = (var1 + var2) / 5120.0;

            return (int)cTemp;
        }
};