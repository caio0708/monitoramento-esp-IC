# Funcionalidades
Medição da temperatura ambiente através do sensor NTC.
Controle de um LED utilizando um reed switch para indicar o estado do dispositivo (ligado/desligado).
Monitoramento em tempo real do nível do potenciômetro.
Interface gráfica para visualizar as informações na tela OLED.

# Componentes Utilizados
ESP32 (ou compatível) com a Arduino IDE.
Sensor NTC (Termistor) para medir a temperatura ambiente.
Reed Switch para controlar o LED indicador.
Potenciômetro para ajustar e disponibilizar o nível.

# Esquema de Conexões
Aqui estão as conexões necessárias para montar o circuito:

Sensor NTC:

Conecte o pino positivo do sensor ao pino 3V3 do ESP32 através de um resistor (10kΩ).
Conecte o pino negativo do sensor ao pino GND do ESP32.
Conecte o pino sinal do sensor ao pino 36 do ESP32.

Reed Switch:

Conecte um pino do reed switch ao pino digital D2 do ESP32.
Conecte o outro pino do reed switch ao pino GND do ESP32 através de um resistor (10kΩ).

Potenciômetro:

Conecte o pino central (wiper) do potenciômetro ao pino analógico 39 do ESP32.
Conecte um dos pinos extremos do potenciômetro ao pino 3V3 do ESP32.
Conecte o outro pino extremo do potenciômetro ao pino GND do ESP32.

LED:

Conecte o pino positivo do LED ao pino digital D0 do ESP32.
Conecte o pino negativo do LED ao pino GND do ESP32.

# Instalação e Uso
Clone este repositório em sua máquina local.
Abra o projeto na Arduino IDE.
Conecte o ESP32 ao computador via USB e selecione o modelo correto nas configurações da Arduino IDE.
Faça as conexões de acordo com o esquema acima.
Carregue o código para o ESP32 e verifique o monitor serial da Arduino IDE para ver as leituras de temperatura e o estado do LED.

# Contribuições
Contribuições são bem-vindas! Se você encontrou algum bug, tem ideias de melhorias ou novos recursos, sinta-se à vontade para abrir uma issue ou enviar um pull request.
