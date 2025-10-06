#pragma once
#include <Arduino.h>

// Broches capteurs IR (Mega 2560)
#define PIN_IR_GAUCHE 48
#define PIN_IR_DROITE 49

// Actif-bas : LOW = obstacle détecté
#define IR_ACTIVE_LOW 1

void CAPTEUR_Init();
bool CAPTEUR_GaucheDetecte();
bool CAPTEUR_DroiteDetecte();
int  Find_Mur();  // 0 rien, 1 gauche, 2 droite, 3 les deux
