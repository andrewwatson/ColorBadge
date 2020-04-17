#include "config.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "colors.h"

#define PIN 4

AdafruitIO_Feed *commandfeed = io.feed("colorbadge");

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(4, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);


// Initialize everything and prepare to start
void setup()
{
  Serial.begin(115200);
  
  Serial.println("Connecting to Adafruit IO");
  io.connect();

  commandfeed->onMessage(handleMessage);

  matrix.begin();
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");

  commandfeed->get();

  Serial.println();
  Serial.println(io.statusText());

  matrix.setBrightness(30);
  matrix.drawPixel(0,0, matrix.Color(255,0,255));
  matrix.show();
  
}


void loop() {
  // put your main code here, to run repeatedly:
  io.run();
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.printf("Incoming Feed Message: %s\n", data->value());

  char* msg = data->toChar();
  
  Color c = Color(msg);
  
  matrix.drawRect(0,0,4,8, matrix.Color(c.Red(),c.Green(),c.Blue()));
//  matrix.fillScreen();
  matrix.show();
}
