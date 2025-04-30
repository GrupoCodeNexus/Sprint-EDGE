// OBS: Esse foi o código utilizado na simulação, lembrando que
//foram feitas alterações pois dentro da plataforma do Wokiwi não existe 
//o RFC522, logo fizemos uma adaptação para o funcionamento do projeto 


#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configurações Wi-Fi e MQTT
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* BROKER_MQTT = "130.131.16.56";
const int BROKER_PORT = 1883;

const char* TOPICO_SUBSCRIBE = "/TEF/NEXUScode2/cmd";
const char* TOPICO_PUBLISH_1 = "/TEF/NEXUScode2/attrs/a";
const char* TOPICO_PUBLISH_2 = "/TEF/NEXUScode2/attrs/n";
const char* ID_MQTT = "fiware_code";

// Pinos
#define BUZZER_PIN   14
#define GREEN_LED    26
#define RED_LED      25
#define MOTOR_PIN    12

WiFiClient espClient;
PubSubClient MQTT(espClient);
Servo motor;
LiquidCrystal_I2C lcd(0x27, 16, 2);

String ultimoCartao = "";
bool portaAberta = false;

// --- Funções de Conexão ---

void initWiFi() {
  delay(10);
  Serial.println("Conectando-se ao Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi conectado. IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // Callback reservado para comandos futuros via MQTT
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado.");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.print("Erro: ");
      Serial.print(MQTT.state());
      Serial.println(" tentando novamente em 2 segundos...");
      delay(2000);
    }
  }
}

// --- Funções de Interface ---

void mostrarTelaDeEspera() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aguardando...");
  Serial.println("Passe o cartao (formato: XX XX XX XX XX XX XX):");
}

void mostrarTelaAguardandoFechamento(String id) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Passe o mesmo ID");
  Serial.println("Aguardando o mesmo cartao para fechar.");
}

// --- Função Setup ---

void setup() {
  Serial.begin(115200);

  initWiFi();         // Primeiro Wi-Fi
  initMQTT();         // Depois MQTT

  // Inicialização dos pinos
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // Inicialização do motor
  motor.setPeriodHertz(50);
  motor.attach(MOTOR_PIN, 500, 2400);
  motor.write(90); // porta fechada

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Code Nexus");

  delay(3000);
  mostrarTelaDeEspera();
}

// --- Loop Principal ---

void loop() {
  if (!MQTT.connected()) reconnectMQTT();
  MQTT.loop();

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Verificando...");
    delay(1000);

    if (isValidFormat(input)) {
      if (portaAberta && input.equals(ultimoCartao)) {
        Serial.println("Fechando a porta...");
        motor.write(90);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Porta fechada");
        delay(2000);
        mostrarTelaDeEspera();
        portaAberta = false;
        ultimoCartao = "";
      } else if (!portaAberta && input.equals("04 3B 1D 5E 72 3D 80")) {
        accessGranted(input);
      } else {
        accessDenied(input);
      }
    } else {
      Serial.println("Formato inválido.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Formato invalido");
      lcd.setCursor(0, 1);
      lcd.print("XX XX XX XX XX XX XX");
      delay(3000);
      mostrarTelaDeEspera();
    }
  }
}

// --- Funções de Validação e Ação ---

bool isValidFormat(String input) {
  if (input.length() == 20) {
    for (int i = 0; i < input.length(); i++) {
      if ((i + 1) % 3 == 0) {
        if (input.charAt(i) != ' ') return false;
      } else {
        if (!isHexadecimalDigit(input.charAt(i))) return false;
      }
    }
    return true;
  }
  return false;
}

void accessGranted(String input) {
  Serial.println("Access granted!");
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  motor.write(0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acesso permitido");

  delay(2000);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);

  portaAberta = true;
  ultimoCartao = input;

  // Publica o ID aceito no tópico apropriado
  MQTT.publish(TOPICO_PUBLISH_1, input.c_str());
  mostrarTelaAguardandoFechamento(input);
}

void accessDenied(String input) {
  Serial.println("Access denied!");
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(RED_LED, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acesso negado");
  lcd.setCursor(0, 1);
  lcd.print("UID invalido");

  delay(3000);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED, LOW);

  // Envia o ID inválido ao tópico de negado
  MQTT.publish(TOPICO_PUBLISH_2, input.c_str());
  mostrarTelaDeEspera();
}
