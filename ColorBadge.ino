#include "config.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include <ArduinoJson.h>

//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
//
//#include <ESP8266HTTPClient.h>
//
//#include <WiFiClient.h>

#include "colors.h"

#define PIN 4
#define CALL_BUTTON_PIN 5
#define AWAY_BUTTON_PIN 14
//#define LED_PIN 14

// button state
bool current = false;
bool last = false;

int pass = 0;

volatile boolean neopixel_state = false; // off by default!
volatile boolean badge_red = false;
volatile boolean badge_blue = false;
volatile boolean badge_green = false;

AdafruitIO_Feed *commandfeed = io.feed("jsoncolors");
AdafruitIO_Feed *buttonfeed = io.feed("buttons");

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(4, 8, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(0, 0, 0), matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

const String hexes[] = {
  "#000000", "#FF0000", "#00FF00", "#0000FF"
};

StaticJsonDocument<200> jsonDoc;

// Initialize everything and prepare to start
void setup()
{

  pinMode(CALL_BUTTON_PIN, INPUT);
  pinMode(AWAY_BUTTON_PIN, INPUT);

  //  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  Serial.println("Connecting to Adafruit IO");
  io.connect();

  commandfeed->onMessage(handleMessage);

  matrix.begin();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");

  commandfeed->get();

  Serial.println();
  Serial.println(io.statusText());

  matrix.setBrightness(50);
  matrix.fillScreen(matrix.Color(255, 0, 0));
  matrix.show();

  //  commandfeed->save("#FFFFFF");
  delay(500);
  matrix.fillScreen(colors[0]);
  matrix.show();
  delay(250);
  //  commandfeed->save("#000000");

}


void loop() {
  // put your main code here, to run repeatedly:
  io.run();

  if (digitalRead(CALL_BUTTON_PIN) == LOW) {
    current = true;
  } else {
    current = false;
  }

  if (current == last) {
    return;
  }

  if (current) {
    Serial.println("button");
    buttonfeed->save(1);

    if (++pass >= 2) pass = 0;
    Serial.printf("PASS %d\n", pass);

    jsonDoc["color"] = hexes[pass];
    String jsonOutput;
    serializeJson(jsonDoc, jsonOutput);
    Serial.println(jsonOutput);

    commandfeed->save(jsonOutput);

    //    matrix.fillScreen(colors[pass]);
    //    matrix.show();

    //    digitalWrite(LED_PIN, HIGH);
  } else {
    //    digitalWrite(LED_PIN, LOW);
  }

  last = current;

}

void handleMessage(AdafruitIO_Data *data) {
  Serial.printf("Incoming Feed Message: %s\n", data->value());

  char* msg = data->toChar();

  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, msg);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  const char* color = doc["color"].as<char*>();
  unsigned int brightness = doc["brightness"].as<unsigned int>();

  Serial.printf("Setting Color to %s and Brightness to %d\n", color, brightness);
  long long number = strtoll( &color[1], NULL, 16);
  Color c = Color(number);

  Serial.printf("%d %d %d\n", c.Red(), c.Green(), c.Blue());

  if (brightness != 0)
    matrix.setBrightness(brightness);

  matrix.fillRect(0, 0, 4, 8, matrix.Color(c.Red(), c.Green(), c.Blue()));

  matrix.show();
}
