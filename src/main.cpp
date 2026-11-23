#include <Arduino.h>
#include <MOMO_RGB_Matrix.h>
#include <bits_manip.h>

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

int ancienPot = -1;
//variable pour les parametres de couleur
int valCouleur = 1;
int SelecteurDeCouleur = 1;
int CouleurR = 255, CouleurG = 255, CouleurB = 255;

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


enum Couleur{
  NOTHING,
  ROUGE,
  VERT,
  BLEU
};

Couleur teinte = NOTHING;

enum Etats
{
  Debut,
  Menu,
  ChoixNiveau,
  SelectionNiveau,
  ChoixCouleur,
  SelectionCouleur,
  GestionCouleur,
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

int valNiveau = 1;
int choixDuNiveau = 1;
void Bouton_Niveau(){
  if((isBitSet(PINC, bD) == LOW) && valNiveau < 3){
    valNiveau++;
    delay(80);
  }
  else if((isBitSet(PINC, bG) == LOW) && valNiveau > 0){
    valNiveau--;
    delay(80);
  }
}

void selectionNiveau(){
  matrix.drawLine(21, 0, 21, 31, matrix.Color333(0, 255, 255));
  matrix.drawLine(42, 0, 42, 31, matrix.Color333(0, 255, 255));
  matrix.setTextColor(matrix.Color333(0, 255, 0));
  matrix.setCursor(1, 1);
  matrix.print("C");
  matrix.setCursor(58, 24);
  matrix.print("A");
  matrix.setCursor(8, 6);
  matrix.setTextColor(matrix.Color333(255, 0, 255));
  matrix.print("1");
  matrix.setCursor(30, 6);
  matrix.print("2");
  matrix.setCursor(51, 6);
  matrix.print("3");
  Bouton_Niveau();

  if(valNiveau == 1){
    matrix.fillCircle(10, 22, 2, matrix.Color333(255, 255, 0));
    matrix.fillCircle(32, 22, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(53, 22, 2, matrix.Color333(0, 0, 0));
  }

  else if(valNiveau == 2){
    matrix.fillCircle(10, 22, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(32, 22, 2, matrix.Color333(255, 255, 0));
    matrix.fillCircle(53, 22, 2, matrix.Color333(0, 0, 0));
  }

  else if(valNiveau == 3){
    matrix.fillCircle(10, 22, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(32, 22, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(53, 22, 2, matrix.Color333(255, 255, 0));
  }

  if(isBitSet(PINC, bC) == LOW){
    etatMachine = ChoixNiveau;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
 }

 if(isBitSet(PINC, bA) == LOW){
    choixDuNiveau = valNiveau;
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


// Affiche une valeur numérique (0 à 255) à une position donnée, précédée d'une lettre.
// On efface seulement la zone du texte (4 caractères de 6 px = 24 px x 8 px) pour éviter le clignotement.
void afficheValeur(int x, int y, int valeur)
{
  matrix.fillRect(x, y, 24, 8, matrix.Color333(0, 0, 0)); // efface l'ancienne valeur
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(255, 255, 255));
  matrix.setCursor(x, y);
  matrix.print(valeur);
}

// Fonction pour potentiometre avec entree analogique A6, sa valeur de 0 a 255, cette valeur est affichee en chiffre
// en haut a gauche de la matrice.
void lirePotentiometre()
{
  int valeurPotentiometre = analogRead(PIN_POTENTIOMETRE); // Lire la valeur du potentiomètre (0 à 1023)
  valeurPotentiometre = valeurPotentiometre >> 2;          // Ramener sur 0 à 255

  // Réafficher seulement si la valeur a changé de plus de 1 (filtre le bruit de l'ADC)
  if (ancienPot < 0 || abs(valeurPotentiometre - ancienPot) > 1)
  {
    afficheValeur(25, 15, valeurPotentiometre); // Au centre
    ancienPot = valeurPotentiometre;
  }
}

void EtatRouge(){
  matrix.setCursor(1, 1);
  matrix.setTextColor(matrix.Color333(255, 0, 0));
  matrix.print("-ROUGE-");
  matrix.setCursor(58, 24);
  matrix.print("B");
  lirePotentiometre();
  if(isBitSet(PINC, bB) == LOW){
    CouleurR = ancienPot;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = SelectionCouleur;
    teinte = NOTHING;
  }
}

void EtatVert(){
  matrix.setCursor(1, 1);
  matrix.setTextColor(matrix.Color333(0, 0, 255));
  matrix.print("-VERT-");
  matrix.setCursor(58, 24);
  matrix.print("B");
  lirePotentiometre();
  if(isBitSet(PINC, bB) == LOW){
    CouleurG = ancienPot;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = SelectionCouleur;
    teinte = NOTHING;
  }
}

void EtatBleu(){
  matrix.setCursor(1, 1);
  matrix.setTextColor(matrix.Color333(0, 255, 0));
  matrix.print("-BLEU-");
  matrix.setCursor(58, 24);
  matrix.print("B");
  lirePotentiometre();
  if(isBitSet(PINC, bB) == LOW){
    CouleurB = ancienPot;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = SelectionCouleur;
    teinte = NOTHING;
  }
}

//Fonction qui vas attribuer les couleur choisi a la voiture
void EtatGestionCouleur(){
  if(SelecteurDeCouleur == 1){
    teinte = ROUGE;
  }
  if(SelecteurDeCouleur == 2){
    teinte = VERT;
  }
  if(SelecteurDeCouleur == 3){
    teinte = BLEU;
  }
}

void Bouton_Couleur(){
  if((isBitSet(PINC, bD) == LOW) && valCouleur < 3){
    valCouleur++;
    delay(80);
  }
  else if((isBitSet(PINC, bG) == LOW) && valCouleur > 0){
    valCouleur--;
    delay(80);
  }
}
//Fonction qui affiche le menu sélection de la couleur et ses paramètre
void etatSelectionCouleur(){
  matrix.drawLine(21, 0, 21, 31, matrix.Color333(0, 255, 255));
  matrix.drawLine(42, 0, 42, 31, matrix.Color333(0, 255, 255));
  matrix.setTextColor(matrix.Color333(0, 255, 0));
  matrix.setCursor(1, 1);
  matrix.print("C");
  matrix.setCursor(58, 24);
  matrix.print("A");
  matrix.setCursor(8,6);
  matrix.setTextColor(matrix.Color333(255, 0, 0));
  matrix.print("R");
  matrix.setCursor(2, 14);
  matrix.print(CouleurR);
  matrix.setTextColor(matrix.Color333(0, 0, 255));
  matrix.setCursor(30, 6);
  matrix.print("G");
  matrix.setCursor(24, 14);
  matrix.print(CouleurG);
  matrix.setTextColor(matrix.Color333(0, 255, 0));
  matrix.setCursor(51, 6);
  matrix.print("B");
  matrix.setCursor(45, 14);
  matrix.print(CouleurB);
  Bouton_Couleur();
  if(valCouleur == 1){
    matrix.fillCircle(10, 27, 2, matrix.Color333(255, 0, 0));
    matrix.fillCircle(32, 27, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(53, 27, 2, matrix.Color333(0, 0, 0));
  }

  else if(valCouleur == 2){
    matrix.fillCircle(10, 27, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(32, 27, 2, matrix.Color333(0, 0, 255));
    matrix.fillCircle(53, 27, 2, matrix.Color333(0, 0, 0));
  }

  else if(valCouleur == 3){
    matrix.fillCircle(10, 27, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(32, 27, 2, matrix.Color333(0, 0, 0));
    matrix.fillCircle(53, 27, 2, matrix.Color333(0, 255, 0));
  }

  if(isBitSet(PINC, bC) == LOW){
    etatMachine = ChoixCouleur;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    delay(50);
 }

 if(isBitSet(PINC, bA) == LOW){
    SelecteurDeCouleur = valCouleur;
    etatMachine = GestionCouleur;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
 }

}
//Fonction qui affiche le bouton pour accèder au menu des couleur
void etetchoixCouleur(){
  matrix.drawRect(10, 11, 45, 9, matrix.Color333(0, 255, 255));
  matrix.setCursor(12, 12);
  matrix.setTextColor(matrix.Color333(255, 0, 0));
  matrix.print("COULEUR");
  matrix.setCursor(1, 1);
  matrix.print("C");
  matrix.drawTriangle(8, 11, 8, 19, 1, 15, matrix.Color333(255, 0, 255));
  if(encoderVal < 0){
    encoderVal = 0;
    etatMachine = Menu;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
 }
  if(isBitSet(PING, bE) == LOW){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = SelectionCouleur;
  }
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
  else if(encoderVal > 0){
    encoderVal = 0;
    etatMachine = ChoixCouleur;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
  if(isBitSet(PING, bE) == LOW){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    etatMachine = Jeu;
  }
  if(isBitSet(PINC, bC) == LOW){
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
  etetchoixCouleur();
  break;

  case SelectionCouleur:
  etatSelectionCouleur();
  break;

  case GestionCouleur:
  EtatGestionCouleur();
  break;

  case Jeu:

  break;

  case FinTour:

  break;

 }


 switch (teinte){
  case NOTHING:
  //Pour ne pas afficher les paramètre de couleur continuellement
  break;

  case ROUGE:
  EtatRouge();
  break;

  case VERT:
  EtatVert();
  break;

  case BLEU:
  EtatBleu();
  break;
 }
}
