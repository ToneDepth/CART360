//Variable for the the average axis value for x,y, and z
unsigned long averageCalibrationX, averageCalibrationY, averageCalibrationZ;
//Variable that establishes which position is being calibrated
int calibrationRound = 0;

//Function that calibrates hand gestures from the accelerometer
void axisCalibration() {
  //Amount of values to establish the average
  int leength = 50;
  if (calibrationRound == 0) {
    //Light cue showing that values are being averaged
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    Serial.println("Resting Position");

    //Loop that takes all the values and makes a sum out of them
    for (int i = 0; i < leength; i++) {
      averageCalibrationX += analogRead(xpin);
      averageCalibrationY += analogRead(ypin);
      averageCalibrationZ += analogRead(zpin);
      delay(100);
    }
    //take the sum values and averages them out
    restingPosition[0] = averageCalibrationX / leength + 1;
    restingPosition[1] = averageCalibrationY / leength + 1;
    restingPosition[2] = averageCalibrationZ / leength + 1;

    //Prints the x, y, and z value for visual confirmation
    Serial.print(restingPosition[0]);
    Serial.print("\t");
    Serial.print(restingPosition[1]);
    Serial.print("\t");
    Serial.print(restingPosition[2]);
    Serial.println();

    //Resets the averages for the next round
    averageCalibrationX = 0;
    averageCalibrationY = 0;
    averageCalibrationZ = 0;

    //Visual cue showing that values are no longer being taken in
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    //pauses for 1 second
    delay(1000);

    //Adds to the variable allowing for the process to continue
    calibrationRound++;
  } else if (calibrationRound == 1) {
    //Light cue showing that values are being averaged
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
    Serial.println("Right Turn Position");

    //Loop that takes all the values and makes a sum out of them
    for (int i = 0; i < leength; i++) {
      averageCalibrationX += analogRead(xpin);
      averageCalibrationY += analogRead(ypin);
      averageCalibrationZ += analogRead(zpin);
      delay(100);
    }

    //take the sum values and averages them out
    leftRightTurn[0] = averageCalibrationX / leength + 1;
    leftRightTurn[1] = averageCalibrationY / leength + 1;
    leftRightTurn[2] = averageCalibrationZ / leength + 1;

    //Prints the x, y, and z value for visual confirmation
    Serial.print(leftRightTurn[0]);
    Serial.print("\t");
    Serial.print(leftRightTurn[1]);
    Serial.print("\t");
    Serial.print(leftRightTurn[2]);
    Serial.println();

    //Resets the averages for the next round
    averageCalibrationX = 0;
    averageCalibrationY = 0;
    averageCalibrationZ = 0;

    //Visual cue showing that values are no longer being taken in
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    delay(1000);

    //Adds to the variable allowing for the process to continue
    calibrationRound++;
  }  else if (calibrationRound == 2) {
    //Light cue showing that values are being averaged
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, HIGH);
    Serial.println("Left Turn Position");

    //Loop that takes all the values and makes a sum out of them
    for (int i = 0; i < leength; i++) {
      averageCalibrationX += analogRead(xpin);
      averageCalibrationY += analogRead(ypin);
      averageCalibrationZ += analogRead(zpin);
      delay(100);
    }

    //take the sum values and averages them out
    leftLeftTurn[0] = averageCalibrationX / leength + 1;
    leftLeftTurn[1] = averageCalibrationY / leength + 1;
    leftLeftTurn[2] = averageCalibrationZ / leength + 1;

    //Prints the x, y, and z value for visual confirmation
    Serial.print(leftLeftTurn[0]);
    Serial.print("\t");
    Serial.print(leftLeftTurn[1]);
    Serial.print("\t");
    Serial.print(leftLeftTurn[2]);
    Serial.println();

    //Resets the averages for the next round
    averageCalibrationX = 0;
    averageCalibrationY = 0;
    averageCalibrationZ = 0;

    //Visual cue showing that values are no longer being taken in
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    delay(1000);

    //Adds to the variable allowing for the process to continue
    calibrationRound++;
  } else if (calibrationRound == 3) {
    //Light cue showing that values are being averaged
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
    Serial.println("Stop Position");

    //Loop that takes all the values and makes a sum out of them
    for (int i = 0; i < leength; i++) {
      averageCalibrationX += analogRead(xpin);
      averageCalibrationY += analogRead(ypin);
      averageCalibrationZ += analogRead(zpin);
      delay(100);
    }

    //take the sum values and averages them out
    leftSlow[0] = averageCalibrationX / leength + 1;
    leftSlow[1] = averageCalibrationY / leength + 1;
    leftSlow[2] = averageCalibrationZ / leength + 1;

    //Prints the x, y, and z value for visual confirmation
    Serial.print(leftSlow[0]);
    Serial.print("\t");
    Serial.print(leftSlow[1]);
    Serial.print("\t");
    Serial.print(leftSlow[2]);
    Serial.println();

    //Resets the averages for the next round
    averageCalibrationX = 0;
    averageCalibrationY = 0;
    averageCalibrationZ = 0;

    //Adds to the variable allowing for the process to continue
    calibrationRound++;
    //Bool that establishes that calibration is over, allowing blinkers to function
    startCalibration = false;
  } else {
    //Visual cue showing that values are no longer being taken in
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
  }
}

