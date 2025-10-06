#include <Arduino.h>
#include <LibRobus.h>
#include "moteur.h"
#include "capteur.h"
#include "pid.h"

// ===== Réglages déplacement =====
static const float VITESSE_BASE = 0.20f;    // vitesse d'avance PID (0..1)
static const long  DISTANCE_TICKS_MAX = 25000; // distance max si pas d'obstacle

// ===== Réaction aux obstacles =====
static const float VITESSE_TOURNE = 0.10f;  // vitesse pour pivoter
static const int   TOURNE_MS = 650;         // durée d'un virage (~90°) à calibrer
static const float VITESSE_RECULE = 0.10f;  // vitesse pour reculer
static const int   RECULE_MS = 300;         // durée de recul avant de tourner quand c'est bloqué en face

// Anti-glitch capteurs (valider N lectures d'affilée)
static const int DETECT_N = 3;
static const int DETECT_DT_MS = 5;

// ---- Helpers ----
static void reculer_ms(int ms) {
  MOTOR_SetSpeed(LEFT,  -VITESSE_RECULE);
  MOTOR_SetSpeed(RIGHT, -VITESSE_RECULE);
  delay(ms);
  MOTEUR_Stop();
}

static int Find_Mur_stable() {
  int count = 0;
  int last = Find_Mur();
  for (int i = 0; i < DETECT_N; i++) {
    int m = Find_Mur();
    if (m == last) count++; else { count = 0; last = m; }
    delay(DETECT_DT_MS);
  }
  return (count >= DETECT_N) ? last : 0;
}

void setup() {
  Serial.begin(9600);
  MOTEUR_Init();
  CAPTEUR_Init();
  Serial.println("Robot PID + evitemment actif (LOW = detecte)");
  delay(300);
}

void loop() {
  // 1) Avance droit avec PID jusqu'à distance max OU détection IR
  Serial.println("➡️ Avance PID");
  PID_AvanceDroit(VITESSE_BASE, DISTANCE_TICKS_MAX);

  // 2) Détection stable avant réaction
  int mur = Find_Mur_stable();
  Serial.print("Mur detecte = "); Serial.println(mur);

  if (mur == 0) {
    // Si on est sorti par distance (pas d'obstacle), on peut repartir
    // (ou faire autre chose). Ici: petite pause et on repart.
    MOTEUR_Stop();
    delay(150);
    return; // boucle et repart
  }

  // 3) Réaction selon le côté détecté
  if (mur == 1) {
    // Obstacle à GAUCHE -> pivote à DROITE
    Serial.println("Obstacle gauche -> tourne DROITE");
    MOTEUR_TourneDroite(VITESSE_TOURNE);
    delay(TOURNE_MS);
    MOTEUR_Stop();
  } else if (mur == 2) {
    // Obstacle à DROITE -> pivote à GAUCHE
    Serial.println("Obstacle droite -> tourne GAUCHE");
    MOTEUR_TourneGauche(VITESSE_TOURNE);
    delay(TOURNE_MS);
    MOTEUR_Stop();
  } else {
    // mur == 3 : les deux -> recule un peu et tourne (U-turn léger)
    Serial.println("Mur en face -> recule + demi-tour");
    reculer_ms(RECULE_MS);
    MOTEUR_TourneGauche(VITESSE_TOURNE);
    delay(TOURNE_MS + 250); // un peu plus long pour demi-tour
    MOTEUR_Stop();
  }

  // 4) Petite pause pour laisser retomber les capteurs et repartir
  delay(100);
  // -> la loop() recommence: avance PID à nouveau
}
