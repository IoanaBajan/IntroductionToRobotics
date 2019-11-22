 const int pinD1 = 13;
const int pinD2 = 12;
const int pinD3 = 11;
const int pinD4 = 10;
const int pinA = 9;
const int pinB = 8;
const int pinC = 7;
const int pinD = 6;
const int pinE = 5;
const int pinF = 4;
const int pinG = 3;
const int pinDP = 2;


const int pinSW = A2;
const int pinX = A0;
const int pinY = A1;

int xValue;
int yValue;
int swValue;
bool joyMovedY = 0;
bool joyMovedX = 0;
const int minThreshold = 400;
const int maxThreshold = 600;
const int segSize = 8;
const int nOfDisplays = 4;
const int nOfDigits = 10;
int lastSW = 0;
int digit = 0;
int currentDisplay = 0;
bool buttonState = 1;

int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[nOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};

int numbers[nOfDisplays] = {
  0, 0, 0, 0
};

byte digitMatrix[nOfDigits][segSize - 1 ] = {
{ 1 , 1 , 1 , 1 , 1 , 1 , 0 }, // 0
{ 0 , 1 , 1 , 0 , 0 , 0 , 0 }, // 1
{ 1 , 1 , 0 , 1 , 1 , 0 , 1 }, // 2
{ 1 , 1 , 1 , 1 , 0 , 0 , 1 }, // 3
{ 0 , 1 , 1 , 0 , 0 , 1 , 1 }, // 4
{ 1 , 0 , 1 , 1 , 0 , 1 , 1 }, // 5
{ 1 , 0 , 1 , 1 , 1 , 1 , 1 }, // 6
{ 1 , 1 , 1 , 0 , 0 , 0 , 0 }, // 7
{ 1 , 1 , 1 , 1 , 1 , 1 , 1 }, // 8
{ 1 , 1 , 1 , 1 , 0 , 1 , 1}
};

void displayNumber( byte digit, int state) {
  for ( int i = 0 ; i < segSize - 1 ; i++) {
    digitalWrite (segments[i], digitMatrix[digit][i]);
  }
  digitalWrite (segments[segSize - 1], state);
 delay(1);
}

void showDigit( int num) {
  for ( int i = 0 ; i < nOfDisplays; i++) {
    digitalWrite (digits[i], HIGH );
  }
  digitalWrite (digits[num], LOW );
}

void setup () {
  for ( int i = 0; i < segSize; i++) {
    pinMode (segments[i], OUTPUT);
  }
  for ( int i = 0 ; i < nOfDisplays; i++)  {
    pinMode (digits[i], OUTPUT );
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  swValue = digitalRead(pinSW);
  if(swValue != lastSW){
  if(swValue == 0)
    buttonState = !buttonState;
  }
  lastSW = swValue;

  Serial.print("buttonState=");
  Serial.println(buttonState);
  
  if(buttonState != 0) {
    xValue = analogRead(pinX);
    if(xValue < minThreshold && joyMovedX == false) {
      if(currentDisplay > 0)
        currentDisplay --;
      else
        currentDisplay = 3;
        
    joyMovedX = true;
    }
    if(xValue > maxThreshold && joyMovedX == false ) {
      if(currentDisplay < 3)
        currentDisplay++;
      else
        currentDisplay = 0;     
      joyMovedX = true;    
    }
  
  if(xValue > minThreshold && xValue < maxThreshold){
    joyMovedX = false;
    }

  }
  if(buttonState == 0){
    yValue = analogRead(pinY);
    if(yValue < minThreshold && joyMovedY == false){
     if(numbers[currentDisplay] > 0) {
      numbers[currentDisplay] --;
     }else {
      numbers[currentDisplay] = 9;
     }
     joyMovedY = true;
  }
  if(yValue > maxThreshold && joyMovedY == false){
    if(numbers[currentDisplay] < 9) {
      numbers[currentDisplay] ++;
    }else{
      numbers[currentDisplay] = 0;
    }
    joyMovedY = true;
  }
  if(yValue >= minThreshold && yValue <= maxThreshold){
       joyMovedY = false;
      }
  }
  for ( int i = 0 ; i < nOfDisplays; i++) {
    showDigit(i);
    if(i == currentDisplay) {
      digitalWrite(segments[7], HIGH);
          }
    else {
     displayNumber(numbers[i],LOW);
    }
    delay(3);
  }  
}
