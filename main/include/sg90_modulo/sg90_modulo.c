#include "sg90_modulo.h"
#include "config.h"
#include "driver/ledc.h"

// Configuracoes do LEDC para o Servo SG90
#define SERVO_LEDC_TIMER       LEDC_TIMER_0
#define SERVO_LEDC_MODE        LEDC_LOW_SPEED_MODE
#define SERVO_LEDC_CHANNEL     LEDC_CHANNEL_0
#define SERVO_LEDC_DUTY_RES    LEDC_TIMER_13_BIT // 8192 resolucao
#define SERVO_LEDC_FREQUENCY   50 // 50 Hz, 20ms por periodo

// Largura de pulso em microssegundos
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimo para 0 graus
#define SERVO_MAX_PULSEWIDTH_US 2500 // Maximo para 180 graus
#define SERVO_MAX_DEGREE        180

void sg90_init(void) {
    // Configura o timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = SERVO_LEDC_MODE,
        .duty_resolution  = SERVO_LEDC_DUTY_RES,
        .timer_num        = SERVO_LEDC_TIMER,
        .freq_hz          = SERVO_LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configura o canal
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = SERVO_LEDC_MODE,
        .channel        = SERVO_LEDC_CHANNEL,
        .timer_sel      = SERVO_LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = PIN_SG90_PWM,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}

void sg90_set_angle(int angle) {
    if (angle < 0) angle = 0;
    if (angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;

    uint32_t pulsewidth = SERVO_MIN_PULSEWIDTH_US + 
        (((SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) * angle) / SERVO_MAX_DEGREE);

    // Calcula o duty baseado na resolucao de 13 bits (8192) e 20ms de periodo (20000us)
    uint32_t duty = (pulsewidth * 8192) / 20000;

    ledc_set_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL, duty);
    ledc_update_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL);
}
