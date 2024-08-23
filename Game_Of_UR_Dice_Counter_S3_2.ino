#include <TFT_eSPI.h>
#include <OneButton.h>
#include <WiFiManager.h>

#include "colors.h"
#include "freefonts.h"
#include "Orbitron_Medium_20.h"
#include "cartouche48.h"
#include "pyramids.h"
#include "players.h"
#include "scores.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite bg = TFT_eSprite(&tft);             // BACKGROUND SPRITE
TFT_eSprite dice = TFT_eSprite(&tft);           // DICE SPRITE (PYRAMID)
TFT_eSprite counters = TFT_eSprite(&tft);       // PLAYER COUNTERS
TFT_eSprite dicebg = TFT_eSprite(&tft);         // SPRITE TO CLEAR DICE FOR REFRESH OF NEXT ROLL

const int buttonPin = 0;            // LEFT BUTTON
const int buttonPin1 = 14;          // RIGHT BUTTON
const int diceCount = 4;            // NUMBER OF DICE
const int diceSize = 76;            // SIZE OF DICE

bool pressed = false;

int debounce = 0;
int debounce2 = 0;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 1000;  // DEBOUNCE TIME IN MILLISECONDS

int player1Counter = 0; // PLAYER 1 COUNTER INITIALIZED TO 0

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);

  randomSeed(analogRead(0));

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  bg.createSprite(320, 170);                // BACKGROUND SPRITE SIZE
  bg.setSwapBytes(true);

  dice.createSprite(76, 76);                // DICE SPRITE SIZE
  dice.setSwapBytes(true);

  dicebg.createSprite(154, 154);            // DICE BACKGROUND REFRESH SPRITE

  counters.createSprite(155, 170);
  counters.setSwapBytes(true);

  drawCounters();                           // INITIAL DRAWING OF COUNTERS
}

void drawBg() {                             
  bg.fillSprite(BLACK);                     // BACKGROUND SPRITE COLOR
  bg.pushSprite(0, 0);                      // BACKGROUND SPRITE PLACEMENT
}

void showRandomPyramids() {
  int numberOfPyramids = random(0, 5);      // RANDOM NUMBER OF PYRAMIDS BETWEEN 0 AND 4

  dicebg.fillSprite(BLACK);                 // DICE REFRESH SPRITE COLOR
  dicebg.pushSprite(3, 3);                  // DICE REFRESH SPRITE PLACEMENT

  // DISPLAY THE PYRAMIDS
  for (int i = 0; i < numberOfPyramids; ++i) {
    int x = 5 + (i % 2) * 78;               // X POSITION FOR ALIGNMENT
    int y = 11 + (i / 2) * 72;              // Y POSITION FOR ALIGNMENT

    dice.pushImage(0, 0, 76, 76, neonPyramid3);
    dice.pushSprite(x, y);
  }
}

void drawCounters() {
  counters.fillSprite(BLACK);               // CLEAR THE COUNTERS SPRITE BEFORE DRAWING

  // PLAYER 1 WIN COUNTER
  counters.pushImage(10, 4, 93, 74, player1);                 // PLAYER 1 IMAGE SIZE AND POSITION
  counters.pushImage(100, 17, 50, 50, winCounter);            // PLAYER 1 WIN COUNTER IMAGE SIZE AND POSITION
  counters.fillRoundRect(110, 14, 30, 55, 5, BROWN);          // PLAYER 1 BACKGROUND COLOR DIRECTLY BEHIND COUNTER
  counters.drawRoundRect(110, 14, 30, 55, 3, GOLD);           // PLAYER 1 OUTLINE OUTSIDE BROWN COLOR (SEE ABOVE LINE)
  counters.fillRoundRect(115, 20, 19, 42, 7, BLACK);          // PLAYER 1 COUNTER (CARTOUCHE) BACKGROUND COLOR
  counters.fillCircle(106, 41, 2, BLUE);                      // BLUE DOT FOR DETAILING NEXT TO WIN COUNTER (RIGHT SIDE)
  counters.fillCircle(143, 41, 2, BLUE);                      // BLUE DOT FOR DETAILING NEXT TO WIN COUNTER (LEFT SIDE)
  counters.loadFont(cartouche48);                             // FONT FOR WIN COUNTER NUMBERS
  counters.setTextSize(1);                                    // FONT SIZE
  counters.setTextColor(GOLD);                                // FONT COLOR
  counters.setCursor(113, 20);                                // FONT POSITION
  counters.println(String(player1Counter));                   // DISPLAYS THE PLAYER 1 COUNTER (NUMBERS)

  counters.drawSmoothRoundRect(5, 0, 5, 5, 149, 82, GOLD);    // GOLD BORDER FOR PLAYER 1

  // PLAYER 2 WIN COUNTER
  counters.pushImage(20, 93, 74, 72, player2);                // PLAYER 2 IMAGE SIZE AND POSITION
  counters.pushImage(100, 104, 50, 50, winCounter);           // PLAYER 2 WIN COUNTER IMAGE SIZE AND POSITION
  counters.fillRoundRect(110, 101, 30, 55, 5, BROWN);         // PLAYER 2 BACKGROUND COLOR DIRECTLY BEHIND COUNTER
  counters.drawRoundRect(110, 101, 30, 55, 3, GOLD);          // PLAYER 2 OUTLINE OUTSIDE BROWN COLOR (SEE ABOVE LINE)
  counters.fillRoundRect(115, 107, 19, 42, 7, BLACK);         // PLAYER 2 COUNTER (CARTOUCHE) BACKGROUND COLOR
  counters.fillCircle(106, 127, 2, BLUE);                     // BLUE DOT FOR DETAILING NEXT TO WIN COUNTER (RIGHT SIDE)
  counters.fillCircle(143, 127, 2, BLUE);                     // BLUE DOT FOR DETAILING NEXT TO WIN COUNTER (LEFT SIDE)
  counters.setTextSize(1);                                    // FONT SIZE
  counters.setCursor(113, 107);                               // FONT POSITION
  counters.println("0");                                
  counters.drawSmoothRoundRect(5, 87, 5, 5, 149, 81, GOLD);   // GOLD BORDER FOR PLAYER 2

  counters.pushSprite(165, 0);                                // POSITION TO DRAW COUNTERS
}

void loop() {
  unsigned long currentTime = millis();
  
  // CHECK FOR BUTTON PRESS WITH DEBOUNCE
  if (digitalRead(buttonPin) == LOW && (currentTime - lastButtonPress) > debounceDelay) {
    lastButtonPress = currentTime;
    showRandomPyramids();  // SHOW A RANDOM NUMBER OF PYRAMIDS
  }
  
  if (digitalRead(buttonPin1) == LOW && (currentTime - lastButtonPress) > debounceDelay) {
    lastButtonPress = currentTime;
    player1Counter++; // INCREMENT PLAYER 1 COUNTER
    if (player1Counter > 9) {
      player1Counter = 0; // RESET TO 0 IF GREATER THAN 9
    }
    drawCounters();  // REDRAW COUNTERS WITH UPDATED VALUE
  }

  tft.drawSmoothRoundRect(0, 0, 5, 5, 164, 168, GOLD); // DRAW BORDER OUTSIDE DICE (PYRAMID) COUNTERS
  drawCounters();
}
