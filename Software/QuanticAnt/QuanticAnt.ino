/***********************************************************************
 * 
 * .______   .______        ______          __   _______ .___________.   .___  ___.   ______        _______. __    __  
|   _  \  |   _  \      /  __  \        |  | |   ____||           |   |   \/   |  /  __  \      /       ||  |  |  | 
|  |_)  | |  |_)  |    |  |  |  |       |  | |  |__   `---|  |----`   |  \  /  | |  |  |  |    |   (----`|  |__|  | 
|   ___/  |      /     |  |  |  | .--.  |  | |   __|      |  |        |  |\/|  | |  |  |  |     \   \    |   __   | 
|  |      |  |\  \----.|  `--'  | |  `--'  | |  |____     |  |        |  |  |  | |  `--'  | .----)   |   |  |  |  | 
| _|      | _| `._____| \______/   \______/  |_______|    |__|        |__|  |__|  \______/  |_______/    |__|  |__| 
                                                                                                                    
 ___     ___    __    ___         ___     ___    __    ___                                                          
|__ \   / _ \  /_ |  / _ \       |__ \   / _ \  /_ |  / _ \                                                         
   ) | | | | |  | | | (_) |  ______ ) | | | | |  | | | (_) |                                                        
  / /  | | | |  | |  > _ <  |______/ /  | | | |  | |  \__, |                                                        
 / /_  | |_| |  | | | (_) |       / /_  | |_| |  | |    / /                                                         
|____|  \___/   |_|  \___/       |____|  \___/   |_|   /_/                                                          
                                                                                                                    
  ______      __    __       ___      .__   __. .___________. __    ______         ___      .__   __. .___________. 
 /  __  \    |  |  |  |     /   \     |  \ |  | |           ||  |  /      |       /   \     |  \ |  | |           | 
|  |  |  |   |  |  |  |    /  ^  \    |   \|  | `---|  |----`|  | |  ,----'      /  ^  \    |   \|  | `---|  |----` 
|  |  |  |   |  |  |  |   /  /_\  \   |  . `  |     |  |     |  | |  |          /  /_\  \   |  . `  |     |  |      
|  `--'  '--.|  `--'  |  /  _____  \  |  |\   |     |  |     |  | |  `----.    /  _____  \  |  |\   |     |  |      
 \_____\_____\\______/  /__/     \__\ |__| \__|     |__|     |__|  \______|   /__/     \__\ |__| \__|     |__|      
                                                                                                                    
 * de l'équipe Nicolas, Quentin et Soufian
 *  
 * Le projet vise à réaliser un robot imitant le comportement d'un insecte.
 * -Déplacements aléatoires sur pattes
 * -Photosensible
 * -Evitement d'obstacles proches
 * -s'immobilise et "Chante" lorsque la luminosité est faible
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
#define NIGHTLEV 400 // Niveau de lumiere correspondant a la nuit (plus grande la valeur, plus facilement le robot passe en mode nuit)
//Variables globales
int C=0; //compteur de loop avec overflow
int quantic=0; // Valeur aleatoire entre 0 et 100 qui change a chaque boucle
int ClearanceF = 100; //distances ultrason mesurees front (defaut 1m)
int ClearanceR = 100;
int ClearanceL = 100;

//----Variables Motrices
//Sens de rotations et positions initiales
int sensA =1;
int sensB =-1;
int sensC =1;
int posA = 59;
int posB = 139;
int posC = 65;
Servo phaseA;  // Moteur droite pin 3
Servo phaseB;  // Moteur gauche pin 5
Servo phaseC;  // Moteur equilibrage pin 6








//Retourne la moyenne de N mesures (moins de bruit) Attention, ne pas utiliser sur un pont diviseur fragile. (>10kohm)
long analogReadN(int pin, int N)
{
  long moy = 0;
  for(int i=0; i<N ; i++)
    moy += analogRead(pin);
    
  return(moy/N);
}



// Fonction qui genere un bruit une fois invoquee. Inspiree de l'exemple "tone"
// Fonction bloquante, attention ! (le temps du bruit)
void bruit() {
    
    int melody[] =        {208, 494, 415, 494, 415}; //tons du buzzer
    int noteDurations[] = {96, 96, 96, 96, 96}; // Durees des tons
  
    for (int thisNote = 0; thisNote < 5; thisNote++)
    {
      int noteDuration = 3000 / noteDurations[thisNote];
      int pauseBetweenNotes = noteDuration * 1.30;
      tone(BUZZ, melody[thisNote], noteDuration);
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(BUZZ);
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
 
  // Recupere le signal echo des capteurs
  // La duree est en microseconds
  pinMode(ECHO, INPUT);
  Dist = (pulseIn(ECHO, HIGH, 15000)/2) / 29.1;
  // conversion du temps en distance en divisant par 29.1
  if(Dist == 0)
   return ( 200.0);
  else
   return (Dist);   

}


// Retourne La direction sombre
int DirLowLux() {
  //Front, Right, Back, Left 0° -> 360° sens horaire
  int LumF =  analogReadN(LUXF, 10);
  int LumL =  analogReadN(LUXL, 10);
  int LumR =  analogReadN(LUXR, 10);
  int LumB =  analogReadN(LUXB, 10);

  if(LumF > LumR && LumF > LumL && LumF > LumB)
   return(2); // Derriere
  if(LumR > LumF && LumR > LumL && LumR > LumB)
   return(4); // gauche
 if(LumB > LumR && LumB > LumL && LumB > LumF)
   return(8); // Devant
 if(LumL > LumR && LumL > LumF && LumL > LumB)
   return(6); // Droite
}

//Retourne la luminosite la plus forte (sonde les 4 capteurs).
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
      if(dir ==8 || dir == 4)
        posC = 70;
      else // Dans le cas de la marche avant ou rotation Droite on inverse le mouvement
        posC = 110;
  }
    if( posB >= 130)
    {
      sensB = -4;

      //Correction du dephasage avec A selon la direction
      if(dir ==8 || dir == 2)
        posA = 59;
      if(dir ==4 || dir == 6)
        posA = 130;

      //Correction du dephasage avec C selon la direction  
      if(dir == 2 || dir == 6)
        posC = 70;
      else
        posC = 110;


    }
    
    posA+=sensA;
    posB+=sensB;
    phaseA.write(posA);           
    phaseB.write(posB);         
    phaseC.write(posC);        
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
  

  randomSeed(analogRead(A4)); // Recuperation des parasites pour generer le random
  
  //Preparation des moteurs
  phaseA.attach(3);
  phaseB.attach(5); 
  phaseC.attach(6);
  
  phaseA.write(posA);              
  phaseB.write(posB);        
  phaseC.write(posC);   

  
  Serial.begin(9600);


  //Le robot dit bonjour
  for(int j=0; j<5 ;j++)
  {
    bruit();
    yeux(HIGH, LOW);
    delay(50);
    yeux(LOW, HIGH);
    delay(50);
  }
  yeux(LOW, LOW);
}


/*Logique de bord :
1) Verifier la luminosité. Sombre => arret et chant
2) sinon verifier la position des obstacles => manoeuvre
3) sinon aller vers l'avant

Evolution possible : Utiliser les 4 capteurs de luminosite pour diriger le robot vers le noir.
Les capteurs sont presents mais pas utilises dans le code actuel. La fonction qui determine la direction sombre
est deja codee.
*/


void loop() {


  
  quantic = random(0, 100); // Nombre aleatoire de ce cycle

  //Recuperation des distances aux objets (prend du temps !!)
  ClearanceF = DistUS(USF);
  ClearanceL = DistUS(USL);
  ClearanceR = DistUS(USR);

  Serial.println("Avant "+String(ClearanceF));
  Serial.println("Gauche "+String(ClearanceL));
  Serial.println("Droite "+String(ClearanceR));
  Serial.println("--------");

  //Comportement dans le noir
  if(MaxLux() < NIGHTLEV)
  {
    //Comportement aleatoire des yeux
    if(quantic < 10)
      yeux(LOW, LOW);
    if(quantic >= 10 && quantic < 40)
      yeux(LOW, HIGH);
    if(quantic >= 40 && quantic < 80)
      yeux(HIGH, LOW);
    if(quantic >= 80)
      yeux(HIGH, HIGH);
      
    phaseC.write(90); // relaxe le moteur avant 
    delay(100);
    if(C%7 == 0)
      bruit(); //Blocant quelques diziemes de secondes
      
  }
  else // Il fait jour, on se deplace
  {
    yeux(LOW, LOW);
 
    //Gestion des collisions.
    if((ClearanceR < PROX_TH)) // Tourner vers gauche
    {
       yeux(HIGH, HIGH);
       delay(1300); // Le robot fait le mort
       for(int j=0 ; j<130 ; j++) 
        Motrice(3, 4);
    }
    else if( ClearanceL < PROX_TH)// Tourner vers droite
    {
       yeux(HIGH, HIGH);
       delay(1300); // Le robot fait le mort
       for(int j=0 ; j<130 ; j++) 
        Motrice(3, 6);
    }
    else if(ClearanceF < PROX_TH)// Reculer puis demi-tour
    {
     yeux(HIGH, HIGH);
     delay(1300); // Le robot fait le mort
    for(int j=0 ; j<300 ; j++) 
      Motrice(5, 2);
    for(int j=0 ; j<300 ; j++) 
      Motrice(5, 4);

    }
    if(ClearanceL>PROX_TH && ClearanceR>PROX_TH && ClearanceF>PROX_TH) // Aucun objet ne gene. On execute la marche habituelle du robot
    {

      //Fonction de marche aleatoire 1% dns un sens, 3% dans l'autre, 96% tout droit 
//    if(quantic < 2) // Parfois l'insecte tourne sans raison dans un sens
//    {
//     for(int j=0 ; j<30 ; j++) 
//        Motrice(3, 4);
//    }
//    if(quantic >= 2 && quantic < 5) // ou dans l'autre !
//    {
//     for(int j=0 ; j<30 ; j++) 
//        Motrice(3, 6);
//    }
//    if(quantic >= 5) // Le reste du temps il avance
//    {
//    for(int j=0 ; j<30 ; j++)   
//      Motrice(3, 8);
//    }
//    Serial.println(quantic);
//    Motrice(8, DirLowLux()); // Suivi de direction sombre desactive. (instable, besoin de lissage).
//    Serial.println("LowLUX "+String(DirLowLux()));
      for(int j=0 ; j<40 ; j++)   
      Motrice(3, 8);

    
   }

  }
  

  C++; // Compteur de boucle
}

