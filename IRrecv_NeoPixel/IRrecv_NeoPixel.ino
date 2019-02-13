
#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NEO_PIN_R 6
#define NEO_PIN_L 7


int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

Adafruit_NeoPixel strip_right = Adafruit_NeoPixel(10, NEO_PIN_R, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_left = Adafruit_NeoPixel(10, NEO_PIN_L, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  strip_right.begin();
  strip_right.show(); // Initialize all pixels to 'off'
  strip_left.begin();
  strip_left.show(); // Initialize all pixels to 'off'
}


void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 1; i < count; i++) {
    if (i & 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void loop() {
  if (irrecv.decode(&results)) { //디코딩 결과
    Serial.println(results.value, HEX);
    dump(&results);
    irrecv.resume(); // Receive the next value
    if (results.value == 0xD7E84B1B)
    {
        colorWipe(strip_right, strip_right.Color(255, 255, 0), 50); // Yellow
        delay(1000);
    }
    if (results.value == 0xFD609F)
    {
      colorWipe(strip_left, strip_left.Color(255, 255, 0), 50); // Red
      delay(1000);
    }
    
    delay(200);
  }
}
