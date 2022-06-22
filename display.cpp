#include <wiringPi.h>
#include <iostream> 
#include <wiringPi.h>
#include <unistd.h>
#include <bitset>

#define HIGH 1
#define LOW 0

#define DIN 27
#define LOAD 28
#define CLK 29
int number[11][4] = {{0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},
                     {0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},{1,0,0,1}, {1,1,1,1}};
int digit[8][4] =  {{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},
                    {0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},};
void codeB(){
    int bits[] = {1,0,0,1};
    digitalWrite(LOAD, LOW);
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
    }
	for(int i = 0; i < 4; i++){
		digitalWrite(DIN, bits[i]);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 1);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
    }
    digitalWrite(LOAD, HIGH);
}
void drawNumber(int index, int num){
    digitalWrite(LOAD, LOW);
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, digit[index][i]);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, number[num][i]);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
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
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
    }
	for(int i = 0; i < 4; i++){
		digitalWrite(DIN, bits[i]);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}
    for(int i = 0; i < 8; i++){
        digitalWrite(DIN, 1);
        digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
    }
    digitalWrite(LOAD, HIGH);
}
void drawChar(int turn){
    digitalWrite(LOAD, LOW);
    int C[12] = {1,0,0,0, 0,1,0,0, 1,1,1,0};
    int H[12] = {1,0,0,0, 0,0,1,1, 0,1,1,1};
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 12; i++){
        if(turn == 1) digitalWrite(DIN,H[i]);
        if(turn == 2) digitalWrite(DIN,C[i]);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LOAD, HIGH);
}
void displayNumber(int num, int turn){
    drawChar(turn);
    digitalWrite(LOAD, LOW);
    int first = (num / 100);
    int second = (num / 100) % 10;
    int third = (num / 10) % 10;
    int last = num % 10;
    //std::cout << "Num: " << first << second << third << last << "\n"; 
    drawNumber(3, first < 1 ? 10 : first);
    drawNumber(2, second < 1 ? 10 : second);
    drawNumber(1, third < 1 ? 10 : third);
    drawNumber(0, last);
    digitalWrite(LOAD, HIGH);
}
void testModeOff(){
    digitalWrite(LOAD, LOW);
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 4; i++){
        digitalWrite(DIN, 1);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    for(int i = 0; i < 8; i++){
        digitalWrite(DIN, 0);
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LOAD, HIGH);
}


void initDisplay(){
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