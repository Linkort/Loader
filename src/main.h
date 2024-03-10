#pragma once
#include <Arduino.h>
#include <cstdint>
#include <map>
#include <string>
 #include <SoftwareSerial.h>

#include "EncButton.h"
#include "GyverShift.h"

#include "mdb.h"

//Кнопки
#define BTN1 D1  //5    U
#define BTN2 D2  //4    D
#define BTN3 D3  //0    R
#define BTN4 D4  //2    W

//Дисплей
#define DAT D6  //DATA  / DS   D0
#define CLK D7  //CLOCK / SHCP D5
#define CS  D8  //Latch / STCP D6



using std::string;

//Вывод на дисплей посимвольно
void display(string s1, string s2, string  s3);

//Вывод на дисплей значения до 999
void display(uint16_t val);
