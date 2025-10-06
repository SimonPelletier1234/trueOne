#include "capteur.h"

void CAPTEUR_Init() {
  // Pull-up interne -> HIGH au repos, LOW quand le module tire à la masse
  pinMode(PIN_IR_GAUCHE, INPUT_PULLUP);
  pinMode(PIN_IR_DROITE, INPUT_PULLUP);
}

static inline bool pinDetecte(uint8_t pin) {
  int v = digitalRead(pin);
#if IR_ACTIVE_LOW
  return v == LOW;   // LOW = détection (ton cas)
#else
  return v == HIGH;  // (non utilisé ici)
#endif
}

bool CAPTEUR_GaucheDetecte() { return pinDetecte(PIN_IR_GAUCHE); }
bool CAPTEUR_DroiteDetecte() { return pinDetecte(PIN_IR_DROITE); }

int Find_Mur() {
  bool g = CAPTEUR_GaucheDetecte();
  bool d = CAPTEUR_DroiteDetecte();
  if (g && d) return 3;
  if (g && !d) return 1;
  if (!g && d) return 2;
  return 0;
}
