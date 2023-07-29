#include <WiFi.h>
#include <PubSubClient.h>

// Pins
#define THERMISTORPIN 36
#define ledPin 0
#define digitalPin 2

// MQTT settings
const char* mqtt_server = "your_mqtt_server_address";
const int mqtt_port = 1883;
const char* mqtt_username = ""; // Leave empty if not required
const char* mqtt_password = ""; // Leave empty if not required
const char* mqtt_topic = "esp32/output";

// WiFi settings
const char* ssid = "your_wifi_network_name";
const char* password = "your_wifi_password";

WiFiClient espClient; 
PubSubClient client(espClient);

// Variables
unsigned long now;
unsigned long now2;
unsigned long now3;
char txt[10];
char txt2[10];
char msg[10];
char pot[10];

void setup() {
  Serial.begin(115200); // Initialize the serial port
  pinMode(ledPin, OUTPUT);
  pinMode(THERMISTORPIN, INPUT);

  now = millis(); 

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  temperatura();
  reed();
  potenciometro();
}

void reed() {
  int digitalVal = digitalRead(digitalPin);  

  if (millis() >= now + 500) {
    now = millis();

    if (digitalVal == LOW) {
      digitalWrite(ledPin, LOW);
      Serial.println("LED is off.");

      // Publish a string to the MQTT topic
      String off = "OFF";
      Serial.println("Published message: " + off);
      snprintf(txt, 10 , "LED: %i", digitalVal);
      client.publish(mqtt_topic, txt);
    } else {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED is on.");

      // Publish a string to the MQTT topic
      String on = "ON";
      Serial.println("Published message: " + on);
      snprintf(txt2, 10 , "LED: %i", digitalVal);
      client.publish(mqtt_topic, txt2);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      client.publish(mqtt_topic, "MQTT Server is Connected");
      // ... and resubscribe
      client.subscribe("esp32/input");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void temperatura() {
  const int numSamples = 100; // Number of samples for averaging
  const float resistorValue = 10000.0; // Resistance value of the NTC thermistor at 25°C
  const float thermistorNominal = 10000.0; // Thermistor nominal resistance at 25°C
  const float bValue = 4300 ; // B-value of the thermistor

  if (millis() >= now2 + 2000) {
    now2 = millis();
    int i;
    float sum = 0.0;

    // Collect numSamples samples from the thermistor
    for (i = 0; i < numSamples; i++) {
      sum += analogRead(THERMISTORPIN);
      delay(10);
    }

    // Calculate the average value of the samples
    float average = sum / numSamples;

    // Convert the average analog value to resistance
    float resistance = resistorValue / (4096.0 / average - 1.0);

    // Calculate the temperature using the Steinhart-Hart equation
    float steinhart;
    steinhart = resistance / thermistorNominal;   // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= bValue;                         // 1/B * ln(R/Ro)
    steinhart += 1.0 / (25.0 + 273.15);          // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    float temperature = steinhart - 273.15;      // Convert to Celsius

    // Display the temperature value on the Serial monitor 
    Serial.print("Temperature: ");
    Serial.print(temperature, 2); // Print temperature value with 2 decimal places
    Serial.println(" C");

    // Publish the temperature value to the MQTT topic
    sprintf(msg, "%.2f", temperature); 
    client.publish("esp32/temp", msg);
  }
  delay(350);
}



void potenciometro() {
  if (millis() >= now3 + 1500) {
    int sensorValue = analogRead(39);
    Serial.print("Potentiometer: ");
    Serial.print(sensorValue);
    Serial.print("\n");

    // Publish the potentiometer value to the MQTT topic
    sprintf(pot, "%i", sensorValue); 
    client.publish("esp32/pot", pot);
  }
}
