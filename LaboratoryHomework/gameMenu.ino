#include <LiquidCrystal.h>

const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;
const int RS = 12;
const int enable = 11;
const int pinSW = A2;
const int pinY = A1;
const int pinX = A0;
int xValue = 0;
int yValue = 0;
int swValue = 0;
int positionX;
unsigned long last = 0;
int levelValue = 0;
int buttonState;
int lastButtonState = HIGH;
bool joyMovedX = false;
bool joyMovedY = false;
const int minThreshold = 400;
const int maxThreshold = 600;
const int interval = 5000;
int mainScreen = 0;
int ok = 0;
int ok2 = 0;
int printedMessage = 0;
int rounds = 0;
int result = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);

void setup() {


  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  lcd.setCursor(1, 0);
  lcd.print("Start");
  lcd.setCursor(8, 0);
  lcd.print("Setup");
  lcd.setCursor(1, 1);
  lcd.print("HighScore");

}
void startGame() {

  lcd.setCursor(0, 0);
  lcd.print("Lives:3");
  lcd.setCursor(8, 0);
  lcd.print("Level:");
  lcd.setCursor(14, 0);
  lcd.print(levelValue);
  lcd.setCursor(15, 0);
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.setCursor(6, 1);
  lcd.print(levelValue * 3);
  lcd.print("   ");
}

void mainMenu() {
  xValue = analogRead(pinX);
  if (xValue > maxThreshold && joyMovedX == false) {
    positionX ++;
    if (positionX == 3) {
      positionX = 0;
    }
    joyMovedX = true;
  }

  if (xValue < minThreshold && joyMovedX == false) {
    positionX --;
    if (positionX == -1) {
      positionX = 2;
    }
    joyMovedX = true;
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    joyMovedX = false;
  }

  lcd.setCursor(0, 0);
  if (positionX == 0) {
    lcd.print(">Start ");
  } else {
    lcd.print(" Start ");
  }


  lcd.setCursor(7, 0);
  if (positionX == 1) {
    lcd.print(">Settings");
  } else {
    lcd.print(" Settings");
  }


  lcd.setCursor(0, 1);
  if (positionX == 2) {
    lcd.print(">HighScore");
  } else {
    lcd.print(" HighScore");
  }

}
void debounceButton() {
  int reading = digitalRead(pinSW);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        mainScreen = !mainScreen;
        last = millis();
        ok = 0;
        ok2 = 0;
        rounds = 0;
        printedMessage = 0;
        lcd.clear();
      }
    }
  }
  lastButtonState = reading;
}

void loop() {

  if (mainScreen == 0) {

    debounceButton();
    mainMenu();

  } else if (mainScreen != 0) {

    if (positionX == 0) {
      if (ok == 0) {
        lcd.clear();
        ok = 1;
      }
      if (rounds <= 2) {
        startGame();
        unsigned long current = millis();
        if ((current - last) >= interval) {
          levelValue ++;
          last = current;
          rounds++;
          if (levelValue == 100) {
            levelValue = 99;
          }
        }
        result = 3 * (levelValue - 1);
      } else   {
        if (ok2 == 0) {
          lcd.clear();
          ok2 = 1;
          lcd.setCursor(0, 0);
          lcd.print("Congratulations!");
        }
        Serial.println(millis());
        Serial.println(last);
        Serial.println(printedMessage);

        if ( ((millis() - last) > 1000) && (printedMessage == 0) ) {
          lcd.clear();
          last = millis();
          lcd.setCursor(0, 0);
          lcd.print("You finished");
          lcd.setCursor(0, 1);
          lcd.print("the game!");
          printedMessage = 1;
        }

        if ((printedMessage == 1) && (millis() - last) > 1000) {
          lcd.clear();
          last = millis();
          lcd.setCursor(0, 0);
          lcd.print("Press button");
          lcd.setCursor(0, 1);
          lcd.print("to exit.");
          printedMessage = 2;
        }

        if (printedMessage == 2) {
          debounceButton();
        }
      }
    }


    if (positionX == 1) {
      if (ok == 0) {
        lcd.clear();
        ok = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("Starting level");

      yValue = analogRead(pinY);
      if (yValue < minThreshold && joyMovedY == false) {
        levelValue++;
        if (levelValue == 100) {
          levelValue = 0;
        }

        joyMovedY = true;
      }
      if (yValue > maxThreshold && joyMovedY == false) {
        levelValue--;
        if (levelValue == -1) {
          levelValue = 99;
        }

        joyMovedY = true;

      }
      if (yValue < maxThreshold && yValue > minThreshold) {
        joyMovedY = false;
      }

      lcd.setCursor(0, 1);
      if (levelValue < 10) {
        lcd.print(0);
        lcd.setCursor(1, 1);
        lcd.print(levelValue);
      } else {
        lcd.print(levelValue);
        lcd.print(" ");
      }

      debounceButton();

    }
    if (positionX == 2) {
      if (ok == 0) {
        lcd.clear();
        ok = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("HighScore");
      lcd.setCursor(0, 1);
      lcd.print(result);
      debounceButton();

    }

  }
}
