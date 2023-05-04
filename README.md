# 2023_1_RelayTeam

Команда:
 * Герасин Антон
 * Мышляев Андрей
 * Османов Ислам
 * Шаповалов Иван

Ментор:
 * Павел Чеклин

Название Проекта:
 * MultiRelay

Зависимости требующиеся к текущему моменту:
 * Qt 5.15.2
 * CMake 3.5 и старше
 * gstreamer 1.20.3 и старше 
```
 https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c
```
 * Sqlite3
```
 sudo apt-get install libsqlite3-dev
```
 * qmdnsengine
```
 https://github.com/Ant0nL0rd/qmdnsengine-qt5-my-fix
```
```
 mkdir build
 cd build
 cmake ..
 sudo make install
```
 * GTest
```
 sudo apt-get install libgtest-dev
```
Компиллируется в Qt Creator при помощи CMake.
