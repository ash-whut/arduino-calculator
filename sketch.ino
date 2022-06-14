#include "Keypad.h"
#include <LiquidCrystal_I2C.h>

#include <ctype.h>

// #include "number.h"

// CONSTANTS 
double firstNum = 0;
double secondNum = 0;
char awperator = '0';

bool firstNumEntered = false;
bool firstDigitEntered = false;


// FOR BUZZER
const int buzzer = 13;

// FOR KEYPAD
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
// END OF KEYPAD INITIALIZATION

LiquidCrystal_I2C lcd(0x27, 20, 4); // FOR I2C 16x2 LCD DISPLAY 

// SCREEN RESET
void lcdRestart(){
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("A:ADD");

  lcd.setCursor(0, 1);
  lcd.print("B:SUB");

  lcd.setCursor(0, 2);
  lcd.print("C:MUL");

  lcd.setCursor(0, 3);
  lcd.print("D:DIV");

  lcd.setCursor(15, 0);
  lcd.print("#:=");

  lcd.setCursor(14, 2);
  lcd.print("*:QUIT");
}

void setup(){
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcdRestart();
}

void loop(){
  char customKey = customKeypad.getKey();

  if (customKey == '*'){
    lcdRestart();

    firstNum = 0;
    secondNum = 0;
    awperator = '0';
    firstNumEntered = false;
    firstDigitEntered = false;
  }

  else if (((awperator == '0') && (!firstNumEntered)) || (awperator == '#')){
    if (customKey){
      tone(buzzer, 500, 50); // BEEEEEP
      lcd.clear();

      if (awperator == '#'){
        if (isdigit(customKey)){
          firstNum = customKey - '0';
          lcd.print(firstNum);
          awperator = '0';
          firstNumEntered = false;
          firstDigitEntered = false;
        }

        else{
          awperator = customKey;
          lcd.print(firstNum);
        }
      }

      else if (isdigit(customKey)){
        firstNum = (firstNum * 10) + (customKey - '0');
        lcd.print(firstNum);
        firstDigitEntered = true;
      }

      else {
        if (firstDigitEntered){
          firstNumEntered = true;
        }
        
        lcd.clear();

        awperator = customKey;

        lcd.setCursor(0, 2);
        lcd.print(firstNum);
      }
    }
  }

  else{
    if (customKey){
      tone(buzzer, 500, 50);
      lcd.clear();

      if (isdigit(customKey)){
        secondNum = (secondNum * 10) + (customKey - '0');
        lcd.print(secondNum);
      }

      else {
        switch (awperator){
          case 'A':
            firstNum += secondNum;
            break;
          case 'B':
            firstNum -= secondNum;
            break;
          case 'C':
            firstNum *= secondNum;
            break;
          case 'D':
            firstNum /= secondNum;
            break;
        }

        lcd.clear();
        lcd.setCursor(0, 2);
        lcd.print(firstNum);
        secondNum = 0;
        awperator = customKey;
      }
    }
  }
}