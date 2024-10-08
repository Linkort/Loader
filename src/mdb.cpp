#include "mdb.h"

ModbusMaster node;

void mdbSetup(SoftwareSerial &port) {
    /* 
    Настройки для работы по ModbusRTU
    */
    //Переоткрыть порт с новыми параметрами
    port.end(); 
    port.begin(BAUDS, SWSERIAL_8N1);
    node.begin(SLAVE_ADDR, port);
}

uint8_t mdbPoll(bool cmdRead, bool cmdWrite, uint8_t& Addr){
    static uint8_t state;
    uint8_t result;
    if (cmdRead) state = 1;
    if (cmdWrite) state = 2;
    switch (state) {
        case 0:  return 255; //Нет команды для опроса
        case 1: //Чтение
            state = 0; //Сброс для выполнения только 1 запроса.
            result = node.readHoldingRegisters(REG_OFFSET, 1);
            //Успешно
            if (result == node.ku8MBSuccess) {
                Addr = node.getResponseBuffer(0); // Прочитанный адрес устройства.
                return 108; //Успешный запрос
            }

            break;
        case 2: //Запись
            state = 0; //Сброс для выполнения только 1 запроса.
            result = node.writeSingleRegister(REG_OFFSET, Addr); //Новый адрес платы
            if (!result == node.ku8MBSuccess) break; //Выход при ошибке
            result = node.writeSingleRegister(REG_OFFSET+1, BAUDS_DIV); //Новый делитель скорости платы
            if (result == node.ku8MBSuccess) {
                return 109; //Успешный запрос
            }
            break;
    } 
    //Ошибка по таймауту. Нет ответа от устройства
    if (result == node.ku8MBResponseTimedOut) return 0;     //Таймаут. Нет ответа от устройства
    //Код ошибки полученное от устройства. Согласно протоколу Modbus. 
    if (result == node.ku8MBIllegalFunction) return 1;      //Принятый код функции не может быть обработан.
    if (result == node.ku8MBIllegalDataAddress) return 2;   //Регистр данных, указанный в запросе, недоступен.
    if (result == node.ku8MBIllegalDataValue) return 3;     //Значение, содержащееся в поле данных запроса, является недопустимой величиной.
    if (result == node.ku8MBSlaveDeviceFailure) return 4;   //Невосстанавливаемая ошибка, slave не смог выполнить затребованное действие.
    //Код ошибки при обработке пакета.
    if (result == node.ku8MBInvalidSlaveID) return 5;       //Адрес устройства в ответе не соответствует ожидаемому.
    if (result == node.ku8MBInvalidFunction) return 6;      //Код функции в ответе не соответствует коду запроса.
    if (result == node.ku8MBInvalidCRC) return 7;           //Ошибка CRC в ответе.
    return 255;
}