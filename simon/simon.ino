/*
A simon game for Arduino.
*/

#define MAX_LEVEL 10
#define TIME_COLOR_SHOWN_READ 200
#define TIME_COLOR_SHOWN_WRITE 400

int currentSequence[MAX_LEVEL] = { 0 };
int inputSequence[MAX_LEVEL] = { 0 };

// ledId = 0
int indicatorRed = 11;
int indicatorGreen = 10;
int indicatorBlue = 9;

// ledId = 1
int simonRed = 6;
int simonGreen = 5;
int simonBlue = 3;

int redButton = 2; // btnId = 0
int greenButton = 1; // btnId = 1
int blueButton = 0; // btnId = 2

int resetButton = 8; // btnId = 3

int action_performed = 0;
int pressed = 0;

int gameState = -1; // -1: prestart, 0: showing_sequence, 1: reading_sequence, 2: test, 3: lost, 4: win
int level = 1;
int currentInput = 0;

//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE

void setup()
{
  pinMode(indicatorRed, OUTPUT);
  pinMode(indicatorGreen, OUTPUT);
  pinMode(indicatorBlue, OUTPUT);

  pinMode(simonRed, OUTPUT);
  pinMode(simonGreen, OUTPUT);
  pinMode(simonBlue, OUTPUT);

  pinMode(redButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);

  pinMode(resetButton, INPUT_PULLUP);
  
  Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready - Leonardo
  Serial.println("Serial Ready\n");
}

void loop()
{
  if (gameState == -1) {
    // Since Arduino's random number generator is pseudo-random, when gameState is -1 (prestart), we
    // keep reseting to actually get a random sequence. 
    reset();
    gameState = -1;
  }

  if (digitalRead(resetButton) == LOW) {
    reset();
  }

  if (gameState == 0) {
    setColor(0, 2);  // state indicator blue

    delay(1000);

    printSequence();
    
    gameState = 1;
  }

  if (gameState == 1) {
    setColor(0, 3);  // state indicator blank

    if (digitalRead(redButton) == LOW) {
      inputSequence[currentInput] = 0;
      setColor(1, 0);
      currentInput++;
    } else if (digitalRead(greenButton) == LOW) {
      inputSequence[currentInput] = 1;
      setColor(1, 1);
      currentInput++;
    } else if (digitalRead(blueButton) == LOW) {
      inputSequence[currentInput] = 2;
      setColor(1, 2);
      currentInput++;
    }

    delay(TIME_COLOR_SHOWN_WRITE);
    setColor(1, 3); // blank

    if (currentInput == level) {
      gameState = 2;
      currentInput = 0; // init currentInput
    }

  }

  if (gameState == 2) {
    setColor(0, 3);  // state indicator blank

    if (correctInput()) {
      level++;

      if (level == MAX_LEVEL) {
        gameState = 4;
      } else {
        gameState = 0;
      }
    } else {
      gameState = 3;
    }
  }

  if (gameState == 3) {
    setColor(0, 0);  // state indicator red
  }

  if (gameState == 4) {
    setColor(0, 1);  // state indicator green
  }
}

void reset() {
  gameState = 0;
  level = 1;
  currentInput = 0;

  Serial.println("Reset\n New Sequence: ");
  for ( int i = 0; i < MAX_LEVEL; i++ ) {
    currentSequence[i] = random(3);
    Serial.println(currentSequence[i]);
  }
}

void printSequence() {
  for ( int i = 0; i < level; i++ ) {
    setColor(1, currentSequence[i]);
    delay(TIME_COLOR_SHOWN_READ);
    setColor(1, 3); // blank
    delay(TIME_COLOR_SHOWN_READ);
  }
  setColor(1, 3); // blank
}

int correctInput() {
  for ( int i = 0; i < level; i++ ) {
    if (currentSequence[i] != inputSequence[i]) {
      return 0;
    }
  }
  return 1;
}

void setColor(int ledId, int color) { // colors: 0: red; 1: green; 2: blue; 3: blank
  int red = 0, green = 0, blue = 0;

  if (color == 0) {
    red = 255;
  } else if (color == 1) {
    green = 255;
  } else if (color == 2) {
    blue = 255;
  }

  if (ledId == 0) {
    analogWrite(indicatorRed, red);
    analogWrite(indicatorGreen, green);
    analogWrite(indicatorBlue, blue); 
  } else {
    analogWrite(simonRed, red);
    analogWrite(simonGreen, green);
    analogWrite(simonBlue, blue);
  }
}
