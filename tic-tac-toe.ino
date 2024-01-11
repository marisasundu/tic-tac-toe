#include "SPI.h";
#include "Adafruit_WS2801.h";

uint8_t dataPin = 45;
uint8_t clockPin = 46;

const int playPin = 21;
const int upPin = 47;
const int downPin = 39;
const int leftPin = 37;
const int rightPin = 35;
const int resetPin = 34;

int playState = 0;
int upState = 0;
// int lastUpState = 0;
int downState = 0;
int leftState = 0;
int rightState = 0;
int resetState = 0;

int cursorX = 13;
int cursorY = 11;

int speed = 8;

int xOne = 18;
int xTwo = 10;
int xThree = 2;
int yOne = 0;
int yTwo = 8;
int yThree = 16;

Adafruit_WS2801 strip = Adafruit_WS2801((uint16_t)27, (uint16_t)24, dataPin, clockPin);
unsigned int trackerArray[3][3] = {{0,0,0},
                                   {0,0,0},
                                   {0,0,0}};

unsigned int arrayX[7][7] = {{1,0,0,0,0,0,1},
                             {0,1,0,0,0,1,0},
                             {0,0,1,0,1,0,0},
                             {0,0,0,1,0,0,0},
                             {0,0,1,0,1,0,0},
                             {0,1,0,0,0,1,0},
                             {1,0,0,0,0,0,1}};

// use as cursor for turn tracker (make miniO too)
unsigned int arrayMiniX[7][7] = {{0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,1,0,1,0,0},
                                 {0,0,0,1,0,0,0},
                                 {0,0,1,0,1,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0}};

unsigned int arrayO[7][7] = {{0,0,1,1,1,0,0},
                             {0,1,0,0,0,1,0},
                             {1,0,0,0,0,0,1},
                             {1,0,0,0,0,0,1},
                             {1,0,0,0,0,0,1},
                             {0,1,0,0,0,1,0},
                             {0,0,1,1,1,0,0}};

unsigned int arrayBlank[7][7] ={{0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0}};


void setup() {
  // initialize the data pin as an output:
  pinMode(dataPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(playPin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(resetPin, INPUT);

  strip.begin();
  // strip.show();
  // drawGrid();
}

void loop() {
  blankMatrix();

  drawGrid();
  // strip.show();
  // drawSeven(10,8,arrayX);
  // drawSeven(2,0,arrayO);
  // Serial.println('hello');
  // drawX(1);
  // put your main code here, to run repeatedly:
  playState = digitalRead(playPin);
  upState = digitalRead(upPin);
  downState = digitalRead(downPin);
  leftState = digitalRead(leftPin);
  rightState = digitalRead(rightPin);
  resetState = digitalRead(resetPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  // testing X
  if (playState == LOW) {
    // turn LED on:
    // digitalWrite(ledPin, HIGH);
    drawSeven(2,0,arrayX);
    // strip.show();
  } 
  // else {
  //   // turn LED off:
  //   // digitalWrite(ledPin, LOW);
  //   drawSeven(2,0,arrayBlank);
  //   // strip.show();
  // }

  showCursor(cursorX, cursorY);

  if (resetState == LOW) {
    // resetCanvas();
    ESP.restart();
  }

  strip.show();
  
}

// for tic tac toe 
void drawGrid() {
  uint16_t y, x;
  for (y=0; y<23; y++) {
    strip.setPixelColor(9,y,255,255,255);
    strip.setPixelColor(17,y,255,255,255);
    
    // strip.show();
  }
  for (x=2; x<25; x++) {
    strip.setPixelColor(x, 7, 255,255,255);
    strip.setPixelColor(x,15,255,255,255);

    // strip.show();
  }
}

// for tic tac toe 
void drawSeven (uint8_t x, uint8_t y, unsigned int arr[7][7]) {
  unsigned int color[3] = {0,0,0};
  if (arr[0][0] == 1) {
    color[1] = 255;
    color[2] = 255;
    color[3] = 0;
  }
  else {
    color[1] = 0;
    color[2] = 0;
    color[3] = 255;
  }

  for (uint8_t i=0; i<7; i++) {
    for (uint8_t j = 0; j<7; j++) {
      if (arr[i][j] == 1) {
        strip.setPixelColor(x+i,y+j, color[1],color[2],color[3]);
      }
      else {
        strip.setPixelColor(x+i,y+j,0,0,0);
      }
    }
  }
  // strip.show();
}

// cursor
void showCursor (uint8_t x, uint8_t y) {
  if (upState == LOW) {
    cursorY = y+speed;
    delay(200);
    // lastUpState = upState;
  }
  if (downState == LOW) {
    cursorY = y-speed;
    delay(200);
  }
  if (leftState == LOW) {
    cursorX = x+speed;
    delay(200);
  }
  if (rightState == LOW) {
    cursorX = x-speed;
    delay(200);
  }
  strip.setPixelColor(cursorX,cursorY,255,255,255);
  // strip.show();
}

void blankMatrix() {
  for (uint8_t i=0; i<27; i++) {
    for (uint8_t j=0; j<24; j++) {
      strip.setPixelColor(i,j,0,0,0);
    }
  }
}


// void drawX(int bottomRight) {
//   uint16_t y;
//   for (y=bottomRight; y<bottomRight+7; y++) {
//     strip.setPixelColor(y,y-1,255,0,0);
//   }
//   6
//   for (y=bottomRight; y<bottomRight+7; y++) {
//     strip.setPixelColor( ,y,)
//   }

// }
