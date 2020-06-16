Tytuł projektu: Zamek elektroniczny z wielopoziomową kontrolą dostępu.
Autor projektu: Mateusz Zych

Jest to projekt wykonany w ramach zajęć "Projekt Inżynierski" 
na III roku studiów stacjonarnych, 
kierunku Informatyka na Uniwersytecie Pedegogicznym w Krakowie.


Konstrukcja układu zamka elektronicznego z trójpoziomową kontrolą dostępu.
Projekt ten został stworzony w oparciu o platformę Arduino i jej kopmonenty. 
System obsługuje klawiaturę, czytnik kart RFID, czujnik odbiociowy 
i wyświetlacz LCD. Mechanizm zamka jest sterowany serwomechanizmem. 
Postęp w wyłączaniu blokad jest sygnalizowany przez diodę LED i komunikaty 
na wyświetlaczu. Dodatkowo, dzięki dedykowanej aplikacji na telefon z systemem Android 
zamek ma możliwość uzbrajania i rozbrajania poprzez sygnał bluetooth.

Plik z programem: sejf.c
Biblioteki wykorzystane w programie:

 Servo.h - https://www.arduinolibraries.info/libraries/servo
 Keypad.h - https://www.arduinolibraries.info/libraries/keypad
 LiquidCrystal_PCF8574.h - https://www.arduinolibraries.info/libraries/liquid-crystal_pcf8574
 SPI.h - https://www.arduinolibraries.info/libraries/mfrc522
 MFRC522.h - https://www.arduinolibraries.info/libraries/mfrc522

Dokumentacja techniczna: http://ux.up.krakow.pl/~matzyc/pinz/sejf_8c.html#adb871bf359770bbab4bd1ab78f7dd2d4
