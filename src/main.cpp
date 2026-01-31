#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <DHT.h>

#define IR_RECEIVE_PIN 7
#define VRx A0
#define VRy A1
#define SW 3
#define BUZZER_PIN 8
#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 10
#define DHTPIN 2
#define DHTTYPE DHT11

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int nokiaMel[] = {NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_D5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, NOTE_CS5};
int nokiaLen[] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};

int swMel[] = {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4};
int swLen[] = {4,4,4,3,1,4,3,1,2,4,4,4,3,1,4,3,1,2};

int marioMel[] = {
  NOTE_E6, NOTE_E6, 0, NOTE_E6, 0, NOTE_C6, NOTE_E6, 0, NOTE_G6, 0,0,0, NOTE_G5,0,0,0,
  NOTE_C6,0,0,NOTE_G5,0,0,NOTE_E5,0,0,NOTE_A5,0,NOTE_B5,0,NOTE_AS5,NOTE_A5,0,
  NOTE_G5,NOTE_E6,NOTE_G6,NOTE_A6,0,NOTE_F6,NOTE_G6,0,NOTE_E6,0,NOTE_C6,NOTE_D6,NOTE_B5,0,0,
  NOTE_C6,0,0,NOTE_G5,0,0,NOTE_E5,0,0,NOTE_A5,0,NOTE_B5,0,NOTE_AS5,NOTE_A5,0,
  NOTE_G5,NOTE_E6,NOTE_G6,NOTE_A6,0,NOTE_F6,NOTE_G6,0,NOTE_E6,0,NOTE_C6,NOTE_D6,NOTE_B5,0,0
};
int marioLen[] = {
  12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
  12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
  9,9,9,12,12,12,12,12,12,12,12,12,12,12,12,
  12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
  9,9,9,12,12,12,12,12,12,12,12,12,12,12,12,12
};



LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

enum AppMode { MENU, GAME, MUSIC_MENU, PLAY_MUSIC, CALCULATOR, WEATHER, DINO, SIMON };
AppMode currentMode = MENU;

const char* menuItems[] = {"Snake", "Music", "Calc", "Weather", "Dino", "Simon"};
const int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
int selectedMenu = 0;

// --- Snake ---
#define GRAPHIC_WIDTH 16
#define GRAPHIC_HEIGHT 2
struct Pos { int8_t x, y; };
Pos snakeBody[GRAPHIC_WIDTH * GRAPHIC_HEIGHT];
size_t snakeLength = 1;
int dir = 1;
Pos food;
unsigned long lastMove = 0;
const unsigned long moveInterval = 400;
void placeFood() {
  bool valid;
  do {
    food.x = random(GRAPHIC_WIDTH);
    food.y = random(GRAPHIC_HEIGHT);
    valid = true;
    for (size_t i = 0; i < snakeLength; i++)
      if (snakeBody[i].x == food.x && snakeBody[i].y == food.y) valid = false;
  } while (!valid);
}
void drawGame() {
  lcd.clear();
  for (size_t i = 0; i < snakeLength; i++) {
    lcd.setCursor(snakeBody[i].x, snakeBody[i].y);
    lcd.write(byte(255));
  }
  lcd.setCursor(food.x, food.y);
  lcd.write('O');
}
void gameLoop() {
  int xVal = analogRead(VRx), yVal = analogRead(VRy);
  bool button = digitalRead(SW);
  if (xVal < 400) dir = 0;
  else if (xVal > 600) dir = 1;
  else if (yVal < 400) dir = 2;
  else if (yVal > 600) dir = 3;
  if (button == LOW) { currentMode = MENU; lcd.clear(); delay(500); return; }
  if (millis() - lastMove > moveInterval) {
    lastMove = millis();
    for (int i = snakeLength - 1; i > 0; i--) snakeBody[i] = snakeBody[i - 1];
    switch (dir) {
      case 0: snakeBody[0].x--; break; case 1: snakeBody[0].x++; break;
      case 2: snakeBody[0].y--; break; case 3: snakeBody[0].y++; break;
    }
    if (snakeBody[0].x < 0) snakeBody[0].x = GRAPHIC_WIDTH - 1;
    if (snakeBody[0].x >= GRAPHIC_WIDTH) snakeBody[0].x = 0;
    if (snakeBody[0].y < 0) snakeBody[0].y = GRAPHIC_HEIGHT - 1;
    if (snakeBody[0].y >= GRAPHIC_HEIGHT) snakeBody[0].y = 0;
    for (int i = 1; i < snakeLength; i++)
      if (snakeBody[i].x == snakeBody[0].x && snakeBody[i].y == snakeBody[0].y) {
        lcd.clear(); lcd.print("Game Over!"); delay(1500); currentMode = MENU; return;
      }
    if (snakeBody[0].x == food.x && snakeBody[0].y == food.y) {
      if (snakeLength < sizeof(snakeBody)/sizeof(*snakeBody)) snakeLength++;
      placeFood();
    }
    drawGame();
  }
}

// --- Music ---
const char* musicItems[] = {"nokia", "starwars", "Mario"};
const int musicItemCount = sizeof(musicItems) / sizeof(musicItems[0]);
int selectedMusic = 0;
void playSelectedMusic() {
  lcd.clear();
  lcd.print("Playing:");
  lcd.setCursor(0, 1);
  lcd.print(musicItems[selectedMusic]);
  int melodyPin = BUZZER_PIN;

  switch (selectedMusic) {
    case 0: // nokia
      for (int i = 0; i < 16; i++) {
        tone(melodyPin, nokiaMel[i], 200);
        delay(nokiaLen[i]*30);
        noTone(melodyPin);
      }
      break;
    case 1: // starwars
      for (int i = 0; i < 18; i++) {
        tone(melodyPin, swMel[i], 200);
        delay(swLen[i]*30);
        noTone(melodyPin);
      }
      break;
    case 2: // Mario
  for (int i = 0; i < 30; i++) {
    tone(melodyPin, marioMel[i], 220); // or *25, *30, etc.
    delay(220); // a little longer than tone duration
    noTone(melodyPin);
  }
  break;
  }
  delay(500);
  currentMode = MUSIC_MENU;
}
void updateMusicMenu() {
  int xVal = analogRead(VRx), yVal = analogRead(VRy);
  bool button = digitalRead(SW);
  static int lastSelectedMusic = -1; // Add this line

  if (xVal < 400) { selectedMusic = (selectedMusic - 1 + musicItemCount) % musicItemCount; delay(200);}
  else if (xVal > 600) { selectedMusic = (selectedMusic + 1) % musicItemCount; delay(200);}

  // Only update the LCD if the selection changed
  if (selectedMusic != lastSelectedMusic) {
    lcd.clear(); lcd.print(">"); lcd.print(musicItems[selectedMusic]);
    lastSelectedMusic = selectedMusic;
  }

  if (button == LOW) { currentMode = PLAY_MUSIC; delay(300);}
  if (yVal > 600) { currentMode = MENU; delay(300);}
}

// --- Calculator ---
String calcInput = ""; long calcResult = 0; char lastOp = 0;
void calculatorLoop() {
  static String lastInput = "";
  static bool justShowedResult = false;

  // Force UI to show immediately when entering calculator mode
  static AppMode lastMode = MENU;
  if (lastMode != currentMode) {
    lastInput = "";
    justShowedResult = true;
    lastMode = currentMode;
  }

  if (calcInput != lastInput || justShowedResult) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calc:");
    lcd.setCursor(0, 1);
    lcd.print(calcInput.substring(0, 16));
    lastInput = calcInput;
    justShowedResult = false;
  }

  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    if (code == 0xEE11FB04) calcInput += "1";
    else if (code == 0xED12FB04) calcInput += "2";
    else if (code == 0xEC13FB04) calcInput += "3";
    else if (code == 0xEB14FB04) calcInput += "4";
    else if (code == 0xEA15FB04) calcInput += "5";
    else if (code == 0xE916FB04) calcInput += "6";
    else if (code == 0xE817FB04) calcInput += "7";
    else if (code == 0xE718FB04) calcInput += "8";
    else if (code == 0xE619FB04) calcInput += "9";
    else if (code == 0xEF10FB04) calcInput += "0";
    else if (code == 0xFD02FB04) { calcInput += "+"; lastOp = '+'; }
    else if (code == 0xFC03FB04) { calcInput += "-"; lastOp = '-'; }
    else if (code == 0xFF00FB04) { calcInput += "*"; lastOp = '*'; }
    else if (code == 0xFE01FB04) { calcInput += "/"; lastOp = '/'; }
    else if (code == 0x6996FB04) {
      int opIdx = calcInput.indexOf(lastOp);
      if (opIdx > 0) {
        long a = calcInput.substring(0, opIdx).toInt();
        long b = calcInput.substring(opIdx + 1).toInt();
        switch (lastOp) {
          case '+': calcResult = a + b; break;
          case '-': calcResult = a - b; break;
          case '*': calcResult = a * b; break;
          case '/': calcResult = b != 0 ? a / b : 0; break;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Result:");
        lcd.setCursor(0, 1);
        lcd.print(calcResult);
        delay(2000);
        calcInput = "";
        calcResult = 0;
        lastOp = 0;
        justShowedResult = true;
      }
    }
    else if (code == 0xFFA25D) { calcInput = ""; calcResult = 0; lastOp = 0; }
    else if (code == 0xFF629D) { currentMode = MENU; }
    IrReceiver.resume();
  }
}

// --- Weather ---
void weatherLoop() {
  float temp = dht.readTemperature(), hum = dht.readHumidity();
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Weather:");
  lcd.setCursor(0, 1);
  if (isnan(temp) || isnan(hum)) lcd.print("Sensor Error");
  else { lcd.print("T:"); lcd.print(temp, 1); lcd.print((char)223); lcd.print("C H:"); lcd.print(hum, 0); lcd.print("%"); }
  delay(2000);
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    if (code == 0xFF629D) currentMode = MENU;
    IrReceiver.resume();
  }
}

// --- Dino ---
int dinoY = 1, dinoVel = 0, obstacleX = 15, dinoScore = 0;
bool dinoGameOver = false;
unsigned long lastDinoUpdate = 0;
const unsigned long dinoInterval = 120;
void dinoGameLoop() {
  static int obstacleY = 1;
  static unsigned long dinoIntervalDynamic = dinoInterval;
  int yVal = analogRead(VRy);
  bool jump = (yVal < 400);
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    if (code == 0xEE11FB04) jump = true;
    if (code == 0xFF629D) { currentMode = MENU; IrReceiver.resume(); return; }
    IrReceiver.resume();
  }
  if (dinoGameOver) {
    lcd.clear(); lcd.print("Game Over!"); lcd.setCursor(0, 1); lcd.print("Score:"); lcd.print(dinoScore);
    delay(2000); dinoGameOver = false; dinoY = 1; dinoVel = 0; obstacleX = 15; obstacleY = 1; dinoScore = 0; dinoIntervalDynamic = dinoInterval; currentMode = MENU; return;
  }
  dinoIntervalDynamic = max(40UL, dinoInterval - dinoScore * 7);
  if (millis() - lastDinoUpdate > dinoIntervalDynamic) {
    lastDinoUpdate = millis();
    if (jump && dinoY == 1) dinoVel = -4;
    dinoY += dinoVel;
    if (dinoY < 0) dinoY = 0; if (dinoY > 1) dinoY = 1;
    if (dinoY < 1) dinoVel++; else dinoVel = 0;
    obstacleX--;
    if (obstacleX < 0) { obstacleX = 15; dinoScore++; obstacleY = random(0, 2); }
    if (obstacleX == 1 && dinoY == obstacleY) dinoGameOver = true;
    lcd.clear(); lcd.setCursor(0, dinoY); lcd.print("D");
    lcd.setCursor(obstacleX, obstacleY); lcd.print("|");
    lcd.setCursor(10, 0); lcd.print("S:"); lcd.print(dinoScore);
  }
}

// --- Simon Says ---
void simonSaysLoop() {
  static int sequence[32], seqLen = 0, userStep = 0, showIdx = 0;
  static bool showing = true, gameOver = false;
  static unsigned long lastShow = 0;
  const unsigned long btnCodes[4] = {0xEE11FB04, 0xED12FB04, 0xEC13FB04, 0xEB14FB04};
  const int ledPins[4] = {LED1, LED2, LED3, LED4};
  if (gameOver) {
    lcd.clear(); lcd.print("Game Over!"); lcd.setCursor(0, 1); lcd.print("Score:"); lcd.print(seqLen - 1);
    delay(2000); seqLen = 0; userStep = 0; showing = true; showIdx = 0; gameOver = false; currentMode = MENU; return;
  }
  if (seqLen == 0) { sequence[0] = random(0, 4); seqLen = 1; userStep = 0; showing = true; showIdx = 0; lcd.clear(); lcd.print("Simon Says!"); delay(1000);}
  if (showing) {
    if (millis() - lastShow > 500) {
      for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
      if (showIdx < seqLen) { int led = sequence[showIdx]; digitalWrite(ledPins[led], HIGH); lastShow = millis(); showIdx++; }
      else { showing = false; showIdx = 0; userStep = 0; delay(300); for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);}
    }
    return;
  }
  lcd.setCursor(0, 0); lcd.print("Repeat:"); lcd.setCursor(0, 1); lcd.print("Score:"); lcd.print(seqLen - 1);
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    if (code == 0x0) { IrReceiver.resume(); return; }
    int btn = -1;
    for (int i = 0; i < 4; i++) if (code == btnCodes[i]) btn = i;
    if (code == 0xFF629D) { currentMode = MENU; IrReceiver.resume(); return; }
    if (btn != -1) {
      digitalWrite(ledPins[btn], HIGH); delay(200); digitalWrite(ledPins[btn], LOW);
      unsigned long t0 = millis();
      while (millis() - t0 < 500) {
        if (IrReceiver.decode()) {
          unsigned long nextCode = IrReceiver.decodedIRData.decodedRawData;
          if (nextCode == 0x0) { IrReceiver.resume(); continue; }
          bool isBtn = false; for (int i = 0; i < 4; i++) if (nextCode == btnCodes[i]) isBtn = true;
          IrReceiver.resume(); if (!isBtn) break;
        }
      }
      if (btn == sequence[userStep]) {
        userStep++;
        if (userStep == seqLen) { seqLen++; sequence[seqLen - 1] = random(0, 4); showing = true; showIdx = 0; delay(500);}
      } else gameOver = true;
    }
    IrReceiver.resume();
  }
}

// --- Menu ---
void updateMenu() {
  static int lastSelectedMenu = -1;
  static unsigned long lastMoveTime = 0;
  const unsigned long menuMoveDelay = 250;
  int xVal = analogRead(VRx), yVal = analogRead(VRy);
  bool button = digitalRead(SW);
  unsigned long now = millis();
  if (now - lastMoveTime > menuMoveDelay) {
    if (xVal < 400) { selectedMenu = (selectedMenu - 1 + menuItemCount) % menuItemCount; lastMoveTime = now; }
    else if (xVal > 600) { selectedMenu = (selectedMenu + 1) % menuItemCount; lastMoveTime = now; }
  }
  if (selectedMenu != lastSelectedMenu) {
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("Menu:"); lcd.setCursor(0, 1); lcd.print(">"); lcd.print(menuItems[selectedMenu]);
    lastSelectedMenu = selectedMenu;
  }
  if (button == LOW) {
    switch (selectedMenu) {
      case 0: // Snake
        currentMode = GAME;
        snakeBody[0] = {GRAPHIC_WIDTH / 2, GRAPHIC_HEIGHT / 2};
        snakeLength = 1;
        dir = 1;
        placeFood();
        drawGame();
        break;
      case 1: currentMode = MUSIC_MENU; break;
      case 2: currentMode = CALCULATOR; break;
      case 3: currentMode = WEATHER; break;
      case 4: dinoY = 1; dinoVel = 0; obstacleX = 15; dinoGameOver = false; dinoScore = 0; currentMode = DINO; break;
      case 5: currentMode = SIMON; break;
    }
    delay(300);
  }
}

// --- Setup & Loop ---
void setup() {
  Serial.begin(9600);
  lcd.init(); lcd.backlight();
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT); pinMode(LED3, OUTPUT); pinMode(LED4, OUTPUT);
  randomSeed(analogRead(A1));
  dht.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Menu:"); lcd.setCursor(0, 1); lcd.print(">"); lcd.print(menuItems[selectedMenu]);
}

void loop() {
  switch (currentMode) {
    case MENU: updateMenu(); break;
    case GAME: gameLoop(); break;
    case MUSIC_MENU: updateMusicMenu(); break;
    case PLAY_MUSIC: playSelectedMusic(); break;
    case CALCULATOR: calculatorLoop(); break;
    case WEATHER: weatherLoop(); break;
    case DINO: dinoGameLoop(); break;
    case SIMON: simonSaysLoop(); break;
  }
}