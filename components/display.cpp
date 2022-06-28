#include <wiringPi.h>
#include <iostream>
#include <unistd.h>

#define HIGH 1
#define LOW 0

#define DIN 27
#define LOAD 28
#define CLK 29

// Display Controller Class
class DisplayController {
    private:
        int number[11][4] = {
            {0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},
            {0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},{1,0,0,1}, {1,1,1,1}};
        int digit[8][4] =  {
            {0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},
            {0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},};
        int C[12] = {1,0,0,0, 0,1,0,0, 1,1,1,0};
        int H[12] = {1,0,0,0, 0,0,1,1, 0,1,1,1};
        void write4BitsZero(){
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, 0);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
        }
        void write4BitsOne(){
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, 1);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
        }
        void testModeOff(){
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            write4BitsZero(); // 0000
            write4BitsOne();  // 1111
            write4BitsZero(); // 0000
            write4BitsZero(); // 0000
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }
        void codeB(){
            int bits[] = {1,0,0,1};
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            write4BitsZero();
	        for(int i = 0; i < 4; i++){
		        digitalWrite(DIN, bits[i]);
                digitalWrite(CLK, HIGH);
		        digitalWrite(CLK, LOW);
	        }
            write4BitsZero();
            write4BitsOne();
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }
        void drawNumber(int index, int num){
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            write4BitsZero();
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, digit[index][i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            write4BitsZero();
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, number[num][i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }
        void resetDisplay(){
            for(int i = 0; i < 4; i++){
                drawNumber(i,10);
            }
            for(int i = 4; i < 8; i++){
                drawNumber(i,0);
            }
        }
        void powerOn(){
            int bits[] = {1,1,0,0};
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            write4BitsZero();
	        for(int i = 0; i < 4; i++){
		        digitalWrite(DIN, bits[i]);
                digitalWrite(CLK, HIGH);
		        digitalWrite(CLK, LOW);
	        }
            write4BitsOne();
            write4BitsOne();
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }
        void drawChar(int turn){
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            write4BitsZero();
            for(int i = 0; i < 12; i++){
                if(turn == 1) digitalWrite(DIN,H[i]);
                if(turn == 2) digitalWrite(DIN,C[i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

    public:
        void init(){
            wiringPiSetup();
            std::cout << "Display wird eingerichtet" << "\n";
            pinMode(DIN, OUTPUT);
            pinMode(LOAD, OUTPUT);
 	        pinMode(CLK, OUTPUT);
            powerOn();
            codeB();
            testModeOff();
            sleep(1);
            resetDisplay();
	        std::cout << "Display erfolgreich eingerichtet" << "\n";
        }
        void show(int num, int turn){
            int first = (num / 1000);
            int second = (num / 100) % 10;
            int third = (num / 10) % 10;
            int last = num % 10;
            //std::cout << "Num: " << first << second << third << last << "\n";
            drawChar(turn);
            drawNumber(3, first < 1 ? 10 : first);
            drawNumber(2, second < 1 ? 10 : second);
            drawNumber(1, third < 1 ? 10 : third);
            drawNumber(0, last);
        }
};