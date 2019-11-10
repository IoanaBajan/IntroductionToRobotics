const int potPin1 = A0;
const int potPin2 = A1;
const int potPin3 = A2; 

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int potValue1 = 0;
int potValue2 = 0;
int potValue3 = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(potPin3, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(9600);

}

void loop() {
  potValue1 = analogRead(potPin1);
  potValue2 = analogRead(potPin2);
  potValue3 = analogRead(potPin3);


  redValue = map(potValue1, 0, 1023, 0, 255);
  greenValue = map(potValue2, 0, 1023, 0, 255);
  blueValue = map(potValue3, 0, 1023, 0, 255);

  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
  
  Serial.println(redValue);
  Serial.println(greenValue);
  Serial.println(blueValue);
  delay(1);
}
