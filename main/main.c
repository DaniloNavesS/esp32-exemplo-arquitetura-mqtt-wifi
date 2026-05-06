#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "hc_sr04_modulo.h"
#include "mh_sensor_modulo.h"
#include "sg90_modulo.h"
#include "wifi_modulo.h"
#include "mqtt_modulo.h"
#include "config.h"

#define TAG "SYSTEM"

// Semáforos e Mutexes
SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void task_conexao_manager(void * params)
{
    while(true) {
        if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)) {
            ESP_LOGI(TAG, "Wi-Fi OK. Iniciando MQTT...");
            mqtt_start();
        }
    }
}

// void app_main(void)
// {
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         ret = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(ret);

//     conexaoWifiSemaphore = xSemaphoreCreateBinary();
//     conexaoMQTTSemaphore = xSemaphoreCreateBinary();

//     wifi_start();
//     hc_sr04_init();
//     mh_sensor_init();

//     vTaskDelay(pdMS_TO_TICKS(3000));

//     while(1) {
//         // 2. Lê se o sensor foi acionado ou não
//         bool sensor_acionado = mh_sensor_read_digital();
        
//         // Em muitos módulos MH (como o de obstáculo infravermelho), 
//         // ele retorna 0 (falso) quando detecta algo e 1 (verdadeiro) quando não detecta.
//         if (sensor_acionado) {
//             ESP_LOGI("SENSOR", "Sinal em ALTO (1)");
//         } else {
//             ESP_LOGI("SENSOR", "Sinal em BAIXO (0) - Possível detecção!");
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     // while (1) {
//     //     float distance = hc_sr04_read_distance();
//     //     if (distance > 0) {
//     //         ESP_LOGI(TAG, "Distancia: %.2f cm", distance);
//     //     } else {
//     //         ESP_LOGE(TAG, "Erro ao ler distancia");
//     //     }
//     //     vTaskDelay(pdMS_TO_TICKS(1000));
//     // }

//     // Task de Conexão
//     xTaskCreate(task_conexao_manager, "ConnManager", 4096, NULL, 5, NULL);
// }


// Cria as instâncias dos dois sensores
hc_sr04_t sensor1 = { .pin_trig = PIN_HC_SR04_1_TRIG, .pin_echo = PIN_HC_SR04_1_ECHO };
hc_sr04_t sensor2 = { .pin_trig = PIN_HC_SR04_2_TRIG, .pin_echo = PIN_HC_SR04_2_ECHO };

void app_main(void) {
    // Inicializa o motor e os dois sensores
    sg90_init();
    hc_sr04_init(&sensor1);
    hc_sr04_init(&sensor2);
    
    while(1) {
        float dist1 = hc_sr04_read_distance(&sensor1);
        float dist2 = hc_sr04_read_distance(&sensor2);
        
        // Verifica se houve erro de leitura
        if (dist1 > 0 && dist2 > 0) {
            ESP_LOGI("APP", "Distancia 1: %.2f cm | Distancia 2: %.2f cm", dist1, dist2);

            // Lógica de Alinhamento: Distância <= 15cm em ambos E diferença <= 3cm
            if (dist1 <= 15.0 && dist2 <= 15.0 && fabs(dist1 - dist2) <= 3.0) {
                ESP_LOGI("APP", "Alinhado e perto! Ativando o motor.");
                // Ativa o motor (como ele é de 360, 0 o faz girar)
                sg90_set_angle(0); 
            } else {
                // Para o motor se estiver desalinhado
                sg90_set_angle(90); 
            }
        } else {
            ESP_LOGE("APP", "Erro na leitura de um dos sensores.");
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Lê 10 vezes por segundo para resposta rápida
    }
}
