Назначение файлов FTP сервера:
- index.html - Web страница;
- README.md - Файл инструкции и правил;
- *.bin - файл прошивки для плат расширения.

Файлы находящиеся в папке "/data" тек. репозитория переместить в Loader при 1 включении.

Правила прошивок:
- Наименование файла прошивки(учитывая расширение) должно состоять из: 
    - минимум из 3 символа (т.к. дисплей Loader - 3х-знаковый);
    - максимум из 31 символа (ограничение ESP8266);
- Наименование файла должно состоять из след. символов:
    - цифры 0123456789;
    - Буквы ACEFHJLPSUY;
    - Буквы bcdehnotur;
    - Символы "-"(тире), " "(пробел), "."(точка), "8."(вкл. все сегменты); 

Рекомендуется 3 символа.