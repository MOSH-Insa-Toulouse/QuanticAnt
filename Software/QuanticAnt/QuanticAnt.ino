/***********************************************************************
 * Projet MOSH de l'équipe Nicolas, Quentin et Soufian
 *  
 * Le projet vise à réaliser un robot imitant le comportement d'un insect.
 * -Déplacements aléatoires, Photophobe, évitement d'obstacles
 * 
 * 
 * 
 ***********************************************************************/

#include <Servo.h> // Voir l'exemple "SERVO" dans le menu arduino

#define USL 11 //Ultrason Left
#define USF 8//Ultrason Front
#define USR 10//Ultrason Right
#define ECHO 12//Retour ultrason
#define BUZZ 9//Buzzer pin (doit etre PWM)
#define LUXF A0//Lumiere Front (doit etre analog)
#define LUXB A1//Lumiere Back (doit etre analog)
#define LUXL A2//Lumiere Left (doit etre analog)
#define LUXR A3//Lumiere Right (doit etre analog)


#define PROX_TH 10 // distance en cm a partir de laquelle le robot veut changer de direction



//----Variables motrices
Servo servoL;  // Objet associe au servo gauche
Servo servoR;  // Objet associe au servo droit
Servo servoD;  // Objet associe au servo arriere






//Retourne la moyenne de N mesures (moins de bruit) Attention, ne pas utiliser sur un pont diviseur fragile
long analogReadN(int pin, int N)
{
  long moy = 0;
  for(int i=0; i<N ; i++)
    moy += analogRead(pin);
    
  return(moy/N);
}



// Fonction qui genere un bruit une fois invoquee.
// Fonction bloquante, attention (le temps du bruit)
void bruit() {
    int silence = random(1, 6); // duree du silence avant de reprendre le champ
    // notes du champ du robot
    int melody[] = {
      208, 494, 415, 494, 415, 0
    };
    //NOTE_B2, NOTE_B3, NOTE_B2, 
    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int noteDurations[] = {
      3*32, 3*32, 3*32, 3*32, 3*32, silence
    };//32, 32, 32, 
    for (int thisNote = 0; thisNote < 6; thisNote++) {
  
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 3000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
  }
}


//Fonction qui retourne la distance en mm entre le capteur "Broche" et un obstacle à moins de 2m.
//Bloque le code pendant 6ms max
float DistUS(int Broche) {
  
  //Impulsion pour le capteur US
  digitalWrite(Broche, HIGH);
  delayMicroseconds(10);
  digitalWrite(Broche, LOW);
  
  // Attente de l'echo... 6ms timout (environ 2m, on est un insecte apres tout, on voit pas loin)
  long dtus = pulseIn(ECHO, HIGH, 6000);
  
  float dist = (dtus * 0.34)/2.0; //conversion en cm
   
  return(dist);
  //Les 3 lignes précedentes peuvent etre combinees en 1 (reduisant la lisibilite)
}


// Retourne l'angle entre l'orientation du robot et la direction CLAIRE
int DirLowLux() {
  //Front, Right, Back, Left 0° -> 360° sens horaire
  int LumF =  analogReadN(LUXF, 10);
  int LumL =  analogReadN(LUXL, 10);
  int LumR =  analogReadN(LUXR, 10);
  int LumB =  analogReadN(LUXB, 10);

  if(LumF > LumR && LumF > LumL && LumF > LumB)
   return(2);
  if(LumR > LumF && LumR > LumL && LumR > LumB)
   return(5);
 if(LumB > LumR && LumB > LumL && LumB > LumF)
   return(3);
 if(LumL > LumR && LumL > LumF && LumL > LumB)
   return(4);  
}


// Deplace QuanticAnt a la vitesse speed et avec un angle (+ => gauche, - => droite). Angle = 90 => rotation sur place.
//Quentin
void Motrice(int angle, int speed)
{
  // A completer souf :)
}





// Execution unique
void setup() {
  
  //Initialisation des capteurs US
  pinMode(LUXF, INPUT);
  pinMode(LUXB, INPUT);
  pinMode(LUXR, INPUT);
  pinMode(LUXL, INPUT);
  pinMode(USL, OUTPUT);
  pinMode(USF, OUTPUT);
  pinMode(USR, OUTPUT);
  pinMode(ECHO, OUTPUT);
  digitalWrite(USL, LOW);
  digitalWrite(USF, LOW);
  digitalWrite(USR, LOW);
  
  
  randomSeed(analogRead(A4));

}

// the loop function runs over and over again forever
void loop() {


  delay(300);
  // Un objet est trop proche droit devant
 /* if(dist(USF) < PROX_TH || dist(USR) < PROX_TH/2 ||  dist(USL) < PROX_TH/2)
  {
    if(dist(USR) < dist(USL) ) // Tourner vers droite
    {
       Motrice(-90, V);
    }
    else // Tourner vers gauche
    {
       Motrice(90, V);
    }
  }
*/
  // Envie de bouger ? + Vitesse voulue

  // Rotation lumiere

  // Deplacement


  // Bruit ?
  
}

