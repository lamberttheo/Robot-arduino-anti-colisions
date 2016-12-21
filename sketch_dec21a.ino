#define motorPin1a 3    // Marche avant du premier moteur
#define motorPin1b 4    // Marche arrière du premier moteur
#define speedPin1 9     // pin d'activation L293D pour le premier moteur
#define motorPin2a 5    // Marche avant du deuxième moteur
#define motorPin2b 6    // Marche arrière du deuxième moteur
#define speedPin2 10    // pin d'activation L293D pour le deuxième moteur

// capteur à ultrasons:
#define trigPinDroite 12     
#define echoPinDroite 11
#define trigPinCentre A0
#define echoPinCentre A1
#define trigPinGauche A2
#define echoPinGauche A3

// variables globales

int Mspeed = 0;  // vitesse du moteur
int seuil = 30; // distance minimale pour laquelle on accepte un obstacle


long look (short capteur){  // évaluation de la distance de l'obstacle

  long temps;
  short trigPin, echoPin;

  if (capteur == 0){  // capoteur de droite
    trigPin = trigPinDroite;
    echoPin = echoPinDroite;
  }

  if (capteur == 1){  // capoteur du centre
    trigPin = trigPinCentre;
    echoPin = echoPinCentre;
  }

  if (capteur == 2){  // capoteur de gauche
    trigPin = trigPinGauche;
    echoPin = echoPinGauche;
  }

  // Nous envoyons un signal haut d'une durée de 10 microsecondes, en sandwich
  // entre deux signaux bas.  Des ultrasons sont émis pendant que le signal est haut

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lors de la réception de l'écho, le module HC-SR04 émet
  // un signal logique haut (5 v) dont la durée est égale au
  // temps écoulé entre l'émission et la réception de l'ultrason.
  pinMode(echoPin, INPUT);
  temps = pulseIn(echoPin, HIGH);

  return temps * 340/(2*10000); 
}


void setup() {

  // réglage des broches à output
  pinMode(motorPin1a, OUTPUT);
  pinMode(motorPin1b, OUTPUT);
  pinMode(speedPin1, OUTPUT);
  pinMode(motorPin2a, OUTPUT);
  pinMode(motorPin2b, OUTPUT);
  pinMode(speedPin2, OUTPUT);

  pinMode(echoPinDroite, INPUT);
  pinMode(trigPinDroite, OUTPUT);
  digitalWrite(trigPinDroite, LOW);
  pinMode(echoPinCentre, INPUT);
  pinMode(trigPinCentre, OUTPUT);
  digitalWrite(trigPinCentre, LOW);
  pinMode(echoPinGauche, INPUT);
  pinMode(trigPinGauche, OUTPUT);
  digitalWrite(trigPinGauche, LOW);

}



void loop() {
  long distanceDroite, distanceCentre, distanceGauche;
  long previousDroite, previousCentre, previousGauche;

  Mspeed = 700; // vitesse du moteur 0 à 1023

  distanceDroite = look(0);  // y a-t-il un obstacle à droite?
  distanceCentre = look(1);  // y a-t-il un obstacle au centre?
  distanceGauche = look(2);  // y a-t-il un obstacle à gauche?


  if (distanceDroite > seuil && distanceCentre > seuil && distanceGauche > seuil){

    // aucun obstacle détecté, donc marche avant:

    analogWrite(speedPin1, Mspeed); 
    digitalWrite(motorPin1a, HIGH); 
    digitalWrite(motorPin1b, LOW); 

    analogWrite(speedPin2, Mspeed); 
    digitalWrite(motorPin2a, HIGH); 
    digitalWrite(motorPin2b, LOW); 

    delay(100);
  }

  else {  // on a détecté un obstacle
    // nouvelle vérification pour éviter les faux positifs
    previousDroite = distanceDroite;
    previousCentre = distanceCentre;
    previousGauche = distanceGauche;

    distanceDroite = look(0);  // y a-t-il un obstacle à droite?
    distanceCentre = look(1);  // y a-t-il un obstacle au centre?
    distanceGauche = look(2);  // y a-t-il un obstacle à gauche?

    if ((distanceDroite <= seuil) && (previousDroite)<= seuil ){
      // obstacle confirmé

      if (distanceDroite < distanceGauche) {  //  on tourne à gauche

        analogWrite(speedPin1, Mspeed); 
        digitalWrite(motorPin1a, HIGH);
        digitalWrite(motorPin1b, LOW); 

        analogWrite(speedPin2, Mspeed); 
        digitalWrite(motorPin2a, LOW); 
        digitalWrite(motorPin2b, HIGH); 

        delay(110);

        if (distanceGauche <= seuil){  // il y a aussi un obstacle proche à gauche
          delay(700);  // on continue de tourner un peu plus
        }

      }
      else // l'obtacle à gauche est plus proche que l'obstacle à droite
      {
        // on tourne assez longtemps vers la droite

        analogWrite(speedPin1, Mspeed); 
        digitalWrite(motorPin1a, LOW);
        digitalWrite(motorPin1b, HIGH); 

        analogWrite(speedPin2, Mspeed); 
        digitalWrite(motorPin2a, HIGH); 
        digitalWrite(motorPin2b, LOW); 

        delay(900);  // tournons de façon significative
      }


    }
    else if ((distanceGauche <= seuil) &&(previousGauche <= seuil)) {
      // obstacle à gauche seulement, on tourne à droite

      analogWrite(speedPin1, Mspeed); 
      digitalWrite(motorPin1a, LOW);
      digitalWrite(motorPin1b, HIGH); 

      analogWrite(speedPin2, Mspeed); 
      digitalWrite(motorPin2a, HIGH); 
      digitalWrite(motorPin2b, LOW); 

      delay(100);
    }
    else if ((distanceCentre <= seuil) && (previousCentre <= seuil)){
      // mince obstacle droit devant, on tourne à droite

      analogWrite(speedPin1, Mspeed); 
      digitalWrite(motorPin1a, LOW);
      digitalWrite(motorPin1b, HIGH); 

      analogWrite(speedPin2, Mspeed); 
      digitalWrite(motorPin2a, HIGH); 
      digitalWrite(motorPin2b, LOW); 

      delay(90);
    }
  }
}
