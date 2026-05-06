#include "mh_sensor_modulo.h"
#include "config.h"
#include "driver/gpio.h"

void mh_sensor_init(void) {
    // Configura o pino como entrada
    gpio_reset_pin(PIN_MH_SENSOR_DO);
    gpio_set_direction(PIN_MH_SENSOR_DO, GPIO_MODE_INPUT);
    
}

bool mh_sensor_read_digital(void) {
    // Lê o estado lógico do pino digital
    int state = gpio_get_level(PIN_MH_SENSOR_DO);
    return (state == 1);
}
