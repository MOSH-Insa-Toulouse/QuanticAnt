/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

//Sens de rotations et positions initiales
int sensA =1;
int sensB =-1;
int sensC =1;
int posA = 59;
int posB = 139;
int posC = 65;
Servo phaseA;  // Moteur droite
Servo phaseB;  // Moteur gauche
Servo phaseC;  // Moteur equilibrage


void setup() {
  phaseA.attach(9);  // attaches the servo on pin 9 to the servo object
  phaseB.attach(10);  // attaches the servo on pin 9 to the servo object
  phaseC.attach(11);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
     phaseA.write(posA);              // tell servo to go to position in variable 'pos'
    phaseB.write(posB);              // tell servo to go to position in variable 'pos'
    phaseC.write(posC);   
  delay(500);

  pinMode(3, INPUT);
  
}

// vitesse de 1 a 5
// dir valeurs acceptees 2, 4, 6, 8.(pave num)
void motrice(int vitesse, int dir)
{
  if(posA<=59)
      sensA =4;
  if( posA >= 130)
      sensA = -4;

    if(posB<=59)
    {
      sensB =4;
      if(dir ==2 || dir == 6)
        posC = 70;
      else
        posC = 110;
    }
    if( posB >= 130)
    {
      if(dir ==2 || dir == 8)
        posA = 59;
     if(dir ==4 || dir == 8)
        posA = 130;
      sensB = -4;
      if(dir == 8 || dir == 4)
        posC = 70;
      else
        posC = 110;
    }
    
    posA+=sensA;
    posB+=sensB; 
    phaseA.write(posA);              // tell servo to go to position in variable 'pos'
    phaseB.write(posB);              // tell servo to go to position in variable 'pos'
    phaseC.write(posC);              // tell servo to go to position in variable 'pos'
    delay(80/vitesse);  
}

void loop() {
    // in steps of 1 degree

    
  
  
}

