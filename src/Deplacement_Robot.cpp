// Deplacement_Robot.cpp - Christopher Plantevin

// les includes (headers et librairies)

#include "Deplacement_Robot.h"
#include "Commandes_Robot.h"
#include <Arduino.h>
#include "LibRobus.h"

// Variables internes

    // variables pour le suiveur de ligne
        int a;
        int b;
        int c;

    // variables pour les correction du suiveur de ligne
        float force_correction;

    // Variables pour le deplacement du robot

        int acceleration_pi;
        int vitesse_pi;

    // Variables pour la recherche de la ligne
        int Direction_Correction;


// Fonctions
    // Section | Deplacement du Robot

    void moteurs(float gauche,float droite){
        MOTOR_SetSpeed(LEFT, gauche);
        MOTOR_SetSpeed(RIGHT, droite);

    }

    void Avancer(int distance){

        //Direction_Correction = 0;
        
        delay(10);

        Serial.print("A: ");
        Serial.println(a);
        Serial.print("B: ");
        Serial.println(b);
        Serial.print("C: ");
        Serial.println(c);
     
        Serial.println("");

        

        moteurs(VITESSE_AVANT, VITESSE_AVANT);
        
    }

    void Reculer(int distance){
    
        moteurs( -VITESSE_ARRIERE, -VITESSE_ARRIERE);
    
    }

    void Arret(){

        int temps;

        vitesse_pi = VITESSE_AVANT;
    
        moteurs(vitesse_pi, vitesse_pi);

        acceleration_pi = vitesse_pi / TEMPS_ARRET;

        temps = TEMPS_ARRET;

        while(temps != 0){
        
            delay(TEMPS_INTERVALLE / 1000);
            vitesse_pi = vitesse_pi - acceleration_pi;

            moteurs(vitesse_pi, vitesse_pi);

            temps = temps - TEMPS_INTERVALLE;

        }
        
        moteurs(0, 0);
    
    }

    void Tourner(int angle){


    }


// Section | Recherche de la ligne


    void Recherche_Ligne(){
        if (Direction_Correction == 1){ // Correction à Gauche

        moteurs(VITESSE_CORRECTION_DROITE*force_correction, 0.05);

        } else if(Direction_Correction == 2) { // Correction à Droite

        moteurs(0.05, VITESSE_CORRECTION_DROITE*force_correction);

        } 
    }

// Section | Suiveur de Ligne
    
    void Correction_Droite(){

        Direction_Correction = 2;
        
        delay(10);
        
        if (b > c){
            force_correction = float( 1 - ( c / ( b )));
        }
        if (force_correction < 0.01){
            Avancer(10);
        }


        Serial.print("A: ");
        Serial.println(a);
        Serial.print("B: ");
        Serial.println(b);
        Serial.print("C: ");
        Serial.println(c);
        Serial.println("");

        Serial.print("correction: ");
        Serial.println(force_correction);
        Serial.print("");

        moteurs(0.05, VITESSE_CORRECTION_DROITE*force_correction);

    }

    void Correction_Gauche(){

        Direction_Correction = 1;

        delay(10);
        
        if (b > c){
            force_correction = float( 1 - ( c / ( b )));
        }
        if (force_correction < 0.01){
            Avancer(10);
        }        

        Serial.print("A: ");
        Serial.println(a);
        Serial.print("B: ");
        Serial.println(b);
        Serial.print("C: ");
        Serial.println(c);
        Serial.println("");

        Serial.print("correction: ");
        Serial.println(force_correction);
        Serial.print("");
        
        moteurs( VITESSE_CORRECTION_DROITE*force_correction, 0.05);
    
    }   

    void SuiveurLigne(){

        a = analogRead(PIN_A);
        b = analogRead(PIN_B);
        c = analogRead(PIN_C);

        if(Etat_Attente){
            Arret();
        } else {
            if(b > SEUIL_LIGNE){
                Avancer(1);
            } else { 
                if ( (a > c ) && ( a > SEUIL_DETECTION_D) ){
                    Correction_Droite();  
                } else if ( ( c > a ) && ( c > SEUIL_DETECTION_G) ){
                    Correction_Gauche();
                } else {
                // rechercher la ligne de suivit au cas le robot perd la ligne 
                    Recherche_Ligne();
                }
            }
        }
    }