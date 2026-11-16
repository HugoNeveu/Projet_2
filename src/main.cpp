#include <Arduino.h>
#include <MOMO_RGB_Matrix.h>
#include <bits_manip (1).h>

#define ENC_CHA_ARD 3//Pin de l'encodeur channel A Arduino
#define ENC_CHA PE5   // pin de l'encodeur channel A
#define ENC_CHB_ARD 5 // pin de l'encodeur channel B Arduino
#define ENC_CHB PE3   // pin de l'encodeur channel B

#define bH PC2
#define bBAS PC7
#define bG PC1
#define bD PC3
#define bA PC6
#define bB PC4
#define bC PC5
#define bE PG5
#define POT PF6

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

// Define voiture size
#define POSITION_INIT_X 30
#define POSITION_INIT_Y 15
#define LARGEUR_VOITURE_NS 3
#define HAUTEUR_VOITURE_NS 5
#define LARGEUR_VOITURE_EO 5
#define HAUTEUR_VOITURE_EO 3

// Résultats possibles de checkCollision()
#define LIBRE 0
#define MUR 1

#define OBSTACLE 2
#define COIN 3

volatile long encoderVal = 0;
//Les noms HAUT, BAS, GAUCHE, DROITE, BTN_HAUT … BTN_C et BTN_ENC viennent de defines_matrice.h, comme dans le code de test.
MOMO_RGB_Matrix matrix;

bool bitmap_voitureNordSud[HAUTEUR_VOITURE_NS][LARGEUR_VOITURE_NS] = {
    0, 1, 0,
    1, 1, 1,
    1, 1, 1,
    1, 1, 1,
    0, 1, 0};
 
bool bitmap_voitureEstOuest[HAUTEUR_VOITURE_EO][LARGEUR_VOITURE_EO] = {
 
    0, 1, 1, 1, 0,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0};

enum Etats
{
  Debut,
  Menu,
  ChoixNiveau,
  SelectionNiveau,
  ChoixCouleur,
  Jeu,
  FinTour,
};

Etats etatMachine = Debut;

void ISR_encodeur(){
  bool valeurCHB = isBitSet(PINE, ENC_CHB);
  if(!valeurCHB){
    encoderVal++;
  }
  else{
    encoderVal--;
  }
}

void etatdebut(){
  matrix.setCursor(5, 2);
  matrix.setTextColor(matrix.Color333(255, 0, 255));
  matrix.print("Kart Game");
  matrix.drawLine(4, 10, 58, 10, matrix.Color333(0, 255, 255));
  matrix.setCursor(30, 20);
  matrix.setTextColor(matrix.Color333(255, 0, 0));
  matrix.print("A");
  matrix.drawCircle(32, 23, 5, matrix.Color333(0, 255, 255));
  if(isBitSet(PINC, bA) == LOW){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = Menu;
  }
}

void selectionNiveau(){
 matrix.drawLine(21, 0, 21, 31, matrix.Color333(0, 255, 255));
 matrix.drawLine(41, 0, 41, 31, matrix.Color333(0, 255, 255));
 if(isBitSet(PINC, bB) == LOW){
  etatMachine = ChoixNiveau;
  matrix.fillScreen(matrix.Color333(0, 0, 0));
 }
}

void etatchoixNiveau(){
 matrix.drawRect(10, 11, 45, 9, matrix.Color333(0, 255, 255));
 matrix.setCursor(14, 12);
 matrix.setTextColor(matrix.Color333(255, 0, 0));
 matrix.print("NIVEAU");
 matrix.setCursor(1, 1);
 matrix.print("C");
 matrix.drawTriangle(56, 11, 56, 19, 62, 15, matrix.Color333(255, 0, 255));
 if(encoderVal > 0){
  encoderVal = 0;
  etatMachine = Menu;
  matrix.fillScreen(matrix.Color333(0, 0, 0));
 }
 else if(isBitSet(PING, bE) == LOW){
  etatMachine = SelectionNiveau;
  matrix.fillScreen(matrix.Color333(0, 0, 0));
 }
 else if(isBitSet(PINC, bC) == LOW){
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  etatMachine = Debut;
 }
}

void etetchoixCouleur(){

}

void etatmenu(){
 matrix.drawRect(10, 11, 45, 9, matrix.Color333(0, 255, 255));
 matrix.setCursor(12, 12);
 matrix.setTextColor(matrix.Color333(255, 0, 0));
 matrix.setCursor(18, 12);
 matrix.setTextColor(matrix.Color333(255, 0, 0));
 matrix.print("JOUER");
 matrix.setCursor(1, 1);
 matrix.print("C");
 matrix.drawTriangle(56, 11, 56, 19, 62, 15, matrix.Color333(255, 0, 255));
 matrix.drawTriangle(8, 11, 8, 19, 1, 15, matrix.Color333(255, 0, 255));

 if(encoderVal < 0){
  encoderVal = 0;
  etatMachine = ChoixNiveau;
  matrix.fillScreen(matrix.Color333(0, 0, 0));
 }
 else if(isBitSet(PING, bE) == LOW){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = Jeu;
  }
 else if(isBitSet(PINC, bC) == LOW){
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  etatMachine = Debut;
 }
}

void afficheVoitureNS(int x, int y) // Affiche la voiture en position Nord-Sud à la position (x, y)
{
  for (int i = 0; i < HAUTEUR_VOITURE_NS; i++)
  {
    for (int j = 0; j < LARGEUR_VOITURE_NS; j++)
    {
      if (bitmap_voitureNordSud[i][j])
      {
        matrix.drawPixel(x + j, y + i, matrix.Color333(255, 0, 0)); // Couleur rouge
      }
    }
  }
}
 
void afficheVoitureEO(int x, int y) // Affiche la voiture en position Est-Ouest à la position (x, y)
{
  for (int i = 0; i < HAUTEUR_VOITURE_EO; i++)
  {
    for (int j = 0; j < LARGEUR_VOITURE_EO; j++)
    {
      if (bitmap_voitureEstOuest[i][j])
      {
        matrix.drawPixel(x + j, y + i, matrix.Color333(255, 0, 0)); // Couleur rouge
      }
    }
  }
}
 
 
void orientationVoiture(int &x, int &y) // Déplace la voiture en fonction des boutons pressés, choisis le bon bool en fonction de la position de la voiture (Nord-Sud ou Est-Ouest)
{
  bool voitureNS = true; // true si la voiture est en position Nord-Sud, false si elle est en position Est-Ouest
  bool voitureEO = false; // true si la voiture est en position Est-Ouest, false si elle est en position Nord-Sud
 
  if (voitureNS)
  {
    afficheVoitureNS(x, y);
  }
  else if (voitureEO)
  {
    afficheVoitureEO(x, y);
  }
 
}
 
void deplaceVoiture(int &x, int &y) // Déplace la voiture en fonction des boutons pressés
{
  if (isBitSet(PINC, bH) == LOW) // Si le bouton haut est pressé
  {
    y--; // Déplacer la voiture vers le haut
  }
  else if (isBitSet(PINC, bBAS) == LOW) // Si le bouton bas est pressé
  {
    y++; // Déplacer la voiture vers le bas
  }
  else if (isBitSet(PINC, bG) == LOW) // Si le bouton gauche est pressé
  {
    x--; // Déplacer la voiture vers la gauche
  }
  else if (isBitSet(PINC, bD) == LOW) // Si le bouton droit est pressé
  {
    x++; // Déplacer la voiture vers la droite
  }
 
  orientationVoiture(x, y); // Afficher la voiture à sa nouvelle position
}

void etatJeu()
{
  int x = POSITION_INIT_X;
  int y = POSITION_INIT_Y;
 
  matrix.fillScreen(matrix.Color333(0, 0, 0)); // Effacer l'écran
  deplaceVoiture(x, y); // Afficher la voiture à sa position initiale
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  DDRC = setBitM(DDRC, B11111110);
  PORTC = setBitM(PORTC, B11111110);
  DDRG &= ~(1 << bE);
  PORTG = setBitM(PORTG, bE);

  attachInterrupt(1, ISR_encodeur, RISING);
}

void loop() {
 switch (etatMachine){
  case Debut:
  etatdebut();
  break;

  case Menu:
  etatmenu();
  break;

  case ChoixNiveau:
  etatchoixNiveau();
  break;

  case SelectionNiveau:
  selectionNiveau();
  break;

  case ChoixCouleur:

  break;

  case Jeu:

  break;

  case FinTour:

  break;

 }
}
