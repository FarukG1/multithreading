#pragma once

class DisplayController {
    private:
        // Number besitzt die einzelnen Bits für die Ziffern 0-9
        // Das letzte Element ist ein Blankes Symbol
        int number[11][4] = {
            {0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},
            {0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},{1,0,0,1}, {1,1,1,1}};
        // Die Stelle an dem man die Ziffer haben möchte
        int digit[8][4] =  {
            {0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},
            {0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},};
        // Bits für das Zeichen C
        int C[12] = {1,0,0,0, 0,1,0,0, 1,1,1,0};
        // Bits für das Zeichen H
        int H[12] = {1,0,0,0, 0,0,1,1, 0,1,1,1};
        void writeZero(int count);
        void writeOne(int count);
        void testModeOff();
        void scanLimit();
        void codeB();
        void drawNumber(int index, int num);
        void powerOn();
        void drawChar(int turn);
        void setIntensity();
    public:
        void resetDisplay();
        void init();
        void show(int num, int turn);
};