// Define the built-in LED pin (usually pin 2 on ESP32 boards)
#define LED_PIN_red 2
#define LED_PIN_yellow 0
#define LED_PIN_green 4




void RedOn(){
  digitalWrite(LED_PIN_red, HIGH);
}
void RedOff(){
  digitalWrite(LED_PIN_red, LOW);
}

void YellowOn(){
  digitalWrite(LED_PIN_yellow, HIGH);
}
void YellowOff(){
  digitalWrite(LED_PIN_yellow, LOW);
}

void GreenOn(){
  digitalWrite(LED_PIN_green, HIGH);
}
void GreenOff(){
  digitalWrite(LED_PIN_green, LOW);
} 
