#include <Arduino.h>
#include "capteur.h"

void CAPTEUR_Init() {
  pinMode(PIN_CAPTEUR_ROUGE, INPUT); // ou INPUT_PULLUP selon ton module
  pinMode(PIN_CAPTEUR_VERT,  INPUT); // idem
}

// Si tes capteurs sont "actif-bas", inverse la logique (== LOW)
bool CAPTEUR_Rouge() { return digitalRead(PIN_CAPTEUR_ROUGE) == HIGH; }
bool CAPTEUR_Vert()  { return digitalRead(PIN_CAPTEUR_VERT)  == HIGH; }

int Find_Mur() {
  bool rouge = CAPTEUR_Rouge();
  bool vert  = CAPTEUR_Vert();

  if (rouge && !vert) return 1;   // rouge seul
  if (!rouge && vert) return 2;   // vert seul
  if (!rouge && !vert) return 0;  // aucun
  return 3;                       // les deux
}
