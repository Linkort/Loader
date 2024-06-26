# Устройство для смены Modbus адреса плат/модулей расширения ПЛК.
![Loader v17](doc/Loader%20v17.png)
## Описание
![Loader v18](doc/Loader%20v18.png)<br>
На лицевой стороне расположены дисплей и кнопки управления:    
Кнопки UP/ DOWN - изменение адреса на дисплее;<br>
Кнопка READ - Прочитать текущий адрес платы расширения;<br>
Кнопка WRITE - Записать адрес на дисплее в плату расширения;<br>
Кнопка POWER - Сброс питания на плате расширения. Требуется для перезагрузки целевой платы. <br>

Разъемы подключения:<br>
X1 - Разъем питания 9-30V DC;<br>
X2, X3 - Разъемы подключения модуля расширения;<br>
X4 - Разъем подключения платы расширения;<br>
USB type C - Разъем питания / программирования.<br>
<details>
  <summary>Индикация на дисплее</summary>

| Индикация на дисплее | Расшифровка                                                                    |
| -------------------- | ------------------------------------------------------------------------------ |
| "8.8.8."             | Процедура включения устройства                                                 |
| "---"                | Устройство готово к работе                                                     |
| Значение 1 - 255     | Процедура задания пользователем адреса для записи.                             |
| "Suc"                | Адрес устройства успешно записан.                                              |
| "Er0"                | Таймаут, нет ответа от устройства.                                             |
| "Er1"                | Принятый код функции не может быть обработан.                                  |
| "Er2"                | Регистр данных, указанный в запросе, недоступен.                               |
| "Er3"                | Значение, содержащееся в поле данных запроса, является недопустимой величиной. |
| "Er4"                | Невосстанавливаемая ошибка, slave не смог выполнить затребованное действие.    |
| "Er5"                | Адрес устройства в ответе не соответствует ожидаемому.                         |
| "Er6"                | Код функции в ответе не соответствует коду запроса.                            |
| "Er7"                | Ошибка CRC в ответе.                                                           |

Ошибки "Er0" - "Er7" - результат неудачи при попытке записи/чтения тек. адреса платы. 
При успешном чтении на дисплее отобразиться адрес устройства.
</details>


## Инструкция

1\. Подать питание на Loader<br>
Подать питание на X1 - 10-30V DC, либо подключить кабель Type C. <br>
>**При питании через кабель USB type C - допускается работа только с платами расширения.**<br>
>**Одновременное подключение двух источников питания запрещено.**

2\. Настройка платы/модуля<br>
Для платы расширения: Установить SW1.1 - в положение ON / UP. Установить SW1.2 - в положение OFF / DOWN<br>
Для модуля расширения: Установить Джампер J4. Снять Джампер J3.<br>
<details>
  <summary> Назначение переключателей</summary>
  
  |                                         | Переключатель                                  | Переключатель                            |
  | --------------------------------------- | ---------------------------------------------- | ---------------------------------------- |
  | Плата <br>Расширения                    | Джампер J4                                     | Джампер J3                               |
  | Модуль <br>расширения                   | SW1.1                                          | SW1.2                                    |
  | Назначение                              | Запись настроек в <br>энергонезависимую память | Применение  скорости <br>из памяти       |
  | Положение ON / UP<br>Джампер установлен | Разрешено                                      | Разрешено                                |
  | Положение OFF / DOWN<br>Джампер снят    | Запрещено                                      | Запрещено.<br>Скорость для обмена 115200 |
  
</details>

3\. Подключить платы/модуля<br>
Плату расширения в разъем X4. Модуль расширения в разъем X2, X3. <br>
>**Одновременное подключение платы расширения и модуля расширения не допустимо.**<br>
              
4\. Нажать кнопку READ - прочитать адрес платы/модуля<br>
  При успешном чтении - на дисплее отобразится тек. адрес платы/модуля. <br>
  
5\. Изменить адрес на дисплее устройства на требуемый.<br>

6\. Нажать кнопку WRITE - записать адрес в плату расширения<br>
Сообщение "Suc" сообщает об успешном изменении адреса.
## Дополнительно

<details>
  <summary>Карта регистров</summary>

Запись/Чтение происходит согласно карте регистров плат расширения:
| Адрес<br>Modbus | Описание                                  | Кнопка READ               | Кнопка WRITE                         |
| --------------- | ----------------------------------------- | ------------------------- | ------------------------------------ |
| 1               | Modbus-адрес платы                        | Чтение и вывод на дисплей | Запись значения с дисплея            |
| 2               | Делитель скорости коммуникационного порта | -                         | Запись фиксированного делителя - 240 |

Функция 0x03 Read Holding Registers(4x). Настройки подключения: 115200 8N1.
Связь ведется с помощью broadcast адреса - 255, поэтому потери связи при смене адреса не будет.
</details>

