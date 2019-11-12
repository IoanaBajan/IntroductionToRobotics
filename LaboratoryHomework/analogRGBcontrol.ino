const int potRed = A0;
const int potGreen = A1;
const int potBlue = A2; 

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int potValueRed = 0;
int potValueGreen = 0;
int potValueBlue = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  pinMode(potRed, INPUT);
  pinMode(potGreen, INPUT);
  pinMode(potBlue, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(9600);

}

void loop() {
  potValueRed = analogRead(potRed);
  potValueGreen = analogRead(potGreen);
  potValueBlue = analogRead(pot Blue);

  redValue = map(potValueRed, 0, 1023, 0, 255);
  greenValue = map(potValueGreen, 0, 1023, 0, 255);
  blueValue = map(potValueBlue, 0, 1023, 0, 255);

  setColor(redValue, greenValue, blueValue);
  
  Serial.println(redValue);
  Serial.println(greenValue);
  Serial.println(blueValue);
  delay(1);
  
}

void setColor(int red, int green, int blue){

 analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  
}
