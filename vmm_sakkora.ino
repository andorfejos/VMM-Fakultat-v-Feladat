#include <LiquidCrystal.h>

//masodperc megadasa
#define SEC (60 +(millis() / 1000))

//Gombok megadasa
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnSTART 4
#define btnRESET   5

//Konstansok
const int btnWhite = 8;
const int btnBlack = 9;
const int pinBuzzer = 10;
const int analogPin = A0;

//Valtozok (menu & gombok)
int lcd_key = 0;
int adc_key_in = 0;
int countMenu = 0;
int blinkTime;
int initBlinkTime;
int blinkDelay = 500;
bool exitMenu = true;
bool sidePlayer = true;
bool blinkState = true;

//Valtozok (masodperc szamolas W/B)
int cTemp = 0;
int cSecWhite = 0;
int cSecBlack = 0;

//Noveles (ha lenyomjuk a gombot)
int setAdd;

//Valtozok (W)
int setSecWhite;
int hWhite;
int minWhite;
int secWhite;

//Valtozok (B)
int setSecBlack;
int hBlack;
int minBlack;
int secBlack;

//LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Gombok olvasasa
void debounceKey() {
  while (adc_key_in < 1000) {
    adc_key_in = analogRead(analogPin);
  }
}

int read_LCD_buttons()
{
  adc_key_in = analogRead(analogPin);

  if (adc_key_in > 1000) return btnRESET;

  /*
  A gombok ertekei az aramkorbe helyezett
  ellenallasokbol lettek megadva,
  hogy azt kapjuk amit szeretnenk
  (pl: felfele gomb megnyomasanal ne lepjen jobbra)
  */

  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 850)  return btnSTART;

  return btnRESET;
}

//MENU - ido es noveles beallitasa
void menuSetUp() {

  lcd_key = read_LCD_buttons();

  switch (lcd_key)
  {
    case btnRIGHT:
      {
        countMenu++;
        if (countMenu > 4) countMenu = 0;
        debounceKey();
        break;
      }
    case btnUP:
      {
        if (countMenu == 0) {
          hWhite++;
          if (hWhite > 9) hWhite = 0;
          debounceKey();
        }
        else if (countMenu == 1) {
          minWhite++;
          if (minWhite > 59) minWhite = 0;
          debounceKey();
        }
        else if (countMenu == 2) {
          secWhite++;
          if (secWhite > 59) secWhite = 0;
          debounceKey();
        }
        else if (countMenu == 3) {
          setAdd++;
          if (setAdd > 99) setAdd = 0;
          debounceKey();
        }
        else {
          sidePlayer ^= 1;
          debounceKey();
        }
        break;
      }
    case btnDOWN:
      {
        if (countMenu == 0) {
          hWhite--;
          if (hWhite < 0) hWhite = 9;
          debounceKey();
        }
        else if (countMenu == 1) {
          minWhite--;
          if (minWhite < 0) minWhite = 59;
          debounceKey();
        }
        else if (countMenu == 2) {
          secWhite--;
          if (secWhite < 0) secWhite = 59;
          debounceKey();
        }
        else if (countMenu == 3) {
          setAdd--;
          if (setAdd < 0) setAdd = 99;
          debounceKey();
        }
        else {
          sidePlayer ^= 1;
          debounceKey();
        }
        break;
      }
    case btnSTART:
      {
        exitMenu = 0;
        break;
      }
    case btnRESET:
      {
        break;
      }
  }
}

//LCDre kiiras (feher karakterek)
void printTimerWhite() {

  blinkTime = millis() - blinkDelay;

  if (exitMenu == 1 && countMenu == 0) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(0, 1);
      lcd.print(" ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      lcd.setCursor(0, 1);
      lcd.print(hWhite);
      blinkState = 1;
      initBlinkTime = millis();
    }
  }

  else {
    lcd.setCursor(0, 1);
    lcd.print(hWhite);
  }
  lcd.setCursor(1, 1);
  lcd.print(":");

  if (exitMenu == 1 && countMenu == 1) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(2, 1);
      lcd.print("  ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      if ((minWhite < 10) && (minWhite >= 0)) {
        lcd.setCursor(2, 1);
        lcd.print(0);
        lcd.setCursor(3, 1);
        lcd.print(minWhite);
      }
      else {
        lcd.setCursor(2, 1);
        lcd.print(minWhite);
      }
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  else {
    if ((minWhite < 10) && (minWhite >= 0)) {
      lcd.setCursor(2, 1);
      lcd.print(0);
      lcd.setCursor(3, 1);
      lcd.print(minWhite);
    }
    else {
      lcd.setCursor(2, 1);
      lcd.print(minWhite);
    }
  }
  lcd.setCursor(4, 1);
  lcd.print(":");

  if (exitMenu == 1 && countMenu == 2) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(5, 1);
      lcd.print("  ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      if ((secWhite < 10) && (secWhite >= 0)) {
        lcd.setCursor(5, 1);
        lcd.print(0);
        lcd.setCursor(6, 1);
        lcd.print(secWhite);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(secWhite);
      }
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
 
  else {
    if ((secWhite < 10) && (secWhite >= 0)) {
      lcd.setCursor(5, 1);
      lcd.print(0);
      lcd.setCursor(6, 1);
      lcd.print(secWhite);
    }
    else {
      lcd.setCursor(5, 1);
      lcd.print(secWhite);
    }
  }
}

//LCDre kiiras (fekete karakterek)
void printTimerBlack() {

  lcd.setCursor(9, 1);
  lcd.print(hBlack);
  lcd.setCursor(10, 1);
  lcd.print(":");

  if ((minBlack < 10) && (minBlack >= 0)) {
    lcd.setCursor(11, 1);
    lcd.print(0);
    lcd.setCursor(12, 1);
    lcd.print(minBlack);
  }
  else {
    lcd.setCursor(11, 1);
    lcd.print(minBlack);
  }
  lcd.setCursor(13, 1);
  lcd.print(":");

  if ((secBlack < 10) && (secBlack >= 0)) {
    lcd.setCursor(14, 1);
    lcd.print(0);
    lcd.setCursor(15, 1);
    lcd.print(secBlack);
  }
  else {
    lcd.setCursor(14, 1);
    lcd.print(secBlack);
  }
}

//LCDre kiiras (menu szovege)
void printMenu() {

  lcd.setCursor(0, 0);
  lcd.print("Ora beallitasa");
  lcd.setCursor(8, 1);
  lcd.print("Add");

  blinkTime = millis() - blinkDelay;

  if (countMenu == 3) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(11, 1);
      lcd.print("  ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      if ((setAdd < 10) && (setAdd >= 0)) {
        lcd.setCursor(11, 1);
        lcd.print(0);
        lcd.setCursor(12, 1);
        lcd.print(setAdd);
      }
      else {
        lcd.setCursor(11, 1);
        lcd.print(setAdd);
      }
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  else {
    if ((setAdd < 10) && (setAdd >= 0)) {
      lcd.setCursor(11, 1);
      lcd.print(0);
      lcd.setCursor(12, 1);
      lcd.print(setAdd);
    }
    else {
      lcd.setCursor(11, 1);
      lcd.print(setAdd);
    }
  }

  if (exitMenu == 1 && countMenu == 4) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(14, 1);
      lcd.print("  ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      if (sidePlayer == 1) {
        lcd.setCursor(14, 1);
        lcd.print("WB");
      }
      else {
        lcd.setCursor(14, 1);
        lcd.print("BW");
      }
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  else {
    if (sidePlayer == 1) {
      lcd.setCursor(14, 1);
      lcd.print("WB");
    }
    else {
      lcd.setCursor(14, 1);
      lcd.print("BW");
    }
  }
}

//Fejlec jatek kozben
void printLabels() {

  lcd.clear();

  if (sidePlayer == 1) {
    lcd.setCursor(0, 0);
    lcd.print("WHITE");
    lcd.setCursor(11, 0);
    lcd.print("BLACK");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("BLACK");
    lcd.setCursor(11, 0);
    lcd.print("WHITE");
  }
  lcd.setCursor(7, 1);
  lcd.print("||");
  lcd.setCursor(6, 0);
  lcd.print("|  |");

  if ((setAdd < 10) && (setAdd >= 0)) {
    lcd.setCursor(7, 0);
    lcd.print(0);
    lcd.print(setAdd);
    lcd.setCursor(8, 0);
  }
  else {
    lcd.setCursor(7, 0);
    lcd.print(setAdd);
  }

  tone(pinBuzzer, 500, 300);
  digitalWrite(LED_BUILTIN, LOW);

  printTimerWhite();
  printTimerBlack();
}

//Fehernek nem telik az ideje
void pauseGameWhite() {
  while (digitalRead(btnBlack) == 0) {

    blinkTime = millis() - blinkDelay;

    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(5, 0);
      lcd.print(" ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      lcd.setCursor(5, 0);
      lcd.print("<");
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  lcd.setCursor(5, 0);
  lcd.print(" ");
}

//Feketenek nem telik az ideje
void pauseGameBlack() {
  while (digitalRead(btnWhite) == 0) {

    blinkTime = millis() - blinkDelay;

    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(10, 0);
      lcd.print(" ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      lcd.setCursor(10, 0);
      lcd.print(">");
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  lcd.setCursor(10, 0);
  lcd.print(" ");
}

//Fehernek telik az ideje
void timerWhite() {
  secWhite = setSecWhite - cSecWhite;

  printTimerWhite();

  if ((secWhite == -1) && (minWhite >= 0)) {
    setSecWhite = cSecWhite + 59;
    minWhite--;
  }

  if ((secWhite == -1) && (minWhite == -1) && (hWhite > 0)) {
    setSecWhite = cSecWhite + 59;
    minWhite = 59;
    hWhite--;
  }

  if ((secWhite == 0) && (minWhite == 0) && (hWhite == 0)) {
    tone(pinBuzzer, 500, 1000);
    while (1) {
      lcd.setCursor(10, 0);
      lcd.print("      ");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      lcd.setCursor(10, 0);
      lcd.print("Nyert");
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
}

//Feketenek telik az ideje
void timerBlack() {
  secBlack = setSecBlack - cSecBlack;

  printTimerBlack();

  if ((secBlack == -1) && (minBlack >= 0)) {
    setSecBlack = cSecBlack + 59;
    minBlack--;
  }
  if ((secBlack == -1) && (minBlack == -1) && (hBlack > 0)) {
    setSecBlack = cSecBlack + 59;
    minBlack = 59;
    hBlack--;
  }

  if ((secBlack == 0) && (minBlack == 0) && (hBlack == 0)) {
    tone(pinBuzzer, 500, 1000);
    while (1) {
      lcd.setCursor(0, 0);
      lcd.print("      ");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("Nyert");
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
}

//Feher lenyomja a gombot (kap idot)
void addWhite() {
  if (setAdd > 0) {
    secWhite = secWhite + setAdd;
    cSecWhite = setSecWhite - secWhite;

    if (secWhite > 59 && secWhite < 120) {
      secWhite = secWhite - 60;
      cSecWhite = setSecWhite - secWhite;
      minWhite++;
    }
    else if (secWhite > 119) {
      secWhite = secWhite - 120;
      cSecWhite = setSecWhite - secWhite;
      minWhite = minWhite + 2;
    }

    if (minWhite > 59) {
      minWhite = minWhite - 60;
      hWhite++;
    }
    printTimerWhite();
  }
}

//Fekete lenyomja a gombot (kap idot)
void addBlack() {
  if (setAdd > 0) {
    secBlack = secBlack + setAdd;
    cSecBlack = setSecBlack - secBlack;
    
    if (secBlack > 59 && secBlack < 120) {
      secBlack = secBlack - 60;
      cSecBlack = setSecBlack - secBlack;
      minBlack++;
    }
    else if (secBlack > 119) {
      secBlack = secBlack - 120;
      cSecBlack = setSecBlack - secBlack;
      minBlack = minBlack + 2;
    }

    if (minBlack > 59) {
      minBlack = minBlack - 60;
      hBlack++;
    }
    printTimerBlack();
  }
}

//Arduino setup
void setup() {

  pinMode(btnWhite, INPUT);
  pinMode(btnBlack, INPUT);
  pinMode(analogPin, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("VMM Fakultativ");
  lcd.setCursor(0, 1);
  lcd.print("Sakk Ora");
  delay(3000);
  lcd.clear();

  digitalWrite(LED_BUILTIN, HIGH);

  while (exitMenu) {
    menuSetUp();
    printMenu();
    printTimerWhite();
  }

  hBlack = hWhite;
  minBlack = minWhite;
  secBlack = secWhite;
  setSecBlack = secWhite;
  setSecWhite = secWhite;

  printLabels();
}

//Arduino loop
void loop() {

  adc_key_in = analogRead(analogPin);

  if (digitalRead(btnBlack)) {
    tone(pinBuzzer, 150, 100);
    cTemp = SEC - cSecWhite;
    while (digitalRead(btnWhite) == 0 && adc_key_in > 1000) {
      cSecWhite = SEC - cTemp;
      timerWhite();
      adc_key_in = analogRead(analogPin);
    }
    if (adc_key_in < 1000) {
      pauseGameWhite();
    }
    else {
      addWhite();
    }
  }

  if (digitalRead(btnWhite)) {
    tone(pinBuzzer, 150, 100);
    cTemp = SEC - cSecBlack;
    while (digitalRead(btnBlack) == 0 && adc_key_in > 1000) {
      cSecBlack = SEC - cTemp;
      timerBlack();
      adc_key_in = analogRead(analogPin);
    }
    if (adc_key_in < 1000) {
      pauseGameBlack();
    }
    else {
      addBlack();
    }
  }
}
