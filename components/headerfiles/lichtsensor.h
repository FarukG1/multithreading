#pragma once

// LichtSensor Class
class LichtSensor {
    private:
        int fd;
    public:
        void init();
        long read();
};