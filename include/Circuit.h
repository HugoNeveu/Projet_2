#pragma once
#include <Arduino.h>

class Circuit{
    private:
    int niveau;

    public:
    //Construcleur
    Circuit();
    Circuit(int niveau);

    //Get
    int getNiveau();

    //Set
    void setNiveau(int niveau);

    void creation_Niveau(int Selection);
};