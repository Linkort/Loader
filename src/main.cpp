#include "main.h"

Button btnUp(BTN1);
Button btnDown(BTN2);
Button btnRead(BTN3);
Button btnWrite(BTN4);

uint8_t ScreenVal;

void setup() {
  // put your setup code here, to run once:
  displayChars("allon", "allon", "allon");
  delay(500);
  displayChars("-", "-", "-");
  Serial.begin(115200);
}

void loop() {
  //обработка кнопок
  btnUp.tick();
  btnDown.tick();
  btnRead.tick();
  btnWrite.tick();

  //upButton
  if (btnUp.click() or btnUp.step()) {
    ScreenVal++;
    Serial.print(ScreenVal);
    displayVal(ScreenVal);
  }
  //downButton
  if (btnDown.click() or btnDown.step()) {
    ScreenVal--;
    Serial.print(ScreenVal);
    displayVal(ScreenVal);
  }

}
