#ifndef PID_H
#define PID_H

#include <Arduino.h>

// Fait avancer droit sur "distance_ticks" (encodeurs), avec correction PID.
// vitesse_base : -1.0 à 1.0 (négatif = recul)
void PID_AvanceDroit(float vitesse_base, long distance_ticks);

#endif
