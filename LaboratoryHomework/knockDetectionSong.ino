#define NOTE_G3 196
const int speakerPin = A0;
const int buzzerPin = 11;
const int threshold = 10;
const int pushButton = 2 ;

int buttonState = 0 ;
//int buzzerTone = 1000;
int speakerValue = 0;
int waitStart = 0;

void setup(){
  pinMode( speakerPin, INPUT);
  pinMode(pushButton, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
   buttonState = digitalRead(pushButton);
   
   if(speakerValue != 0){
    Serial.println(speakerValue);
  }
   
  if(speakerValue < threshold){  
    speakerValue = analogRead(speakerPin);
    waitStart = millis(); 
  }
  
  if(speakerValue >= threshold){
    Serial.println("KNOCK ");
    if((millis() - waitStart) >= 5000){   
      tone(buzzerPin, 196);
     }
     
    if(buttonState != 0){
      noTone(buzzerPin);
      speakerValue = 0;
      }   
  }  
}
