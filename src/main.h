#pragma once
#include <Arduino.h>
#include <cstdint>
#include <map>
#include <string>

#include "EncButton.h"
#include "GyverShift.h"


//Кнопки
#define BTN1 D1  //5    U
#define BTN2 D2  //4    D
#define BTN3 D3  //0    R
#define BTN4 D4  //2    W

//Дисплей
#define DAT D0  //DATA  / DS
#define CLK D5  //CLOCK / SHCP
#define CS  D6  //Latch / STCP

using std::string;

//Вывод на дисплей посимвольно
void displayChars(string s1, string s2, string  s3);

//Вывод на дисплей значения до 999
void displayVal(uint16_t val);
