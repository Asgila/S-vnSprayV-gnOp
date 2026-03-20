#include <WiFi.h>
#include <PubSubClient.h>


// --- WiFi Credentials ---
const char* ssid = "OnePlus 8 Pro";
const char* password = "123456789";

// --- Maqiatto Broker Settings ---
const char* mqtt_server = "maqiatto.com";
const int mqtt_port = 1883;

// Your Maqiatto login details
const char* mqtt_user = "asger.frimor@gmail.com"; 
const char* mqtt_password = "123456789";

// --- Topics ---
// CRITICAL: Maqiatto requires your email to be the prefix for ALL topics
const char* topic_switch = "asger.frimor@gmail.com/esp32/switch";
const char* topic_slider = "asger.frimor@gmail.com/esp32/slider";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        messageTemp += (char)payload[i];
    }
    Serial.println(messageTemp);

    // Handle the message based on the topic
    if (String(topic) == topic_switch) {
        if (messageTemp == "ON") {
            GreenOn();
        } else if (messageTemp == "OFF") {
            GreenOff();
        }
    } else if (String(topic) == topic_slider) {
        int sliderValue = messageTemp.toInt();
        // Use sliderValue to control something, e.g., brightness
        Serial.print("Slider value: ");
        Serial.println(sliderValue);
    }
}


void mqqt_setup() {
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqtt_callback);
}

void mqtt_reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting Maqiatto MQTT connection...");
          
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        
        // Attempt to connect using your Maqiatto email and password
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("connected!");
            
            // CRITICAL: Subscribe to your topics so Maqiatto sends you the data!
            client.subscribe(topic_switch);
            client.subscribe(topic_slider);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}