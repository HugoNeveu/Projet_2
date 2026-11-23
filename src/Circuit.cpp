#include <Arduino.h>
#include <Circuit.h>
    int Hauteur_matrice = 32;
    int Largeur_matrice = 64;

    enum Niveau{
        UN,
        DEUX,
        TROI
    };
    Niveau choix = UN;
 //Construcleur
    Circuit::Circuit(){
        this->niveau = 1;
    }

    Circuit::Circuit(int niveau){
        this->niveau = niveau;
    }

    //Get
    int Circuit::getNiveau(){
        return niveau;
    }

    //Set
    void Circuit::setNiveau(int niveau){
        this->niveau = niveau;
    }

    void Circuit::creation_Niveau(int Selection){
        if(Selection == 1){
            choix = UN;
        }
        else if(Selection == 2){
            choix = DEUX;
        }
        else{
            choix = TROI;
        }
    }

    void Niveau_un(){
        bool bitmap_Niveau_un[Hauteur_matrice][Largeur_matrice] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0,
            1, 1, 0,
            1, 1, 0,
            1, 1, 0,
            1, 1, 0,
            1, 1, 0,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
            1, 1,
         };
    }
    void loop(){
        switch (choix){
            case UN:

            break;

            case DEUX:

            break;

            case TROI:

            break;
        };
    }