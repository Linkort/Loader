#include "main.h"

Button btnUp(BTN1);
Button btnDown(BTN2);
Button btnRead(BTN3);
Button btnWrite(BTN4);
VirtButton btnChMode;

uint8_t screenVal;  //Адрес платы на экране. Значение 1-255;

uint8_t mode; // Тек. режим работы. 

FtpServer ftpSrv;

void setup() {
  Serial.begin(BAUDS);
  /*
  Режим точки доступа.
  Вход - При подаче питания удерживать кнопку READ или WRITE
  Выход - отключить питание
  Назначение - hабота с файлами по FTP, работа с Loader'ом по Webс
  */
  if (btnRead.read() or btnWrite.read()) {
    display("alloff", "A", "P"); 
    WiFi.mode(WIFI_AP); 
    WiFi.softAP(AP_SSID, AP_PASS);//Включение точки доступа
    if (SPIFFS.begin()) ftpSrv.begin(AP_SSID, AP_PASS);//Настройки FTP сервера
    mode = 11;
    return;
  }
  //Настройки
  display("allon", "allon", "allon");
  mdbSetup();
  delay(500);
  display("-", "-", "-");
}

void loop() {
  // Обработка запросов в режиме точки доступа
  if (mode == 11) { 
    ftpSrv.handleFTP(); // FTP запрос
    return;
  }
  //Обработка кнопок
  btnUp.tick();
  btnDown.tick();
  btnRead.tick();
  btnWrite.tick();
  btnChMode.tick(btnUp, btnDown); //Виртуальная кнопка для смены режима UP+DOWN

  //Cмена режима при удержании кнопок UP и DOWN. Исполнение 1 раз при смене.
  if (btnChMode.hold()){
    mode = (mode+1) % 2;
    if (mode == 0) display("A", "d", "r"); //Режим смены адреса платы
    if (mode == 0) display("F", "L", "S"); //Режим прошивки плат
  }

  switch (mode)
  {
    case 0: //Режим смены адреса платы
      ChangeAddrMode();
      break; 
    case 1: //Режим прошивки плат
      break;
  }
  
}



void ChangeAddrMode() {
  uint8_t mdbErrCode; //Результат чтения/записи по Modbus

  //Up Button press
  if (btnUp.click() or btnUp.step()) {
    screenVal++;
    if (screenVal == 0) screenVal = 1;  //Защита от 0 значения
    display(screenVal);
  }

  //Down Button press
  if (btnDown.click() or btnDown.step()) {
    screenVal--;
    if (screenVal == 0) screenVal = 255;  //Защита от 0 значения
    display(screenVal);
  }

  //Работа с Modbus устройством
  mdbErrCode = mdbPoll(btnRead.click(), btnWrite.click(), screenVal); 
  if (mdbErrCode == 255) return; //Не было команды  
  if (mdbErrCode == 108) { //Успешное чтение
    display(screenVal); // Вывод адреса платы
    return;
  }
  if (mdbErrCode == 109) { //Успешная запись
    display("S","u","c"); // Вывод сообщения
    return;
  }
  display("E", "r", std::to_string(mdbErrCode % 10)); // Вывод кода ошибки
}
