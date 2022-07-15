#pragma once

class TempSensor {
    private:
        int fd;
        int dig_t1;
        int dig_t2;
        int dig_t3;
    public:
        void init();
        int read();
};