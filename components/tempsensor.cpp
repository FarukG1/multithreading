#include <wiringPiI2C.h>
#include <wiringPi.h>

// TempSensor Class
class TempSensor {
    private:
        int fd;
        int dig_t1;
        int dig_t2;
        int dig_t3;

    public:
    
        // Initialisierung vom Temperatur Sensor
        void init(){
            fd = wiringPiI2CSetup(0x76);
            wiringPiI2CWriteReg8(fd, 0xF4, 0);

            // Konstanten auslesen zur konvertierung/rechnung
            dig_t1 = (wiringPiI2CReadReg8(fd,0x89) << 8) + wiringPiI2CReadReg8(fd,0x88);
            dig_t2 = (wiringPiI2CReadReg8(fd,0x8B) << 8) + wiringPiI2CReadReg8(fd,0x8A);
            dig_t3 = (wiringPiI2CReadReg8(fd,0x8D) << 8) + wiringPiI2CReadReg8(fd,0x8C);
            if(dig_t2 > 32767) {
		        dig_t2 -= 65536;
	        }
	        if(dig_t3 > 32767) {
		        dig_t3 -= 65536;
	        }

            // Sensor einstellen
            wiringPiI2CWriteReg8(fd, 0xF4, 0x83);
            wiringPiI2CWriteReg8(fd, 0xF5, 0x5);
        }

        // Lese die Temperatur aus
        int read(){
            // Wert in Temperatur (Celsius) umrechnen/konvertieren
            int t_msb1 = wiringPiI2CReadReg8(fd,0xFA);
            int t_msb = wiringPiI2CReadReg8(fd,0xFB);
            int t_lsb = wiringPiI2CReadReg8(fd,0xFC);
            long adc_t = (((long)t_msb1 * 65536) + ((long)t_msb * 256) + (long)(t_lsb & 0xF0)) / 16;
            double var1 = (((double)adc_t) / 16384.0 - ((double)dig_t1) / 1024.0) * ((double)dig_t2);
	        double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_t1) / 8192.0) *(((double)adc_t)/131072.0 - ((double)dig_t1)/8192.0)) * ((double)dig_t3);
	        double cTemp = (var1 + var2) / 5120.0;

            return (int)cTemp;
        }
};