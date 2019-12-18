#include "LedControl.h" 
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
const int pinSW = A0;
const int pinY = A1;
const int pinX = A2;
const int data = 12;
const int load = 10;
const int clock = 13;

const int minThreshold = 300;
const int maxThreshold = 700;
int joyMovedX = false;
int joyMovedY = false;
int xValue;
int yValue;
int level = 1;
struct player {
  int col, row;
  bool dead = 0;
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
{ {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1}
};
void setup() {
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 15); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
    p.row = 1;
    p.col = 3;
  
}
void showMatrix() {
  for (int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
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
    if (p.col <= 7) {
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
    if (p.row <=6 ) {
      p.row++;
    }
    else p.row = 1;
    joyMovedY = true;
  }

  if (yValue > maxThreshold && joyMovedY == false ) {

    if (p.row >= 1) {
      p.row--;
    }
    else p.row = 6;
    joyMovedY = true;
  }
  if (yValue > minThreshold && yValue < maxThreshold) {
    joyMovedY = false;
  }
  
  checkIfDead(p.row, p.col);
 
}

void moveCar(int pos, int sizeCar, unsigned long int timer) {
  lc.setLed(0, pos, (timer-1) % 8, 0);
  for(int i = 0; i < sizeCar; i++) {
    matrix[pos][(timer+i)%8] = 1;
    
  }
  delay(100);
}
void lostGame() {
  for ( int i = 1; i < 7; i++)
    for (int j = 0; j < 8; j++)
     {
     matrix[i][j] = 0;
     }  
}
void checkIfDead(int x, int y) {
  if(matrix[x][y] == 1 && x!=7 && y!=7) {
    lostGame();
     //p.row = 1;
   //  p.col = 3;
  }
}

bool checkIfWin(int row) {
  if( row == 7) 
    return true;
   else return false;
}

void displayRandomCoin() {
  coinX = rand()%7;
  coinY = rand()%7;
  matrix[coinX][coinY] = 1;
}

void loop() {  
  matrix[p.row][p.col] = 1;
  showMatrix();
  calculateTime();
 if(level == 1) {
  
//  score = millis();
  moveCar(4, 3, t02);
  moveCar(6, 3, t03);
  matrix[p.row][p.col] = 0;
  movePlayer();
  if(checkIfWin(p.row) == true) {
   for ( int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
      matrix[i][j] = 1;
      level++;
      p.row = 1;
      p.col = 3;
   }
  // score = (millis() - score) *
 }
 
 if(level == 2) {  
  moveCar(2, 3, t01);
//  moveCar(3, 3, t04);
  moveCar(4, 3, t02);
  movePlayer();
  //checkIfDead(p.row, p.col);
  if(checkIfWin(p.row) == true) {
   for ( int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
      matrix[i][j] = 1;
      level++;
      p.row = 1;
      p.col = 2;
   }
 }
}
