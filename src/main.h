#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <FS.h>
#include <ESP8266FtpServer.h>


#include <cstdint>
#include <map>
#include <string>

#include <SoftwareSerial.h>

#define EB_STEP_TIME 100 //скорость прироста при удержании up/down 
#include "EncButton.h"
#include "GyverShift.h"

#include "mdb.h"

//Кнопки
#define BTN1 D4  //2    U
#define BTN2 D3  //0    D
#define BTN3 D2  //4    R
#define BTN4 D1  //5    W

//Дисплей
#define DAT D6  //DATA  / DS   D0
#define CLK D7  //CLOCK / SHCP D5
#define CS  D8  //Latch / STCP D6

//WiFi точка доступа

#define AP_SSID "LoaderAP" // Название WiFi точки | UserName для FTP
#define AP_PASS "11111111" // Пароль   WiFi точки | Password для FTP

using std::string;




//Вывод на дисплей посимвольно
void display(string s1, string s2, string  s3);

//Вывод на дисплей значения до 999
void display(uint16_t val);

//Режим смены Modbus адреса платы
void ChangeAddrMode();