/**
 * \file sejf.c
 */
/**
 * Biblioteki uzyte w aplikacji: 
 * Servo - Biblioteka do obslugi Serwa.
 * Keypad - Biblioteka do obslugi klawiatury.
 * LiquidCrystal_PCF8574 - Biblioteka I2C dla wyswietlacza LCD.
 * SPI - Biblioteka do obslugi czytnika RFID.
 * MFRC522 - Biblioteka do obslugi czytnika RFID.
 */
#include <Wire.h>
#include <Servo.h>	
#include <Keypad.h>
#include <LiquidCrystal_PCF8574.h>  
#include <SPI.h> 
#include <MFRC522.h> 

#define biblioteki
#define SS_PIN 10
#define RST_PIN 9
Biblioteki;
MFRC522 rfid(SS_PIN, RST_PIN); //! Ustawienia czytnika RFID.

const byte poprawna_karta[] = {0xE9, 0xD3, 0x9B, 0x84};


LiquidCrystal_PCF8574 lcd(0x27);  //!< Ustawienie adresu ukladu wyswietlacza LCD na 0x27


Servo servo;
int Servopin = 3; //!< Ustawienie pinu dla serwa.

int dioda = 2; //!< Ustawienie pinu dla diody LED.

 //-------------------------------------------------------------------------WAROSC BLUETOOTH
char wartosc; //! Zmienna do ktorej przypisany jest PIN.

char* password ="7890"; //! Ustwienie hasla.
int pozisyon = 0; 
int przycisk = 0;

//-------------------------------------------------------------------------KLAWIATURA
const byte rows = 4; 
const byte cols = 4;

char keyMap [rows] [cols] = { {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

byte rowPins [rows] = {8, 7, 6, 5}; 
byte colPins [cols] = {4, A3, A2, A1};

Keypad myKeypad = Keypad(makeKeymap(keyMap), rowPins, colPins, rows, cols);



//-------------------------------------------------------------------------CZUJNIK ZBLIZENIOWY/ODBCIOWY
#define sensor1 A0



//-------------------------------------------------------------------------POPRAWNOSC WSZYTSKICH OPCJI
int haslo=30;
int haslo_karta = 0;
int haslo_pin = 0;
int haslo_czujnik = 0;
int haslo_suma;


//-------------------------------------------------------------------------KOD BLOKOWANIE SEJFU
void setLocked(int locked){ //! Funkcja odpowiedzialna za zamykanie i otwieranie sejfu (ruch sewromechanizmu).
 if(locked){    //locked==true - sejf zamkniety
    servo.write(110);
   }
    else{     //locked==false - sejf otwarty
      przycisk = 0;
      servo.write(20);
    }
}


void zle_klaw(){
  przycisk=0;
    pozisyon=0;
    setLocked (true);
    digitalWrite(dioda, HIGH);
    
    delay(1000);  
    digitalWrite(dioda, LOW);
}




void setup() {//! Funkcja wykonywana jako pierwsza, zaraz po podlaczeniu urzadzenia do pradu. Ustawia ona serwo w pozycje zamknietego zamka, wlacza podswietlenie ekranu i wyswietla napis "Witaj".
  delay(500);
  servo.attach(Servopin);
  servo.write(110); //! servo.write(110) - Serwo ustawine na 90 stopni.
  pinMode(dioda, OUTPUT); //! pinMode(dioda, OUTPUT) - Ustawienie pinu dioda jako wyjscie.
  
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init(); 
  
  lcd.begin(16,2); 
  lcd.setBacklight(50);  //! lcd.setBacklight(50) - Podswietlanie ekranu.
  //lcd.display(); 
  lcd.setCursor(6,0);  
  lcd.print("Witaj");   //! lcd.print("Witaj") - Dowolny napis nie dluzszy niz 16 znakow.
  delay(2500);          //! delay(2500) - Opoznienie.
  lcd.clear();          //! lcd.clear() - Czyszczenie ekranu
  lcd.setBacklight(0);  //! lcd.setBacklight(0) - Wylaczenie podswietlania.


pinMode(sensor1, INPUT); 

 

}


void loop() {//! Glowna petla programu. Tu wywolywane sa petle odpowiedzialne za poszczegolne zabezpieczenia (karta zblizeniowa, kod, czujnik odbiciowy).
  poczatek:
    setLocked (true); //sejf jest zablokowany


if(Serial.available() > 0)
{
  wartosc=Serial.read();
  if(wartosc=='A'){
    setLocked (false);
    goto otwarte;
  }
  }




while(haslo_karta==0){ //! Sprawdza czy zostala przylozona odpowiendnia karta, a nastepnie czy zostal podany odpowieni kod.
        lcd.setBacklight(50);  
      lcd.setCursor(0,0);  
      lcd.print("   Zbliz karte");  


      

  if ( ! rfid.PICC_IsNewCardPresent()) 
    return;


  if ( ! rfid.PICC_ReadCardSerial())
    return;

 
  if (rfid.uid.uidByte[0] == poprawna_karta[0] || 
      rfid.uid.uidByte[1] == poprawna_karta[1] || 
      rfid.uid.uidByte[2] == poprawna_karta[2] || 
      rfid.uid.uidByte[3] == poprawna_karta[3] ) 
     {
      
    digitalWrite(dioda, HIGH);
    lcd.setBacklight(50);  
    lcd.setCursor(1,0);  
    lcd.print("Poprawna karta");
    haslo_karta=10;
    delay(5000);
    lcd.clear();
    lcd.setBacklight(0);
    digitalWrite(dioda, LOW);
    
    
  }
  else 
  {
    digitalWrite(dioda, HIGH);
    lcd.setBacklight(50);  
    lcd.setCursor(3,0);  
    lcd.print("Zla karta   ");
    delay(5000);
    lcd.clear();
    lcd.setBacklight(0);
    digitalWrite(dioda, LOW);
    goto poczatek;
    
  }
  

  rfid.PICC_HaltA();


  rfid.PCD_StopCrypto1();

 
     
   }


//-------------------------------------------------------------------------PODAJ PIN
lcd.clear();
    

        
  lcd.setBacklight(50);  
      lcd.setCursor(3,0);  
      lcd.print("Podaj kod");  
     // lcd.clear();
     // lcd.setBacklight(0);
  
 przycisk = 0;
 char whichKey = myKeypad.getKey();

  if(whichKey == '*'){
    zle_klaw();
  }
   if(whichKey == '#'){
    zle_klaw();
  }
   if(whichKey == 'A'){
    zle_klaw();
  }
   if(whichKey == 'B'){
    zle_klaw();
  }
   if(whichKey == 'C'){
    zle_klaw();
  }
   if(whichKey == 'D'){
    zle_klaw();
  }


  if(whichKey == '0'){
    przycisk=przycisk+1;
    }
    if(whichKey == '1'){
    przycisk=przycisk+1;
    }
    if(whichKey == '2'){
    przycisk=przycisk+1;
    }
    if(whichKey == '3'){
   przycisk=przycisk+1;
    }
    if(whichKey == '4'){
     przycisk=przycisk+1;
    }
    if(whichKey == '5'){
     przycisk=przycisk+1;
    }
    if(whichKey == '6'){
     przycisk=przycisk+1;
    }
    if(whichKey == '7'){
     przycisk=przycisk+1;
    }
    if(whichKey == '8'){
     przycisk=przycisk+1;
    }
    if(whichKey == '9'){
     przycisk=przycisk+1;
    }
if(whichKey == password [pozisyon]){

    pozisyon++;
  }
    
    if(przycisk == 5){
      digitalWrite(dioda, HIGH);
      lcd.setBacklight(50);  
      lcd.setCursor(4,0);  
      lcd.print("Zly kod"); 
      haslo_pin=0; 
      delay(5000);
      lcd.clear();
      lcd.setBacklight(0);
      digitalWrite(dioda, LOW);
      przycisk = 0;
      goto poczatek;
    }
    if(przycisk == 4){
      if(pozisyon < 3){
      digitalWrite(dioda, HIGH);
      lcd.setBacklight(50);  
      lcd.setCursor(4,0);  
      lcd.print("Zly kod");
      haslo_pin=0;  
      delay(5000);
      lcd.clear();
      lcd.setBacklight(0); 
      digitalWrite(dioda, LOW);
      przycisk = 0;
      goto poczatek;
    }
    }
  
  if(pozisyon == 4){
    przycisk = 0;
    pozisyon = 0; 
    digitalWrite(dioda, HIGH);
    lcd.setBacklight(50);  
    lcd.setCursor(2,0);  
    lcd.print("Poprawny kod");  //dowolny napis nie dluzszy niz 16 
    haslo_pin=10;
    delay(5000);
    lcd.clear();
    lcd.setBacklight(0);
    digitalWrite(dioda, LOW);
  }
   
 }


//-------------------------------------------------------------------------CZUJNIK
int proby = 0;

while(haslo_czujnik==0){ //! Petla ktora sprawdza poprawnosc wykonenej sekwencji przysloniecia czujnika odbiciowego.
int odczyt = analogRead(sensor1);
lcd.setBacklight(50);  
    lcd.setCursor(4,0);  
    lcd.print("Wykonaj");  //dowolny napis nie dluzszy niz 16 
    lcd.setCursor(3,1);
    lcd.print("sekwencje");
    delay(5000);
    lcd.clear();
    lcd.setBacklight(0);




if(odczyt >= 500)           //jezeli odsloniete
  {
  //zapal diode
  digitalWrite(dioda, HIGH);
    
  }
  else                       //jezeli zasloniete
  {
  //zgas diode
  digitalWrite(dioda, LOW);
  lcd.setBacklight(50);  
    lcd.setCursor(7,0);  
    lcd.print("OK"); 
    proby++;
    delay(5000);
    lcd.clear();
    lcd.setBacklight(0);
  
  
}

if (proby==3){
  haslo_czujnik=10;
}
}

//-------------------------------------------------------------------------OTWIERANIE
haslo_suma=haslo_karta+haslo_pin+haslo_czujnik;

otwarte:

if(haslo_suma==haslo){   //! Je¿eli wszystkie zabezpieczenia zosta³y prawid³owo odblokowane - otwiera sejf.
  digitalWrite(dioda, HIGH);
    lcd.setBacklight(50);  
    lcd.setCursor(0,0);  
    lcd.print("Sejf sie otworzy");  //dowolny napis nie dluzszy niz 16 
    delay(5000);
    setLocked (false);    //sejf otwarty
    //delay(5000);
    lcd.clear();
   lcd.setBacklight(0);
    digitalWrite(dioda, LOW);
    haslo_karta=0;
    haslo_pin=0;
    haslo_czujnik=0;
    }

//-------------------------------------------------------------------------ZAMYKANIE
int c=0;

 while (c==0){ //! Petla oczekujaca na przycsk "A". Po wcisnieciu "A" zamyka sejf.
   char whichKey2 = myKeypad.getKey();
  lcd.setBacklight(50);  
    lcd.setCursor(2,0);  
    lcd.print("A - Zamknij");
    if(whichKey2 == 'A'){
      c=10;
      }
      if(Serial.available() > 0)
{
  wartosc=Serial.read();
  if(wartosc=='B'){c=10;}
  }
    } 
}
