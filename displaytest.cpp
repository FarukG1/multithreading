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

int C[12] = {1,0,0,0, 0,1,0,0, 1,1,1,0};
int H[12] = {1,0,0,0, 0,1,0,0, 1,1,1,0};
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
void writeNumber(int index, int num){
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
        writeNumber(i,10);
    }
    for(int i = 4; i < 8; i++){
        writeNumber(i,0);
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

void show(int num){
    int first = (num / 100);
    int second = (num / 100) % 10;
    int third = (num / 10) % 10;
    int last = num % 10;
    std::cout << "Num: " << first << second << third << last << "\n"; 
    writeNumber(3, first <= 0 ? 10 : first);
    writeNumber(2, second <= 0 ? 10 : second);
    writeNumber(1, third <= 0 ? 10 : third);
    writeNumber(0, last);
}
int main()
{
	wiringPiSetup();
	std::cout << "Display wird eingerichtet" << "\n";
	pinMode(DIN, OUTPUT);
	pinMode(LOAD, OUTPUT);
	pinMode(CLK, OUTPUT);
    sleep(1);
    powerOn();
    codeB();
    sleep(1);
    resetDisplay();
	std::cout << "Display erfolgreich eingerichtet" << "\n";
    sleep(1);
    std::cout << "Start!" << "\n";
    for(int i = 0; i < 100; i++){
        show(i);
        sleep(1);
    }
	return 0;
}

