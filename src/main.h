#pragma once
#include <Arduino.h>

#include <cstdint>
#include <map>
#include <string>

#include <SoftwareSerial.h>

#define EB_STEP_TIME 100 //скорость прироста при удержании up/down 
#include "EncButton.h"
#include "GyverShift.h"

#include "mdb.h"
#include "httpwork.h"
#include "ftpwork.h"

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



//Вывод на дисплей посимвольно
void display(String s1, String s2, String  s3);

//Вывод на дисплей значения до 999
void display(uint16_t val);

//Вывод на дисплей текста
void display(String text);


//режим точки доступа
void AccessPointMode();

//Режим прошивки платы расширения
void FlashMode();

//Режим смены Modbus адреса платы
void ChangeAddrMode();