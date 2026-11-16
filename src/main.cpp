#include <Arduino.h>
#include <MOMO_RGB_Matrix.h>
#include <bits_manip (1).h>

#define ENC_CHA_ARD 3//Pin de l'encodeur channel A Arduino
#define ENC_CHA PE5   // pin de l'encodeur channel A
#define ENC_CHB_ARD 5 // pin de l'encodeur channel B Arduino
#define ENC_CHB PE3   // pin de l'encodeur channel B

#define DELR1 PG2
#define DELG1 PG1
#define DELB1 PG0
#define DELR2 PL7
#define DELG2 PL6
#define DELB2 PL5
#define DELR3 PL4
#define DELG3 PL3
#define DELB3 PL2

#define PIN_POTENTIOMETRE A6 // potentiomètre (vitesse de la voiture)

#define TEMPS_LECTURE_ANALOG 100   // ms entre deux lectures du potentiomètre
#define TEMPS_INTRO 3000           // ms de l'animation d'intro
#define TEMPS_FIN_TOUR 5000        // ms d'affichage du temps et des coins
#define TEMPS_INVINCIBLE 1000      // ms sans perdre de coin après un obstacle

#define ADRESSE_EEPROM_TEMPS 0     // adresse de départ des meilleurs temps dans l'EEPROM

// Résultats possibles de checkCollision()
#define LIBRE 0
#define MUR 1

#define OBSTACLE 2
#define COIN 3
//Les noms HAUT, BAS, GAUCHE, DROITE, BTN_HAUT … BTN_C et BTN_ENC viennent de defines_matrice.h, comme dans le code de test.
MOMO_RGB_Matrix matrix;

void debut(){
  matrix.setCursor(5, 2);
  matrix.setTextColor(matrix.Color333(255, 0, 255));
  matrix.print("Kart Game");
  matrix.drawLine(5, 10, )
  matrix.setCursor(30, 20);
  matrix.setTextColor(matrix.Color333(255, 0, 0));
  matrix.print("A");
}

void choixNiveau(){

}

void choixCouleur(){

}

void menu(){
 matrix.fillRect(10, 10, 53, 21, matrix.Color333(255, 0, 255));
 matrix.setCursor(12, 12);
 matrix.setTextColor(matrix.Color333(255, 0, 0));
}

enum Etats
{
  Debut,
  Menu,
  ChoixNiveau,
  ChoixCouleur,
  Jeu,
  FinTour,
};

Etats etatMachine = Debut;

void setup() {
 Serial.begin(9600);
 matrix.begin();
}

void loop() {
 switch (etatMachine){
  case Debut:
  debut();
  break;

  case Menu:
  menu();
  break;

  case ChoixNiveau:

  break;

  case ChoixCouleur:

  break;

  case Jeu:

  break;

  case FinTour:

  break;

 }
}
