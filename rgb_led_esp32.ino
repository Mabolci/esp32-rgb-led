#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const int redPin = 32;
const int greenPin = 22;   
const int bluePin = 23;    

const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;

const int resolution = 8;

int brightness = 0;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_server = "";
short int mqtt_port = 0;

const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "";

const char* SSID = "";
const char* password = "";

const char* device_name = "";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // deserialize json
  StaticJsonDocument<244> doc;
  deserializeJson(doc, payload, length);

  // read values
  short unsigned int idx = doc["idx"];
  if(idx == 8) {
    unsigned short int nvalue = doc["nvalue"];
    if(nvalue != 0) {
    double brightness = doc["svalue1"];
    unsigned short int r = doc["Color"]["r"];
    unsigned short int g = doc["Color"]["g"];
    unsigned short int b = doc["Color"]["b"];
    
    brightness = brightness/100;
    r = r*brightness;
    g = g*brightness;
    b = b*brightness;

    ledcWrite(redChannel, r);
    ledcWrite(greenChannel, g);
    ledcWrite(blueChannel, b);


    
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    }
    else {
      ledcWrite(redChannel, 0);
      ledcWrite(greenChannel, 0);
      ledcWrite(blueChannel, 0);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(device_name, mqtt_username, mqtt_password)) {
      Serial.print("connected with ");
      Serial.print(mqtt_username);
      Serial.println("!");
      // Subscribe
      Serial.println("Subscribing to topics:");
      Serial.println(mqtt_topic);
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // initialize the serial port:
  Serial.begin(115200);

  // initialize led strip
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  ledcAttachPin(redPin, redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin, blueChannel);
  
  // wifi connection starts
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);


  WiFi.begin(SSID, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // wifi connection ends

  // mqtt starts
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}




void loop() {
  // put your main code here, to run repeatedly:
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

