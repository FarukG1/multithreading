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

        // Bit pusht nullen
        void writeZero(int count){
            for(int i = 0; i < count; i++){
                digitalWrite(DIN, 0);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
        }

        // Bit pusht einsen
        void writeOne(int count){
            for(int i = 0; i < count; i++){
                digitalWrite(DIN, 1);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
        }

        // Diese Funktion Schaltet den Test Modus aus
        void testModeOff(){
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
            writeOne(4);
            writeZero(8);
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

        // Diese Funktion ändert das Scanlimit auf 8 (alle Felder/Stellen)
        void scanLimit(){
            int address[4] = {1,0,1,1};
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, address[i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            writeZero(5);
            writeOne(3);
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

        // Diese Funktion wechselt für die ersten 4 Stellen
        // den Decode-Mode zu Code B
        void codeB(){
            int address[] = {1,0,0,1};
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
	        for(int i = 0; i < 4; i++){
		        digitalWrite(DIN, address[i]);
                digitalWrite(CLK, HIGH);
		        digitalWrite(CLK, LOW);
	        }
            writeZero(4);
            writeOne(4);
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

        // Diese Funktion sendet die übergegebene Ziffer 
        // in die übergegebene Stelle
        void drawNumber(int index, int num){
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, digit[index][i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            writeZero(4);
            for(int i = 0; i < 4; i++){
                digitalWrite(DIN, number[num][i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

        // Diese Funktion schaltet das Display an
        void powerOn(){
            int address[] = {1,1,0,0};
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
	        for(int i = 0; i < 4; i++){
		        digitalWrite(DIN, address[i]);
                digitalWrite(CLK, HIGH);
		        digitalWrite(CLK, LOW);
	        }
            writeOne(8);
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

        // Diese Funktion schaltet alle Segmente aus
        void resetDisplay(){
            for(int i = 0; i < 4; i++){
                drawNumber(i,10);
            }
            for(int i = 4; i < 8; i++){
                drawNumber(i,0);
            }
        }

        // Diese Funktion zeigt C oder ein H
        // an der vordersten Stelle an
        void drawChar(int turn){
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
            for(int i = 0; i < 12; i++){
                if(turn == 1) digitalWrite(DIN,H[i]);
                if(turn == 2) digitalWrite(DIN,C[i]);
                digitalWrite(CLK, HIGH);
                digitalWrite(CLK, LOW);
            }
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

        // Diese Funktion setzt die Intensität ein
        void setIntensity(){
            int address[] = {1,0,1,0};
            // intensity data
            int data[] = {0,0,0,0};
            digitalWrite(LOAD, LOW);
            // START DATA STREAM
            writeZero(4);
	        for(int i = 0; i < 4; i++){
		        digitalWrite(DIN, address[i]);
                digitalWrite(CLK, HIGH);
		        digitalWrite(CLK, LOW);
	        }
            writeZero(4);
            for(int i = 0; i < 4; i++){
		        digitalWrite(DIN, data[i]);
                digitalWrite(CLK, HIGH);
		        digitalWrite(CLK, LOW);
	        }
            // END DATA STREAM
            digitalWrite(LOAD, HIGH);
        }

    public:
        // Diese Funktion initialisiert das Display
        void init(){
            wiringPiSetup();
            pinMode(DIN, OUTPUT);
            pinMode(LOAD, OUTPUT);
 	        pinMode(CLK, OUTPUT);
            powerOn();
            sleep(1);
            codeB();
            sleep(1);
            testModeOff();
            sleep(1);
            scanLimit();
            sleep(1);
            setIntensity();
            sleep(1);
            resetDisplay();
	        std::cout << "Display erfolgreich eingerichtet" << "\n";
        }

        // Diese Funktion teil die übergebene Zahl
        // auf in einzelne Ziffern und gibt die
        // dan aufs Display aus
        void show(int num, int turn){
            int first = (num / 1000);
            int second = (num / 100) % 10;
            int third = (num / 10) % 10;
            int last = num % 10;
            drawChar(turn);
            drawNumber(3, first < 1 ? 10 : first);
            drawNumber(2, second < 1 ? 10 : second);
            drawNumber(1, third < 1 ? 10 : third);
            drawNumber(0, last);
        }
};