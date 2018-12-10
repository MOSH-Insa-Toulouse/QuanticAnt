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

#define USL //Ultrason Left
#define USF //Ultrason Front
#define USR //Ultrason Right
#define ECHO //Retour ultrason
#define BUZZ //Buzzer pin (doit etre PWM)
#define LUXF //Lumiere Front (doit etre analog)
#define LUXB //Lumiere Back (doit etre analog)
#define LUXL //Lumiere Left (doit etre analog)
#define LUXR //Lumiere Right (doit etre analog)


#define PROX_TH 10 // distance en cm a partir de laquelle le robot veut changer de direction



//----Variables motrices
Servo servoF;  // Objet associe au servo avant
Servo servoB;  // Objet associe au servo arriere
Servo servoC;  // Objet associe au servo central






//Retourne la moyenne de N mesures (moins de bruit) Attention, ne pas utiliser sur un pont diviseur fragile
long analogReadN(int pin, int N)
{
  long moy = 0;
  for(int i=0; i<N ; i++)
    moy += analogRead(Pin);
    
  return(moy/N);
}



//Fonction qui retourne la distance en mm entre le capteur "Broche" et un obstacle à moins de 2m.
//Bloque le code pendant 6ms max
// Souf
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
// Quentin (souf avait commence)
int DirLowLux() {
  //Front, Right, Back, Left 0° -> 360° sens horaire
  LumF =  analogReadN(LUXF, 10);
  LumL =  analogReadN(LUXL, 10);
  LumR =  analogReadN(LUXR, 10);
  LumB =  analogReadN(LUXB, 10);

  if(LumF > LumR && LumF > LumL && LumF > LumB)
   return(0);
  if(LumR > LumF && LumR > LumL && LumR > LumB)
   return(90);
 if(LumB > LumR && LumB > LumL && LumB > LumF)
   return(180);
 if(LumL > LumR && LumL > LumF && LumL > LumB)
   return(270);  
}


// Deplace QuanticAnt a la vitesse speed et avec un angle (+ => gauche, - => droite). Angle = 90 => rotation sur place.
//Quentin
Motrice(int angle, int speed)
{
  // A completer souf :)
}





// Execution unique
void setup() {
  
  //Initialisation des capteurs US
  pinMode(USL, OUTPUT);
  pinMode(USF, OUTPUT);
  pinMode(USR, OUTPUT);
  pinMode(ECHO, OUTPUT);
  digitalWrite(USL, LOW);
  digitalWrite(USF, LOW);
  digitalWrite(USR, LOW);

}

// the loop function runs over and over again forever
void loop() {


  random(0, 100); //Nouvelles envies de QuanticAnt

  // Un objet est trop proche droit devant
  if(dist(USF) < PROX_TH || dist(USR) < PROX_TH/2 ||  dist(USL) < PROX_TH/2)
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

  // Envie de bouger ? + Vitesse voulue

  // Rotation lumiere

  // Deplacement


  // Bruit ?
  
}

