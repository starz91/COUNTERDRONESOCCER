/* Noiasca Neopixel Display
   40 scoreboard
   a simple scoreboard for two players (A and B)
   separate displays for A and B on the same strip pin
   
   Wire three buttons from the GPIO to GND. Input pullups are activated
   
   http://werner.rothschopf.net/202005_arduino_neopixel_display.htm

   by noiasca & modifié par VINCENT BOISSELIER
   12.11.2022 DRONE SOCCER SCOREBOARD
*/

const byte ledPin = 12;                // Which pin on the Arduino is connected to the NeoPixels?
const byte numDigits = 2;              // How many digits (numbers) are available on each display
const byte pixelPerDigit = 21;         // all pixels, including decimal point pixels if available at each digit base 14
const byte addPixels = 0;              // unregular additional pixels to be added to the strip base 4

const byte startPixelA = 0;            // start pixel of display A
const byte startPixelB = 42;           // start pixel of display B (assumption: 2 x 14 used by displayA + 4 additional Pixels) base 32

const byte buttonApin = A1;            // button pin player A, TEAM BLUE connects to GND
const byte buttonBpin = A3;            // button pin player B, TEAM RED connects to GND
const byte buttonCpin = A2;            // BOUTON PIN DOWN PLAYER A TEAM BLUE - vincent boisselier
const byte buttonDpin = A4;            // BOUTON PIN DOWN PLAYER B TEAM RED - vincent boisselier
const byte buttonResetPin = A0;        // button to reset scores, connects to GND
byte counterA;                         // counts/goals/score for player A - TEAM BLUE
byte counterB;                         // counts/goals/score for player B - TEAM RED

const uint16_t ledCount(pixelPerDigit * numDigits * 2 + addPixels);
/*
   Segments are named and orded like this

          SEG_A
   SEG_F         SEG_B
          SEG_G
   SEG_E         SEG_C
          SEG_D          SEG_DP

  in the following constant array you have to define
  which pixels belong to which segment
*/

typedef uint32_t segsize_t;              
const segsize_t segment[8] {
  bit(2) | bit(1) | bit(0), // SEG_A
  bit(5) | bit(4) | bit(3), // SEG_B
  bit(8) | bit(7) | bit(6), // SEG_C
  bit(11) | bit(10) | bit(9), // SEG_D
  bit(14) | bit(13) | bit(12), // SEG_E
  bit(17) | bit(16) | bit(15), // SEG_F
  bit(20) | bit(19) | bit(18), // SEG_G
 // bit(21) // SEG_DP
} ;

#include <Adafruit_NeoPixel.h>                                       // install Adafruit library from library manager
Adafruit_NeoPixel strip(84, ledPin, NEO_GRB + NEO_KHZ800);     // create neopixel object like you commonly used with Adafruit

#include <Noiasca_NeopixelDisplay.h>                                      // download library from: http://werner.rothschopf.net/202005_arduino_neopixel_display.htm
// in this sketch we handle displayA and displayB as two individual displays:
Noiasca_NeopixelDisplay displayA(strip, segment, numDigits, pixelPerDigit, startPixelA);  // create display object, handover the name of your strip as first parameter!
Noiasca_NeopixelDisplay displayB(strip, segment, numDigits, pixelPerDigit, startPixelB);  // create display object, handover the name of your strip as first parameter!

#include <OneButton.h>                                               // install OneButton library from library manager
OneButton buttonA(buttonApin, true);
OneButton buttonB(buttonBpin, true);
OneButton buttonC(buttonCpin, true);
OneButton buttonD(buttonDpin, true);
OneButton buttonReset(buttonResetPin, true);

void clickA()
{
  counterA++;
  Serial.print(F("PlayerA ")); Serial.println(counterA);
  displayA.setCursor(0);
  if (counterA < 10) displayA.print(" ");
  displayA.print(counterA);
}

void clickB()
{
  counterB++;
  Serial.print(F("PlayerB ")); Serial.println(counterB);
  displayB.setCursor(0);
  if (counterB < 10) displayB.print(" ");
  displayB.print(counterB);
}

void clickC()
{
  counterA--;
  Serial.print(F("PlayerA ")); Serial.println(counterA);
  displayA.setCursor(0);
  if (counterA < 10) displayA.print(" ");
  displayA.print(counterA);
}

void clickD()
{
  counterB--;
  Serial.print(F("PlayerB ")); Serial.println(counterB);
  displayB.setCursor(0);
  if (counterB < 10) displayB.print(" ");
  displayB.print(counterB);
}

void resetScore()
{
  Serial.println(F("Reset Score"));
  counterA = 0;
  counterB = 0;
  displayA.clear();
  displayA.print(" 0");
  displayB.clear();
  displayB.print(" 0");
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("\nNoiascaNeopixelDisplay\n40 scoreboard two displays"));

  strip.begin();                       // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                        // Turn OFF all pixels ASAP
  strip.setBrightness(255);             // Set BRIGHTNESS to about 1/5 (max = 255) base 50 VINCENT BOISSELIER
  strip.clear();

  displayA.setColorFont(0x0000AA);     // each display gets its own color, e.g. corresponding to the button color
  displayB.setColorFont(0xAA0000);

  Serial.println(F("test display"));
  for (byte i = 99; i > 0; i = i - 11)
  {
    displayA.print(i);
    displayB.print(i);
    delay(200);
  }
  displayA.print(" 0");
  displayB.print(" 0");
  buttonA.attachClick(clickA);
  buttonB.attachClick(clickB);
  buttonC.attachClick(clickC);
  buttonD.attachClick(clickD);
  buttonReset.attachClick(resetScore);  //  buttonReset.attachLongPressStart(resetScore);
}

void loop()
{
  // put here other code which needs to run:
  buttonA.tick();
  buttonB.tick();
  buttonC.tick();
  buttonD.tick();
  buttonReset.tick();
}
