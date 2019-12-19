#include "LedControl.h"
#include <LiquidCrystal.h>
const int pinSW = A0;
const int pinY = A1;
const int pinX = A2;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;
const int RS = 9;
const int enable = 8;
int positionX;
unsigned long last = 0;
int levelValue = 0;
int buttonState;
int lastButtonState = HIGH;
const int interval = 5000;
int mainScreen = 0;
int ok = 0;
int ok2 = 0;
int printedMessage = 0;
int rounds = 0;
int score = 0;
int bestScore = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long wait = 0;
const int intervalMoveCar = 100;
int lives = 3;
const int minThreshold = 300;
const int maxThreshold = 700;
int joyMovedX = false;
int joyMovedY = false;
int xValue;
int yValue;
int level = 1;
struct player {
  int col, row;
  int notDead = 1;
};
long int t0 = 0;
long int t1 = 0;
long int t2 = 0;
long int t3 = 0;
long int t4 = 0;
int t01;
int t02;
int t03;
int t04;
int coinX;
int coinY;
player p;

byte matrix[8][8] =
{ {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  lcd.setCursor(1, 0);
  lcd.print(">Start");
  lcd.setCursor(8, 0);
  lcd.print(" Setup");
  lcd.setCursor(1, 1);
  lcd.print(" HighScore");
  lcd.setCursor(10, 1);
  lcd.print(" INFO");
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 15); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  p.row = 1;
  p.col = 3;

}
// --GRAFICS--
void showMatrix() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col] % 2);
    }
  }
  lc.setLed(0,p.row, p.col, 1);
}

void displayRandomCoin() {
  coinX = rand() % 7;
  coinY = rand() % 7;
  matrix[coinX][coinY] = 1;
}

void moveCar(int pos, int sizeCar, unsigned long int timer) {

  lc.setLed(0, pos, (timer - 1) % 8, 0);

  for (int i = 0; i < sizeCar; i++) {
    matrix[pos][(timer + i) % 8] = 1;

  }
  
}

//--GAME DYNAMICS--
void calculateTime() {

  if (millis() - t0 > 100) {
    movePlayer();
    t0 = millis();
  }

  if (millis() - t1 > 400) {
    t01++;
    t1 = millis();
  }

  if (millis() - t2 > 500) {
    t02++;
    t2 = millis();
  }

  if (millis() - t3 > 200) {
    t03++;
    t3 = millis();
  }

  if (millis() - t4 > 100) {
    t04++;
    t4 = millis();
  }
}

void movePlayer() {
  //move on x axis
  xValue = analogRead(pinX);
  if (xValue < minThreshold && joyMovedX == false) {
    if (p.col >= 0) {
      p.col--;
    }
    else p.col = 7;

    joyMovedX = true;
  }

  if (xValue > maxThreshold && joyMovedX == false ) {
    if (p.col < 7) {
      p.col++;
    }
    else p.col = 0;

    joyMovedX = true;
  }
  if (xValue > minThreshold && xValue < maxThreshold) {
    joyMovedX = false;
  }

  //move on y axis
  yValue = analogRead(pinY);
  if (yValue < minThreshold && joyMovedY == false) {
    if (p.row < 7 ) {
      p.row++;
    }
    else p.row = 7;
    joyMovedY = true;
  }

  if (yValue > maxThreshold && joyMovedY == false ) {

    if (p.row >= 1) {
      p.row--;
    }
    else p.row = 1;
    joyMovedY = true;
  }
  if (yValue > minThreshold && yValue < maxThreshold) {
    joyMovedY = false;
  }

  if ( checkIfDead(p.row, p.col) == 1) {
    lives--;
    if (lives) {
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
          if (i == j || i + j == 7)
            lc.setLed(0, i, j, 1);
            showMatrix();
      reset();

    }
  }

}
void reset() {
  int pushState = digitalRead(pinSW);
  lostGame();
  p.col = 2;
  p.row = 1;
  p.notDead = 0;

}

// --GAMEPLAY--

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
  lcd.print(levelValue);
  lcd.print("   ");
  //matrix[p.row][p.col] = 1;
  showMatrix();
  calculateTime();
  if (level == 1) {
    moveCar(4, 3, t02);
    moveCar(6, 3, t03);
    lc.setLed(0, p.row, p.col, 0);
    movePlayer();
    lc.setLed(0, p.row, p.col, 1);

    if (checkIfWin(p.row) == true) {
      for ( int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
          matrix[i][j] = 1;
      level++;
      p.row = 1;
      p.col = 3;
    }
  }
  if (level == 2) {
    moveCar(2, 3, t01);
    moveCar(4, 3, t03);
    
    lc.setLed(0, p.row, p.col, 0);
    movePlayer();
    lc.setLed(0, p.row, p.col, 1);
    
    if (checkIfWin(p.row) == true) {
      for ( int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
          matrix[i][j] = 1;
      p.notDead++;
      if (p.notDead == 3) {
        lives++;
        p.notDead = 1;
      }
      levelValue++;
      p.row = 1;
      p.col = 2;
    }
  }

}
void lostGame() {
  for ( int i = 1; i < 7; i++)
    for (int j = 0; j < 8; j++)
    {
      matrix[i][j] = 0;
    }
}
bool checkIfDead(int x, int y) {
  if (matrix[x][y] == 1 && x != 7 && x != 0 && y != 7) {
    return true;
  }
  else return false;
}

bool checkIfWin(int row) {
  if (row == 7)
    return true;
  else return false;
}

// --LCD--

void mainMenu() {
  xValue = analogRead(pinX);
  if (xValue > maxThreshold && joyMovedX == false) {
    positionX ++;
    if (positionX == 4) {
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

  lcd.setCursor(10, 1);
  if (positionX == 3) {
    lcd.print(">INFO");
  } else {
    lcd.print(" INFO");
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
        wait = millis();
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
    debounceButton();
    if (positionX == 0) {
      if (ok == 0) {
        lcd.clear();
        ok = 1;
      }
      if (rounds <= 2) {
        startGame();
        unsigned long current = millis();
        if ((current - last) >= interval) {
          last = current;
          rounds++;
          if (levelValue == 100) {
            levelValue = 99;
          }
        }
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
      lcd.print(score);
      debounceButton();

    }
     if (positionX == 3) {
      if (ok == 0) {
        lcd.clear();
        ok = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("Bajan Ioana");
      lcd.setCursor(0, 1);
      lcd.print("UnibucRobotics");
      
     }

  }
}
