#include <FastLED.h>
#include <OneButton.h>

const byte bandeBleuePin = 7;
const byte bandeRougePin = 8;
const byte bandeStripPinL = 9;
const byte bandeStripPinR = 10;

const byte boutonBleuPin  = 2;
const byte boutonRougePin = 3;
const byte boutonBlancPinL = A4;
const byte boutonBlancPinR = A5;

const int nbLedsScore = 3;
const int nbledBlanchesL = 68;
const int nbledBlanchesR = 68;

//#define BRIGHTNESS 255;             // Set BRIGHTNESS to about 1/5 (max = 255) base 50 VINCENT BOISSELIER

CRGB ledsBleues[nbLedsScore];
CRGB ledsRouges[nbLedsScore];
CRGB ledBlanchesL[nbledBlanchesL];
CRGB ledBlanchesR[nbledBlanchesR];

OneButton boutonBleu(boutonBleuPin, true);
OneButton boutonRouge(boutonRougePin, true);

int nbBleu = 0;    // Nombre de leds bleues allumées
int nbRouge = 0;   // Nombre de leds rouges allumées
int nbBlancL = 0;  //nombre de led blanche/rouges alluméés team blue
int nbBlancR = 0;  //nombre de led blanche/rouges alluméés  team red

void appuiBleu() {
  if (nbBleu < nbLedsScore) nbBleu++;
  for (int i = nbBleu; i < nbLedsScore; i++) ledsBleues[i] = CRGB::Black;
  if (nbBleu != 0)
    for (int i = 0; i < nbBleu; i++) ledsBleues[i] = CRGB::Blue;
  FastLED.show();
}

void appuiBleuLong() {
  nbBleu = 0;
  fill_solid(ledsBleues, nbLedsScore, CRGB::Black);
  FastLED.show();
}

void appuiRouge() {
  if (nbRouge < nbLedsScore) nbRouge++;
  for (int i = nbRouge; i < nbLedsScore; i++) ledsRouges[i] = CRGB::Black;
  if (nbRouge != 0)
    for (int i = 0; i < nbRouge; i++) ledsRouges[i] = CRGB::Red;
  FastLED.show();
}

void appuiRougeLong() {
  nbRouge = 0;
  fill_solid(ledsRouges, nbLedsScore, CRGB::Black);
  FastLED.show();
}

void testBoutonBlancL() {
  static bool boutonAppuyeL = false;
  if (digitalRead(boutonBlancPinL) == LOW) { // bouton en appui
    if (! boutonAppuyeL) { // changement d'état
      boutonAppuyeL = true;
      fill_solid(ledBlanchesL, nbledBlanchesL, CRGB::Red);
      FastLED.show();
      delay(20); // anti bounce du pauvre
    }
  } else { // bouton pas appuyé
    if (boutonAppuyeL) { // c'est le relâchement
      boutonAppuyeL = false;
      fill_solid(ledBlanchesL, nbledBlanchesL, CRGB::White);
      FastLED.show();
      delay(20); // anti bounce du pauvre
    }
  }
}

void testBoutonBlancR() {
  static bool boutonAppuyeR = false;
  if (digitalRead(boutonBlancPinR) == LOW) { // bouton en appui
    if (! boutonAppuyeR) { // changement d'état
      boutonAppuyeR = true;
      fill_solid(ledBlanchesR, nbledBlanchesR, CRGB::Red);
      FastLED.show();
      delay(20); // anti bounce du pauvre
    }
  } else { // bouton pas appuyé
    if (boutonAppuyeR) { // c'est le relâchement
      boutonAppuyeR = false;
      fill_solid(ledBlanchesR, nbledBlanchesR, CRGB::White);
      FastLED.show();
      delay(20); // anti bounce du pauvre
    }
  }
}


void setup() {
  pinMode(boutonBlancPinL, INPUT_PULLUP);
  pinMode(boutonBlancPinR, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, bandeBleuePin, GRB>(ledsBleues, nbLedsScore);
  FastLED.addLeds<WS2812B, bandeRougePin, GRB>(ledsRouges, nbLedsScore);
  FastLED.addLeds<WS2812B, bandeStripPinL, GRB>(ledBlanchesL, nbledBlanchesL);
  FastLED.addLeds<WS2812B, bandeStripPinR, GRB>(ledBlanchesR, nbledBlanchesR);
  
  boutonBleu.attachClick(appuiBleu);
  boutonRouge.attachClick(appuiRouge);

  boutonBleu.attachLongPressStart(appuiBleuLong);
  boutonRouge.attachLongPressStart(appuiRougeLong);

  fill_solid(ledBlanchesL, nbledBlanchesL, CRGB::White);
  FastLED.show();
  Serial.begin(115200);
  Serial.println("PRET");

   fill_solid(ledBlanchesR, nbledBlanchesR, CRGB::White);
  FastLED.show();
  Serial.begin(115200);
  Serial.println("PRET");
}

void loop() {
  boutonBleu.tick();
  boutonRouge.tick();
  testBoutonBlancL();
  testBoutonBlancR();
  // ici vous pouvez faire autre chose de non bloquant
}
