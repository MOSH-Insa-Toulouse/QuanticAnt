/***********************************************************************
 * Projet MOSH de l'équipe Nicolas, Quentin et Soufian
 *  
 * Le projet vise à réaliser un robot imitant le comportement d'un insect.
 * -Déplacements aléatoires sur pates
 * -Photophobe
 * -Evitement d'obstacles proches
 * -s'imobilise et "Chante" lorsque la luminosité est faible
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
#define LUXL A0//Lumiere Front (doit etre analog)
#define LUXB A1//Lumiere Back (doit etre analog)
#define LUXR A2//Lumiere Left (doit etre analog)
#define LUXF A3//Lumiere Right (doit etre analog)
#define LEDR 2 // led droite rouge
#define LEDL 7 // led gauche rouge

#define PROX_TH 30 // distance en cm a partir de laquelle le robot veut changer de direction

//Variables globales
int C=0; //compteur de loop avec overflow
int quantic=0; // Valeur aleatoire entre 0 et 100 qui change a chaque boucle
//----Variables Motrices
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
/*
 *   phaseA.attach(3);
  phaseB.attach(5); 
  phaseC.attach(6);
 */
int ClearanceF = 100;
int ClearanceR = 100;
int ClearanceL = 100;





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
      tone(9, melody[thisNote], noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(9);
  }
}


//Fonction qui retourne la distance en mm entre le capteur "Broche" et un obstacle à moins de 2m.
//Bloque le code pendant 6ms max
float DistUS(int Broche) {
  float Dist = 0.0;
  //Impulsion pour le capteur US
  digitalWrite(Broche, LOW);
  delayMicroseconds(5);
  digitalWrite(Broche, HIGH);
  delayMicroseconds(10);
  digitalWrite(Broche, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ECHO, INPUT);
  Dist = (pulseIn(ECHO, HIGH, 15000)/2) / 29.1;
  // Convert the time into a distance. Divide by 29.1 or multiply by 0.0343
  if(Dist == 0)
   return ( 200.0);
  else
   return (Dist);   

}


// Retourne l'angle entre l'orientation du robot et la direction SOMBRE
int DirLowLux() {
  //Front, Right, Back, Left 0° -> 360° sens horaire
  int LumF =  analogReadN(LUXF, 10);
  int LumL =  analogReadN(LUXL, 10);
  int LumR =  analogReadN(LUXR, 10);
  int LumB =  analogReadN(LUXB, 10);

  if(LumF > LumR && LumF > LumL && LumF > LumB)
   return(2);
  if(LumR > LumF && LumR > LumL && LumR > LumB)
   return(4);
 if(LumB > LumR && LumB > LumL && LumB > LumF)
   return(8);
 if(LumL > LumR && LumL > LumF && LumL > LumB)
   return(6);  
}

int MaxLux() {
  int maxL = analogReadN(LUXF, 10);
  if( analogReadN(LUXL, 10) > maxL)
     maxL = analogReadN(LUXL, 10);
  if( analogReadN(LUXR, 10) > maxL)
     maxL = analogReadN(LUXR, 10);
  if( analogReadN(LUXB, 10) > maxL)
     maxL = analogReadN(LUXB, 10);
  return maxL;
}

//Controle des yeux rouge du robot
void yeux(int droite, int gauche)
{
  digitalWrite(LEDR, droite);
  digitalWrite(LEDL, gauche);
}

// Deplace QuanticAnt a la vitesse speed. La valeur Dir donne la direction selon le pave numerique
//789 : 8 = avancer
//456 : 4 et 6 = tourner
//123 : 2 = reculer
void Motrice(int vitesse, int dir)
{

  //La phase A oscille entre 60 et 130
  if(posA<=59)
      sensA =4;
  if( posA >= 130)
      sensA = -4;

  // La phase B oscille entre 60 et 130
  if(posB<=59)
  {
      sensB =4;
      //Controle de la phase C au moment ou B est en butee
      if(dir ==2 || dir == 6)
        posC = 70;
      else // Dans le cas de la marche avant ou rotation Droite on inverse le mouvement
        posC = 110;
  }
    if( posB >= 130)
    {
      sensB = -4;

      //Correction du dephasage avec A selon la direction
      if(dir ==2 || dir == 8)
        posA = 59;
      if(dir ==4 || dir == 6)
        posA = 130;

      //Correction du dephasage avec C selon la direction  
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
  
  //Preparation des moteurs
  phaseA.attach(3);
  phaseB.attach(5); 
  phaseC.attach(6);
  Serial.begin(9600);
     phaseA.write(posA);              // tell servo to go to position in variable 'pos'
    phaseB.write(posB);              // tell servo to go to position in variable 'pos'
    phaseC.write(posC);   
  delay(500);

}

// the loop function runs over and over again forever
/*Logique de bord :
1) Verifier la luminosité. Sombre => arret et chant
2) sinon verifier la position des obstacles => manoeuvre
3) sinon aller dans la direction la plus sombre
*/
void loop() {


  
  quantic = random(0, 100);

  //Recuperation des distances aux objets
  ClearanceF = DistUS(USF);
  ClearanceL = DistUS(USL);
  ClearanceR = DistUS(USR);

//  Serial.println(ClearanceF);
//  Serial.println(ClearanceL);
//  Serial.println(ClearanceR);
//    Serial.println("--------");





//   Un objet est trop proche droit devant


    //controle des yeux dans le noir
  if(MaxLux() < 300)
  {
    if(quantic < 10)
      yeux(LOW, LOW);
    if(quantic >= 10 && quantic < 40)
      yeux(LOW, HIGH);
    if(quantic >= 40 && quantic < 80)
      yeux(HIGH, LOW);
    if(quantic >= 80)
      yeux(HIGH, HIGH);
    bruit();
  }
  else // Il fait jour, on se deplace
  {
    yeux(LOW, LOW);
 
    if(ClearanceL < PROX_TH || ClearanceR < PROX_TH)
    {
      if((ClearanceR < ClearanceL)) // Tourner vers gauche
      {
         Motrice(3, 4);
      }
      else // Tourner vers droite
      {
         Motrice(3, 6);
      }
    }
    if(ClearanceF < PROX_TH)
    {
    for(int j=0 ; j<100 ; j++) 
      Motrice(2, 2);
    for(int j=0 ; j<300 ; j++) 
      Motrice(2, 6);

    }
    if(ClearanceL>PROX_TH && ClearanceR>PROX_TH && ClearanceF>PROX_TH) // Aucun objet ne gene. On cherche le sombre
    {
    Motrice(8, 2);
    Serial.println(DirLowLux());
   // Motrice(8, 8);
   }

  }
  

  C++;
  //delay(300);
}

