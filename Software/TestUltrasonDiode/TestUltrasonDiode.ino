int trigPin[3] = {9, 13, 2};    // Trigger
int echoPin = 12;    // Echo
long duration, cm;
 int i = 0;
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin[0], OUTPUT);
  pinMode(trigPin[1], OUTPUT);
  pinMode(trigPin[2], OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin[i%3], LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin[i%3], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin[i%3], LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  
  Serial.print("no"+String(1+i%3)+" ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  if(i%3==2){
      Serial.println();
      delay(1000);}

  delay(50);
  i++;
}
