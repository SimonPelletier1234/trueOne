#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <Arduino.h>

// Broches des capteurs infrarouges
#define PIN_IR_ROUGE  48
#define PIN_IR_VERTE  49

// Type de capteur : LOW = détection active
#define IR_ACTIVE_LOW  1

// Fonctions
void CAPTEUR_Init(void);
bool CAPTEUR_GaucheDetecte(void);
bool CAPTEUR_DroiteDetecte(void);
int  Find_Mur(void);

#endif
