#include "main.h"

Button btnUp(BTN1);
Button btnDown(BTN2);
Button btnRead(BTN3);
Button btnWrite(BTN4);
VirtButton btnChMode;

uint8_t screenVal, mode; //Адрес платы на экране

FtpServer ftpSrv;

void setup() {
  mdbSetup();
  display("allon", "allon", "allon");
  Serial.begin(BAUDS);

  //Настройки точки доступа
  //WiFi.softAP(AP_SSID, AP_PASS);

  //Настройки FTP сервера
  SPIFFS.begin();
  ftpSrv.begin(AP_SSID, AP_PASS); 

  delay(500);
  display("-", "-", "-");
}

void loop() {
  //Обработка кнопок
  btnUp.tick();
  btnDown.tick();
  btnRead.tick();
  btnWrite.tick();
  btnChMode.tick(btnUp, btnDown); //Виртуальная кнопка для смены режима UP+DOWN

  //Cмена режима при удержании кнопок UP и DOWN. Исполнение 1 раз.
  if (btnChMode.hold()){
    mode = (mode+1) % 3;
    switch (mode) 
    {
      case 0: //Режим смены адреса платы
        display("A", "d", "r");
        APModeOnOff(false);
        break;
      case 1: //Режим прошивки плат
        display("F", "L", "S"); 
        break;
      case 2: //Режим точки доступа
        display("alloff", "A", "P"); 
        APModeOnOff(true);
       break;
    }
  }

  switch (mode)
  {
    case 0: //Режим смены адреса платы
      ChangeAddrMode();
      break; 
    case 1: //Режим прошивки плат
      break;
    case 2: //Режим точки доступа
      ftpSrv.handleFTP(); //Ftp клиент
      break;
  }
  
}


//Режим смены Modbus адреса платы
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


//Управление точкой доступа
void APModeOnOff(bool ApEnable){
  if (ApEnable) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);//Включение точки доступа
  } else{
    WiFi.softAPdisconnect(true); //Отключение точки доступа
  }
}