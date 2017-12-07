//Variables for the accelerometer
const int groundpin = 18;             // ground
const int powerpin = 19;              // voltage
const int xpin = A3;                  // x-axis
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis

//Inclusion of the adafruit neopixel library
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//Decleration for two different strip light possibitlies
#define PIN 6
#define PIN2 5

//Declaration for two different strip light possibiltiies
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(29, PIN2, NEO_GRB + NEO_KHZ800);

//Declares pins for RGB light
#define LED_R 12
#define LED_G 11
#define LED_B 10
//Declare pin for callibration button
#define calibrationButton 2

//Arrays for the different hand gestures to trigger light blinkers
int restingPosition[3];
int leftRightTurn[3], leftLeftTurn[3], leftSlow[3];
int rightRightTurn[3], rightLeftTurn[3], rightSlow[3];

//Pirmary variable to get raw data from accelerometer
int xAxis, yAxis, zAxis;

//Maximum ammount for the average getter
const int numReadings = 11;
//Variable for where in the avarege index the reader is at
int readIndex = 0;
//Variable for recording rhe total amount for each axis recorded
int totalX = 0, totalY = 0, totalZ = 0;
//Array for storing values to create an average
int storedX[numReadings], storedY[numReadings], storedZ[numReadings];
//average value for each axis
int averageX, averageY, averageZ;

//Bool waiting for button to be pressed
bool startCalibration = false;
//Margin of error for detecting a trigger for a light blinker
int marginError = 30;

//Variables for recording time
unsigned long currentTime, timePassed;
bool timeAchieved = false;
bool resetTime = false;

void setup() {
  Serial.begin(9600);

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  //End of trinket special code

  //Starts the LED in a off setting
  strip.begin();
  strip.show();

  //Sets the ground and power pin as outpit
  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);

  //Sets the button as input
  pinMode(calibrationButton, INPUT);

  //Sets ground to low and power to high for current control
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);

  //Sets each level of rgb led as output
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() {
  //Sets x,y,z axis to the problem acceleromete pins
  xAxis = analogRead(xpin);
  yAxis = analogRead(ypin);
  zAxis = analogRead(zpin);

  //Test function to test led light functionality
  //justRed(strip.Color(255, 0, 0), 1000); // yellow

  //stopLight(strip.Color(255, 0, 0), 1000); // yellow
  //stopLight(strip.Color(0, 0, 0), 1000); // yellow

  //colorWipe(strip.Color(200, 255, 20), 5); // yellow
  //colorWipe(strip.Color(0, 0, 0), 5); // yellow

  //colorWipe(strip2.Color(235, 255, 20), 5); // yellow
  //colorWipe(strip2.Color(0, 0, 0), 5); // yellow

  //alteredTurn(strip.Color(200, 255, 20), 5); // yellow
  //alteredTurn(strip.Color(0, 0, 0), 5); // yellow

  //Functions that get the average axis and comntrols the input
  averageAxis();
  inputManager();

  //condition that only allows button calibration, after blinkers can be initiated
  if (startCalibration == true) {
    axisCalibration();
  } else {
    activateLight();
  }

  //print the sensor values:
  Serial.print(averageX);
  //print a tab between values:
  Serial.print("\t");
  Serial.print(averageY);
  //print a tab between values:
  Serial.print("\t");
  Serial.print(averageZ);
  Serial.println();
  //delay before next reading:
  //delay(1000);
}

//Function that gets an overall average of each axis to avoid twittering functions
void averageAxis() {
  //Takes the current index to array and substracts it from its associated total
  totalX = totalX - storedX[readIndex];
  totalY = totalY - storedY[readIndex];
  totalZ = totalZ - storedZ[readIndex];

  //Sets current index to array as a value from the accelerometer
  storedX[readIndex] = analogRead(xpin);
  storedY[readIndex] = analogRead(ypin);
  storedZ[readIndex] = analogRead(zpin);

  //Takes the total and adds the current index array value
  totalX = totalX + storedX[readIndex];
  totalY = totalY + storedY[readIndex];
  totalZ = totalZ + storedZ[readIndex];

  //Adds to the index
  readIndex = readIndex + 1;
  //Condition that sets index to zero once it reaches its maximum allowed established by numReadings
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  //Sets the average by taking the total and dividing by numReadings
  averageX = totalX / numReadings;
  averageY = totalY / numReadings;
  averageZ = totalZ / numReadings;

  //Prototype average getter, but the nature of a for loop made it slow and clunky
  /*for (int i = 0; i <= 20; i++) {
    storedX[i] = analogRead(xpin);
    storedY[i] = analogRead(ypin);
    storedZ[i] = analogRead(zpin);

    averageX = (storedX[0] + storedX[1] + storedX[2] + storedX[3] + storedX[4] + storedX[5] + storedX[6] + storedX[7]
                + storedX[8] + storedX[9] + storedX[10] + storedX[11] + storedX[12] + storedX[13] + storedX[14] + storedX[15]
                + storedX[16] + storedX[17] + storedX[18] + storedX[19]) / 19;
    averageY = (storedY[0] + storedY[1] + storedY[2] + storedY[3] + storedY[4] + storedY[5] + storedY[6] + storedY[7]
                + storedY[8] + storedY[9] + storedY[10] + storedY[11] + storedY[12] + storedY[13] + storedY[14] + storedY[15]
                + storedY[16] + storedY[17] + storedY[18] + storedY[19]) / 19;
    averageZ = (storedZ[0] + storedZ[1] + storedZ[2] + storedZ[3] + storedZ[4] + storedZ[5] + storedZ[6] + storedZ[7]
                + storedZ[8] + storedZ[9] + storedZ[10] + storedZ[11] + storedZ[12] + storedZ[13] + storedZ[14] + storedZ[15]
                + storedZ[16] + storedZ[17] + storedZ[18] + storedZ[19]) / 19;
    if (i >= 20) {
      i = 0;
      }
    }*/
}

//Function that activates blinkers depeneding on hand gestures established by axisCalibration
void activateLight() {
  //Coindition that checks that the accelerometer values are equal to axisCalibration
  if (averageX + marginError > leftRightTurn[0] && averageX - marginError < leftRightTurn[0] &&
      averageY + marginError > leftRightTurn[1] && averageY - marginError < leftRightTurn[1] &&
      averageZ + marginError > leftRightTurn[2] && averageZ - marginError < leftRightTurn[2]) {
    //Plays the alteredturn function
    alteredTurn(strip.Color(235, 255, 20), 5); // yellow
    alteredTurn(strip.Color(0, 0, 0), 5); // yellow
    //delay(1500);
    /*if (resetTime == false) {
      startTimer();
      resetTime = true;
      }
      if (timeAchieved == false) {
      checkTimer(1500);
      }*/
    //Coindition that checks that the accelerometer values are equal to axisCalibration
  } else if (averageX + marginError > leftLeftTurn[0] && averageX - marginError < leftLeftTurn[0] &&
             averageY + marginError > leftLeftTurn[1] && averageY - marginError < leftLeftTurn[1] &&
             averageZ + marginError > leftLeftTurn[2] && averageZ - marginError < leftLeftTurn[2]) {
    //Plays the colorWipe function
    colorWipe(strip.Color(235, 255, 220), 10); // yellow
    colorWipe(strip.Color(0, 0, 0), 10); // yellow
    //delay(1500);
    //Coindition that checks that the accelerometer values are equal to axisCalibration
  } else if (averageX + marginError > leftSlow[0] && averageX - marginError < leftSlow[0] &&
             averageY + marginError > leftSlow[1] && averageY - marginError < leftSlow[1] &&
             averageZ + marginError > leftSlow[2] && averageZ - marginError < leftSlow[2]) {
    //Plays the stopLight function
    stopLight(strip.Color(255, 0, 0), 1000); // yellow
    stopLight(strip.Color(0, 0, 0), 1000); // yellow
    //delay(1500);
  } else {
    //Sets LED's to low
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    //delay(1500);
  }
}

//Function that starts a timer
void startTimer() {
  //Sets currentTime to the current value of millis()
  currentTime = millis();
  //Serial.println(currentTime);
}

//function that checks the timer and compared it to targetTime.
void checkTimer(int targetTime) {
  //Sets timePassed according to millis() minus currentTime
  timePassed = millis() - currentTime;
  Serial.println(timePassed);
  if (timePassed > targetTime) {
    Serial.println("achieved");
    //Sets currentTime to millis to reset values
    currentTime = millis();
    //Sets timeAchieved to true to signifie the value has reached its goal.
    timeAchieved = true;
  }
}

//Function that allows the button to control values
void inputManager() {
  digitalWrite(calibrationButton, HIGH);
  //If the button is pressed calibration will start
  if (digitalRead(calibrationButton) == LOW) {
    startCalibration = true;
    //Serial.println("HI");
    delay(1000);
  }
}

// Fill the LEDs one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    //averageAxis();
    //Condition that checks the value of i
    if (i > - 59) {
      colorWipe(strip.Color(210, 0, 20), 15); // yellow
      //averageAxis();
    }
    //Sets each LED to the correct color value
    strip.setPixelColor(i, c);
    //Acitvates the LED
    strip.show();
    //Sets delay established in the function line
    delay(wait);
  }
}

// Fill the LEDs one after the other with a color in reverse
void alteredTurn(uint32_t c, uint8_t wait) {
  {
    //Loop that progressively turns on each LED on the street.
    for (uint16_t i = strip.numPixels(); i > 1; i--) {
      //averageAxis();
      //Sets each LED to the correct color value
      strip.setPixelColor(i, c);
      //Acitvates the LED
      strip.show();
      //Sets delay established in the function line
      delay(wait);
    }
  }
}

// Fill the dots one after the other with a color
void stopLight(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    averageAxis();
    //Sets each LED to the correct color value
    strip.setPixelColor(i, c);
    //Acitvates the LED
    strip.show();
  }
  //Sets delay established in the function line
  delay(wait);
}

// Fill the dots one after the other with a color
void justRed(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    //Sets each LED to the correct color value
    strip.setPixelColor(i, c);
    //Acitvates the LED
    strip.show();
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheell(byte WheelPoss) {
  WheelPoss = 255 - WheelPoss;
  if (WheelPoss < 85) {
    return strip2.Color(255 - WheelPoss * 3, 0, WheelPoss * 3);
  }
  if (WheelPoss < 170) {
    WheelPoss -= 85;
    return strip2.Color(0, WheelPoss * 3, 255 - WheelPoss * 3);
  }
  WheelPoss -= 170;
  return strip2.Color(WheelPoss * 3, 255 - WheelPoss * 3, 0);
}


