// Commandes_Robot.cpp - Christopher Plantevin

// les includes (headers et librairies)

#include "Commandes_Robot.h"
#include "Deplacement_Robot.h"
#include <Arduino.h>
#include "LibRobus.h"

// Variables internes

bool Etat_Attente;
bool Fait;

// Fonctions

    // Section | Commandes du robot

    void Initialisation(){
        Etat_Attente = false;
        Fait = false;
        
        pinMode(PIN_DEL_TEST, OUTPUT);
        digitalWrite(PIN_DEL_TEST, HIGH);

        ENCODER_Reset(LEFT);
        ENCODER_Reset(RIGHT);

        Arret();

    }

    void Reprendre(){
        Etat_Attente = false;
        digitalWrite(PIN_DEL_TEST, LOW);
    }

    void Pause(){

        Etat_Attente = true;
        digitalWrite(PIN_DEL_TEST, HIGH);

        Arret();
    }

    void Attente_depart(){

        delay(10);

        Serial.println("Attente d'activation...");
        while (!ROBUS_IsBumper(AVANT)) {
            delay(20);
            SOFT_TIMER_Update();
            Serial.begin(9600);
        }
        Serial.println("Allons-y!");
        Etat_Attente = false;
        
    }


// Section | Verification des commandes du robot

    void Verif_Bouton_Principal(){
        if (ROBUS_IsBumper(AVANT) && !Etat_Attente && !Fait){
            Fait = true;
            Pause();
        }
        if (ROBUS_IsBumper(AVANT) && Etat_Attente && !Fait){
            Fait = true;
            Reprendre();
        }
        if (!ROBUS_IsBumper(AVANT)){
            Fait = false;
        }
    }

    void Verif_Commandes(){

        Verif_Bouton_Principal();

    }
