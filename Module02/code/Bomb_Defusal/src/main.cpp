#include <Arduino.h>
#include <FastLED.h>
#include <M5StickC.h>

// LED Configuration
#define LOGICAL_LEDS 10 // 10 triplets (30 physical LEDs)
#define PHYSICAL_LEDS_PER_LOGICAL 3
#define NUM_LEDS (LOGICAL_LEDS * PHYSICAL_LEDS_PER_LOGICAL) // 30 LEDs total
#define DATA_PIN 26                                         // M5StickC GPIO26 (G26 on side port)
#define COLOR_ORDER GRB                                     // Adjust based on your LED strip (e.g., RGB, BRG)

// Button Pins
#define MOVE_LEFT_PIN 39  // M5StickC Button B (left)
#define MOVE_RIGHT_PIN 37 // M5StickC Button A (right)

struct Bomb
{
  int pos;
  unsigned long startTime;
  int duration;
};

CRGB leds[NUM_LEDS];
Bomb bomb = Bomb();
int score = 0;
int playerPos = 0;
int wallPos = 0;

void setLed(int logicalIndex, CRGB color);
void movePlayer();
void moveBomb();
void moveWall();
int getDistance(int p1, int p2);
CRGB getBombColor(int timeLeft);
void gameOver(int pos);
void startGame();

void setup()
{
  M5.begin(); // Initialize M5StickC
  pinMode(MOVE_LEFT_PIN, INPUT_PULLUP);
  pinMode(MOVE_RIGHT_PIN, INPUT_PULLUP);

  // Initialize LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(160);
  startGame();
}

void loop()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  EVERY_N_MILLIS(100)
  {
    movePlayer();

    if (playerPos == bomb.pos)
    {
      score++;
      moveBomb();
      moveWall();
    }
  }

  setLed(playerPos, CRGB::Blue);
  setLed(wallPos, CRGB::Red);

  int bombTimeLeft = bomb.duration - (millis() - bomb.startTime);
  setLed(bomb.pos, getBombColor(bombTimeLeft));

  if (bombTimeLeft <= 0)
  {
    gameOver(bomb.pos);
  }
  else if (playerPos == wallPos)
  {
    gameOver(wallPos);
  }

  FastLED.show();
  M5.update(); // Update M5StickC button states (optional)
}

void startGame()
{
  bomb = Bomb();
  score = 0;
  playerPos = 0;

  moveBomb();
  moveWall();
}

// Set a logical LED (3 physical LEDs)
void setLed(int logicalIndex, CRGB color)
{
  if (logicalIndex >= 0 && logicalIndex < LOGICAL_LEDS)
  {
    int start = logicalIndex * PHYSICAL_LEDS_PER_LOGICAL;
    for (int i = 0; i < PHYSICAL_LEDS_PER_LOGICAL; i++)
    {
      int physicalIndex = start + i;
      if (physicalIndex < NUM_LEDS)
        leds[physicalIndex] = color;
    }
  }
}

// Update movement logic for 10 logical positions
void movePlayer()
{
  if (digitalRead(MOVE_LEFT_PIN) == LOW)
  { // Button B pressed
    playerPos = (playerPos - 1 + LOGICAL_LEDS) % LOGICAL_LEDS;
    delay(100); // Debounce
  }
  else if (digitalRead(MOVE_RIGHT_PIN) == LOW)
  { // Button A pressed
    playerPos = (playerPos + 1) % LOGICAL_LEDS;
    delay(100);
  }
}

void moveBomb()
{
  do
  {
    bomb.pos = random16(LOGICAL_LEDS); // 0-9 (not 0-29)
  } while (bomb.pos == playerPos);
  bomb.startTime = millis();
  bomb.duration = max(1500, 5000 - (score * 100));
}

void moveWall()
{
  do
  {
    wallPos = random16(NUM_LEDS);
  } while (getDistance(wallPos, bomb.pos) < 2 || getDistance(wallPos, playerPos) < 2);
}

int getDistance(int p1, int p2)
{
  int distance = abs(p2 - p1);
  if (distance < NUM_LEDS / 2)
  {
    return distance;
  }
  else
  {
    return NUM_LEDS - distance;
  }
}

CRGB getBombColor(int timeLeft)
{
  int brightness = map(timeLeft, bomb.duration, 0, 0, 255);
  if (timeLeft < 1000)
  {
    brightness = beatsin8(240, 0, 255);
  }

  return CRGB(brightness, brightness, 0);
}

void gameOver(int pos)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    setLed(pos - i, CRGB::Red);
    setLed(pos + i, CRGB::Red);
    FastLED.show();
    delay(30);
  }
  delay(500);

  for (int i = 255; i >= 0; i--)
  {
    fill_solid(leds, NUM_LEDS, CRGB(i, 0, 0));
    FastLED.show();
    delay(4);
  }

  startGame();
}