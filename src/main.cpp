#include "main.h"

Button btnUp(BTN1);
Button btnDown(BTN2);
Button btnRead(BTN3);
Button btnWrite(BTN4);
VirtButton btnChMode;

uint8_t screenVal; //Адрес платы на экране. Значение 1-255;
uint8_t mode; // Тек. режим работы.
uint8_t firmVal; //Для отображения названий прошивок. Индекс прошивки в векторе.
std::vector <String> List; //Список прошивок

SoftwareSerial Ch(RX, TX); //порт подключения плат расширения

void setup() {
  Serial.begin(BAUDS);
  Serial.printf("\n");
  ftpSetup(); 
  if (btnRead.read() or btnWrite.read()) {
    //Переход режим точки доступа
    display(" ", "A", "P");
    Serial.println("Режим точки доступа");
    httpSetup();
    mode = 11;
    return;
  }

  //Настройки
  display("allon", "allon", "allon");
  Ch.begin(BAUDS, SWSERIAL_8N1);
  mdbSetup(Ch);
  delay(500);
  display("-", "-", "-");
}

void loop() {
  if (mode == 11) {// Режим точки доступа
    AccessPointMode();
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
    if (mode == 0) {//Режим смены адреса платы
      display("A", "d", "r");
      mdbSetup(Ch);
      Serial.println("Режим смены адреса платы");
    }
    
    if (mode == 1) {//Режим прошивки плат
      display("F", "L", "S");
      Serial.println("Режим прошивки плат");
      flashSetup(Ch);
      List = firmwareListUpdate();
    }
  }

  switch (mode)
  {
    case 0: //Режим смены адреса платы
      ChangeAddrMode();
      break; 
    case 1: //Режим прошивки плат
      FlashMode();
      break;
  }
  
}


void AccessPointMode(){
  /*
  Режим точки доступа.
  Вход - При подаче питания удерживать кнопку READ или WRITE.
  Выход - отключить питание.
  Назначение - работа с файлами по FTP, работа с Loader'ом по Web странице.
  */
  httpClient();
  ftpClient();
}

void FlashMode(){
  /*
  //Режим прошивки платы расширения.
  Вход - Удерживать 2 сек кнопки UP и DOWN.
  Выход - Удерживать 2 сек кнопки UP и DOWN.
  Назначение - Прошивка микроконтроллера STM32, который установлен на платах расширения.
  */

  uint8_t flashErrCode; //Результат прошивки

  //Up Button press
  if (btnUp.click() or btnUp.step()) {
    firmVal++;
    if (firmVal >= List.size()) firmVal = 0; // Защита от выхода за пределы 0-кол-во прошивок
    display(List[firmVal]);
  }

  //Down Button press
  if (btnDown.click() or btnDown.step()) {
    firmVal--;
    if (firmVal >= List.size()) firmVal = (List.size()-1); // Защита от выхода за пределы 0-кол-во прошивок
    display(List[firmVal]);
  }
  //Write Button press
  if (btnWrite.click()){
    flashErrCode = flash(List[firmVal]);
  }
}


void ChangeAddrMode() {
  /*
  //Режим смены Modbus адреса платы
  Вход - Удерживать 2 сек кнопки UP и DOWN.
  Выход - Удерживать 2 сек кнопки UP и DOWN.
  Назначение - , который установлен на платах расширения.
  */

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
  Serial.printf("Резульат запроса: %u", mdbErrCode);  
  if (mdbErrCode == 108) { //Успешное чтение
    display(screenVal); // Вывод адреса платы
    return;
  }
  if (mdbErrCode == 109) { //Успешная запись
    display("S","u","c"); // Вывод сообщения
    return;
  }
  display("E", "r", String(mdbErrCode % 10)); // Вывод кода ошибки
}

