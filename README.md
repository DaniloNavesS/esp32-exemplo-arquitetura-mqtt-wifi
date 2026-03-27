## Estrutura de Diretórios e Modularização

O projeto adota uma arquitetura modular para o framework **ESP-IDF**, separando as responsabilidades de rede, comunicação e lógica de aplicação em diretórios distintos.

### Hierarquia do Projeto

```text
PI2-CAR/
├── main/                   # Arquivos
│   ├── include/            # Bibliotecas
│       ├── wifi_modulo/        # Módulo de conectividade Wi-Fi
│       │       ├── wifi_modulo.c   # Implementação da conexão Wi-Fi
│       │       └── wifi_modulo.h   # Protótipos e definições de rede
│       ├── mqtt_modulo/        # Módulo de protocolo MQTT
│       │   ├── mqtt_modulo.c   # Lógica de publicação/inscrição
│       │   └── mqtt_modulo.h   # Definições do MQTT
|       ├── sensor_generic/        # Módulo de protocolo MQTT
│       │   ├── sensor_generic.c   # Lógica especifica do sensor
│       │   └── sensor_generic.h   # Definição do sensor
|       ├── atuador_generic/        # Módulo de protocolo MQTT
│       │   ├── atuador_generic.c   # Lógica especifica do atuador
│       │   └── atuador_generic.h   # Definição do atuador
│   ├── main.c              # Funções de inicio e tasks
│   ├── config.h            # Configurações globais do sistema
│   ├── pin_config.h        # Disposição dos pinos utilizados
│   └── CMakeLists.txt      # Instruções de compilação do componente main
├── build/                  # Arquivos binários gerados na compilação
```