#include <wiringPi.h>
#include <unistd.h>
#include "../headerfiles/display.h"

#define HIGH 1
#define LOW 0

#define DIN 27
#define LOAD 28
#define CLK 29

// Bit pusht nullen
void DisplayController::writeZero(int count){
    for(int i = 0; i < count; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
}

// Bit pusht einsen
void DisplayController::writeOne(int count){
    for(int i = 0; i < count; i++){
        digitalWrite(DIN, 1);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
}

// Diese Funktion Schaltet den Test Modus aus
void DisplayController::testModeOff(){
    digitalWrite(LOAD, LOW);
    // START DATA STREAM
    writeZero(4);
    writeOne(4);
    writeZero(8);
    // END DATA STREAM
    digitalWrite(LOAD, HIGH);
}

// Diese Funktion ändert das Scanlimit auf 8 (alle Felder/Stellen)
void DisplayController::scanLimit(){
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
void DisplayController::codeB(){
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
void DisplayController::drawNumber(int index, int num){
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
void DisplayController::powerOn(){
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

// Diese Funktion zeigt C oder ein H
// an der vordersten Stelle an
void DisplayController::drawChar(int turn){
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
void DisplayController::setIntensity(){
    int address[] = {1,0,1,0};
    // intensity data
    int data[] = {0,0,0,1};
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

// Diese Funktion schaltet alle Segmente aus
void DisplayController::resetDisplay(){
    for(int i = 0; i < 4; i++){
        drawNumber(i,10);
    }
    for(int i = 4; i < 8; i++){
        drawNumber(i,0);
    }
}

// Diese Funktion initialisiert das Display
void DisplayController::init(){
    wiringPiSetup();
    pinMode(DIN, OUTPUT);
    pinMode(LOAD, OUTPUT);
 	pinMode(CLK, OUTPUT);
    powerOn();
    testModeOff();
    sleep(1);
    scanLimit();
    sleep(1);
    setIntensity();
    sleep(1);
    codeB();
    sleep(1);
    resetDisplay();
}

// Diese Funktion teil die übergebene Zahl
// auf in einzelne Ziffern und gibt die
// dan aufs Display aus
void DisplayController::show(int num, int turn){
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