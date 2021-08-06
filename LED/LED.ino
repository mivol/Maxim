#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

int led_1 = 2;
#define NUMPIXELS 30 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, led_1, NEO_GRB  + NEO_KHZ800);

void whiteLight(){
  //pixels.clear(); // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(50); // Pause before next pass through loop
  }
}

void Farbverlauf(){
  int color1 = 250;
  int color2 = 0;
  int color3 = 250;
  //pixels.clear(); // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(color1, color2, color3));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(50); // Pause before next pass through loop
    color1-=8;
    color2+=8;
    color3-=8;
  }
}

void setup() {
  //#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  //clock_prescale_set(clock_div_1);
  //#endif
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

}

void loop() {
  whiteLight();
  delay(1000);
  Farbverlauf();
  delay(2000);
}
