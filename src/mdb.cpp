#include "mdb.h"

ModbusMaster node;
SoftwareSerial S(RX, TX);

//Стартовые настройки Modbus
void mdbSetup() {
    Serial.begin(BAUDS);
    S.begin(115200);
   // Serial.swap();
    node.begin(SLAVE_ADDR, S);
  //  node.begin(SLAVE_ADDR, Serial);
}


//Опрос платы
uint8_t mdbPoll(bool cmdRead, bool cmdWrite, uint8_t& Addr){
    static uint8_t state;
    uint8_t result;
    if (cmdRead) state = 1;
    if (cmdWrite) state = 2;
    switch (state) {
        case 0:  return 255; //Нет команды для опроса
        case 1: //Чтение
            result = node.readHoldingRegisters(REG_OFFSET, 1);
            state = 0; //Сброс для выполнения только 1 запроса.
            //Успешно
            if (result == node.ku8MBSuccess) {
                Addr = node.getResponseBuffer(0); // Прочитанный адрес устройства.
                return 108; //Успешный запрос
            }

            break;
        case 2: //Запись
            node.setTransmitBuffer(0,Addr); //Новый адрес платы
            node.setTransmitBuffer(1,BAUDS_DIV); //Новый делитель скорости платы
            result = node.writeMultipleRegisters(REG_OFFSET, 2);
            state = 0; //Сброс для выполнения только 1 запроса.
            if (result == node.ku8MBSuccess) {
                return 109; //Успешный запрос
            }
    } 
    //Ошибка по таймауту. Нет ответа от устройства
    if (result == node.ku8MBResponseTimedOut) return 0;     //Нет ответа от устройства
    //Код ошибки полученное от устройства. Согласно протоколу Modbus. 
    if (result == node.ku8MBIllegalFunction) return 1;      //Принятый код функции не может быть обработан.
    if (result == node.ku8MBIllegalDataAddress) return 2;   //Адрес данных, указанный в запросе, недоступен.
    if (result == node.ku8MBIllegalDataValue) return 3;     //Значение, содержащееся в поле данных запроса, является недопустимой величиной.
    if (result == node.ku8MBSlaveDeviceFailure) return 4;   //Невосстанавливаемая ошибка имела место, пока ведомое устройство пыталось выполнить затребованное действие.
    //Код ошибки при обработке пакета.
    if (result == node.ku8MBInvalidSlaveID) return 5;       //Адрес устройства в ответе не соответствует ожидаемому.
    if (result == node.ku8MBInvalidFunction) return 6;      //Код функции в ответе не соответствует коду запроса.
    if (result == node.ku8MBInvalidCRC) return 7;           //Ошибка CRC в ответе.
    return 255;
}