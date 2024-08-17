#include <TFT_eSPI.h>
#include <OneButton.h>
#include <WiFiManager.h>

#include "colors.h"
#include "freefonts.h"
#include "Orbitron_Medium_20.h"
#include "cartouche35.h"
#include "cartouche48.h"
#include "pyramids.h"
#include "players.h"
#include "scores.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite bg = TFT_eSprite(&tft);
TFT_eSprite neonZero = TFT_eSprite(&tft);
TFT_eSprite dice = TFT_eSprite(&tft);
TFT_eSprite dice2 = TFT_eSprite(&tft);
TFT_eSprite dice3 = TFT_eSprite(&tft);
TFT_eSprite dice4 = TFT_eSprite(&tft);
TFT_eSprite counters = TFT_eSprite(&tft);

const int buttonPin = 0;            // LEFT BUTTON
const int buttonPin1 = 14;          // RIGHT BUTTON
const int diceCount = 4;            // NUMBER OF DICE
const int diceSize = 76;            // SIZE OF DICE

bool pressed = false;

int debounce = 0;
int debounce2 = 0;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;  // Debounce time in milliseconds

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);

  randomSeed(analogRead(0));

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  bg.createSprite(320, 170);
  bg.setSwapBytes(true);

  neonZero.createSprite(76, 76);
  neonZero.setSwapBytes(true);

  dice.createSprite(76, 76);
  dice.setSwapBytes(true);
  dice2.createSprite(76, 76);
  dice2.setSwapBytes(true);
  dice3.createSprite(76, 76);
  dice3.setSwapBytes(true);
  dice4.createSprite(76, 76);
  dice4.setSwapBytes(true);

  counters.createSprite(155, 170);
  counters.setSwapBytes(true);

}

void drawBg() {
  bg.fillSprite(BLACK);
  bg.pushSprite(0, 0);
}

void showRandomPyramids() {
  int numberOfPyramids = random(0, 5);  // Random number between 0 and 4

  // Clear the previous display
  tft.fillScreen(BLACK);

  // Display the pyramids
  for (int i = 0; i < numberOfPyramids; ++i) {
    int x = 5 + (i % 2) * 78;  // X position for alignment
    int y = 11 + (i / 2) * 72;  // Y position for alignment

    dice.pushImage(0, 0, 76, 76, neonPyramid3);
    dice.pushSprite(x, y);
  }
}

void drawCounters() {
  // PLAYER 1 WIN COUNTER
  counters.pushImage(10, 4, 93, 74, player1);
  counters.pushImage(100, 17, 50, 50, winCounter);
  counters.fillRoundRect(110, 14, 30, 55, 5, BROWN);
  counters.drawRoundRect(110, 14, 30, 55, 3, GOLD);
  counters.fillRoundRect(115, 20, 19, 42, 7, BLACK);
  counters.fillCircle(106, 41, 2, BLUE);
  counters.fillCircle(143, 41, 2, BLUE);
  counters.loadFont(cartouche48);
  counters.setTextSize(1);
  counters.setTextColor(GOLD);
  counters.setCursor(113, 20);
  counters.println("3");

  counters.drawSmoothRoundRect(5, 0, 5, 5, 149, 82, GOLD);

  // PLAYER 2 WIN COUNTER
  counters.pushImage(20, 93, 74, 72, player2);
  counters.pushImage(100, 104, 50, 50, winCounter);
  counters.fillRoundRect(110, 101, 30, 55, 5, BROWN);
  counters.drawRoundRect(110, 101, 30, 55, 3, GOLD);
  counters.fillRoundRect(115, 107, 19, 42, 7, BLACK);
  counters.fillCircle(106, 127, 2, BLUE);
  counters.fillCircle(143, 127, 2, BLUE);
  counters.setTextSize(1);
  counters.setCursor(113, 107);
  counters.println("0");
  counters.drawSmoothRoundRect(5, 87, 5, 5, 149, 81, GOLD);

  counters.pushSprite(165, 0);
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check for button press with debounce
  if (digitalRead(buttonPin) == LOW && (currentTime - lastButtonPress) > debounceDelay) {
    lastButtonPress = currentTime;
    showRandomPyramids();  // Show a random number of pyramids
  }

  tft.drawSmoothRoundRect(0, 0, 5, 5, 164, 168, GOLD);
  drawCounters();
}
