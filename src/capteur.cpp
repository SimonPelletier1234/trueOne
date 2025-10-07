#include "capteur.h"

void CAPTEUR_Init(void) {
  // Active les résistances internes et prépare les entrées
  pinMode(PIN_IR_VERTE, INPUT_PULLUP);
  pinMode(PIN_IR_ROUGE, INPUT_PULLUP);
}

// Retourne vrai si le capteur branché sur "broche" détecte un obstacle
static inline bool capteurDetecte(uint8_t broche) {
  int lecture = digitalRead(broche);
#if IR_ACTIVE_LOW
  return (lecture == LOW);
#else
  return (lecture == HIGH);
#endif
}

bool CAPTEUR_GaucheDetecte(void) {
  return capteurDetecte(PIN_IR_VERTE);
}

bool CAPTEUR_DroiteDetecte(void) {
  return capteurDetecte(PIN_IR_ROUGE);
}

// Retourne un code selon l’état des capteurs
// 0 = rien, 1 = gauche, 2 = droite, 3 = les deux
int Find_Mur(void) {
  bool gauche = CAPTEUR_GaucheDetecte();
  bool droite = CAPTEUR_DroiteDetecte();

  if (gauche && droite) return 3;
  if (gauche) return 1;
  if (droite) return 2;
  return 0;
}
