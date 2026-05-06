#ifndef HC_SR04_MODULO_H
#define HC_SR04_MODULO_H

typedef struct {
    int pin_trig;
    int pin_echo;
} hc_sr04_t;

void hc_sr04_init(hc_sr04_t *sensor);
float hc_sr04_read_distance(hc_sr04_t *sensor);

#endif
