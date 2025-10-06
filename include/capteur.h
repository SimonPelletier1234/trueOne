#pragma once
#include <Arduino.h>

// === Broches capteurs (ajuste si nécessaire) ===
#define PIN_CAPTEUR_ROUGE 2
#define PIN_CAPTEUR_VERT  3

// Init des broches
void CAPTEUR_Init();

// Renvoie true si le capteur voit l'état "actif"
bool CAPTEUR_Rouge();
bool CAPTEUR_Vert();

// 0: aucun, 1: rouge seul, 2: vert seul, 3: les deux
int Find_Mur();
