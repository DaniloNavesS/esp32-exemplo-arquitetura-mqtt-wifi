#ifndef MH_SENSOR_MODULO_H
#define MH_SENSOR_MODULO_H

#include <stdbool.h>

// Inicializa o pino digital do MH-Sensor
void mh_sensor_init(void);

// Lê o valor digital do sensor (D0)
bool mh_sensor_read_digital(void);

#endif
