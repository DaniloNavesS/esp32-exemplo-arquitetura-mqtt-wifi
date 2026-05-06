#include "hc_sr04_modulo.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void hc_sr04_init(hc_sr04_t *sensor) {
    gpio_reset_pin(sensor->pin_trig);
    gpio_set_direction(sensor->pin_trig, GPIO_MODE_OUTPUT);
    gpio_set_level(sensor->pin_trig, 0);

    gpio_reset_pin(sensor->pin_echo);
    gpio_set_direction(sensor->pin_echo, GPIO_MODE_INPUT);
}

float hc_sr04_read_distance(hc_sr04_t *sensor) {
    gpio_set_level(sensor->pin_trig, 0);
    ets_delay_us(2);
    gpio_set_level(sensor->pin_trig, 1);
    ets_delay_us(10);
    gpio_set_level(sensor->pin_trig, 0);

    uint32_t start_time = esp_timer_get_time();
    while (gpio_get_level(sensor->pin_echo) == 0) {
        if (esp_timer_get_time() - start_time > 1000000) { // 1 second timeout
            return -1.0;
        }
    }
    
    start_time = esp_timer_get_time();
    while (gpio_get_level(sensor->pin_echo) == 1) {
        if (esp_timer_get_time() - start_time > 1000000) { // 1 second timeout
            return -1.0;
        }
    }
    uint32_t end_time = esp_timer_get_time();
    
    uint32_t duration = end_time - start_time;
    // Speed of sound is 343 m/s = 0.0343 cm/us
    // Distance = (duration * 0.0343) / 2
    float distance = duration / 58.0;
    
    return distance;
}
