#include <Arduino.h>
#include <LibRobus.h>
#include "moteur.h"
#include "capteur.h"
#include "pid.h"

static const float VITESSE_BASE   = 0.50f;
static const long  DISTANCE_TICKS = 1800000;


void setup() {
  Serial.begin(9600);
  MOTEUR_Init();
  CAPTEUR_Init();
  delay(300);
}

void loop() {
  PID_AvanceDroit(VITESSE_BASE, DISTANCE_TICKS);
  while (true) { delay(1000); };

}
