#include "main.h"

Button btnUp(BTN1);
Button btnDown(BTN2);
Button btnRead(BTN3);
Button btnWrite(BTN4);

uint8_t screenVal = 1; //Адрес платы на экране
uint8_t mdbErrCode; //Код 

void setup() {
  mdbSetup();
  display("allon", "allon", "allon");
  delay(500);
  display("-", "-", "-");
}

void loop() {
  //Обработка кнопок
  btnUp.tick();
  btnDown.tick();
  btnRead.tick();
  btnWrite.tick();
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
