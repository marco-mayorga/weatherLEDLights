#include <DHT.h>
#include <DHT_U.h>



// Led And humidity sensor consts
#define RED_LED 3
#define GREEN_LED 4
#define BLUE_LED 2
#define NUM_LEDS 33



#define DHTPIN 5
#define DHTTYPE DHT11

// TIme in seconds and hours
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600 * SECOND;

// Led Brightness
byte brightness = 255;
byte gBright = 0;
byte rBright = 0;
byte bBright = 0;
byte fadeSpeed = 2;

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {

  // Begin serial connection
  Serial.begin(9600);
  // Initialize device
  dht.begin();

  //Sets pins to output
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Basic info about the temp sensor
  sensor_t sensor;

  // Initializes the function at start up
  tempColorChanger();
}


void tempColorChanger() {

  // sensor event
  sensors_event_t event;

  // specifies the event
  dht.temperature().getEvent(&event);

  // Color Chnages to Dark Blue if colder than 70F
  if (event.temperature <= 21.1) {
    for (int i = 0; i < 256; i++) {
      analogWrite(RED_LED, rBright);
      analogWrite(GREEN_LED, gBright);
      rBright += 1;
      gBright += 1;
    }
    for (int i = 0; i < 140; i++) {
      analogWrite(BLUE_LED, brightness);
      brightness -= 1;
    }
  }

  // Color Chnages to Light Blue if temp between 71F and 78F
  else if (event.temperature >= 21.7 && event.temperature <= 25.6) {
    for (int i = 0; i < 256; i++) {
      analogWrite(RED_LED, rBright);
      rBright += 1;
    }
    for (int i = 0; i < 256; i++) {
      analogWrite(BLUE_LED, brightness);
      analogWrite(GREEN_LED, brightness);
      brightness -= 1;
    }
  }

  // Color Chnages to Yellow if temp between 79F and 88F
  else if (event.temperature >= 25.7 && event.temperature <= 31.1) {
    for (int i = 0; i < 256; i++) {
      analogWrite(BLUE_LED, bBright);
      bBright += 1;
    }
    for (int i = 0; i < 256; i++) {
      analogWrite(RED_LED, brightness);
      analogWrite(GREEN_LED, brightness);
      brightness -= 1;
    }
  }
  // Color Chnages to Red if temp higher than 88F
  else if (event.temperature > 31.1) {
    for (int i = 0; i < 256; i++) {
      analogWrite(BLUE_LED, bBright);
      analogWrite(GREEN_LED, gBright);
      bBright += 1;
      gBright += 1;
    }
    for (int i = 0; i < 256; i++) {
      analogWrite(RED_LED, brightness);
      brightness -= 1;
    }
  }
}

void loop() {
  // Get temperature event and print its value to serial monitor
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Calls function and start looping over it
  tempColorChanger();
}