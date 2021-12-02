/********************************************************
*  OBJETOS INTELIGENTES CONECTADOS
*  ESP32 - Utilizando  Sensor MQ-2 e MQTT
*  02/12/2021 Entrega final do projeto da disciplina
*/

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "***************";
const char* password =  "***************";
const char* mqttServer = "mqtt.eclipseprojects.io";
const int mqttPort = 1883;
const char* mqttUser = "abcdefg";
const char* mqttPassword = "123456";

WiFiClient espClient;
PubSubClient client(espClient);

//int contador = 1;
char mensagem[30];

// Sensor  ----------------------------------------------
#define MQ2pin (36)

int sensorValue;  //variable to store sensor value

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("Ligando sensor de gas!");
  pinMode(MQ2pin, INPUT);
  randomSeed(analogRead(0));
  delay(20000); // allow the MQ-2 to warm up

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
}

void loop()
{
  // Valor do sensor
  sensorValue = analogRead(MQ2pin); // read analog input pin 0
  
  //Faz a conexao com o broker MQTT
  reconectabroker();
  sprintf(mensagem, "MQTT ESP32 - Valor do sensor %d", sensorValue);
  Serial.print("Mensagem enviada: ");
  Serial.println(mensagem);

  //Envia a mensagem ao broker
  client.publish("ArduinoeCia", mensagem);
  Serial.println("Mensagem enviada com sucesso...");
  
  //Incrementa o contador
  //contador++;
  
  //Aguarda 3 segundos para enviar uma nova mensagem
  delay(3000);
}

void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
