#define buzzer 6                  //Alarm buzzer
#define heatPad 7                 //Pillow heater
#define photocell A0

#define timer_Button 11           //Button associated to the incremental increase of goalTime
#define start_Button 13           //Button associated to starting the timer and machine
#define photoCell_Button 10

bool startAlarm;                  //Bool that sets off the buzzers as alarms
bool beginTimer;                  //Bool that starts the functions of the napPillow
bool getTimeOnce = false;         //Bool that gets a number from millis() once
bool photoCell = true;

int heatTemp = 0;
int buzzerIntensity = 0;          //Intensity in whicht the buzzer vibrates at
int timeIncreaseInterval = 3000;  //Increases the alarmTime by X ammount

unsigned long currentTimer;       //CurrentTime is the number grabbed from millis() once
unsigned long goalTime = 3000;    //GoalTime is when the designated time to set off the alarm
unsigned long correctedTimer;     //CorrectedTime is the timer adjusted by millis()

int photoCellReading;

void setup() {
  Serial.begin(9600);

  pinMode(photoCell, INPUT);
  pinMode(heatPad, OUTPUT);        //Sets the heating pads as output
  pinMode(buzzer, OUTPUT);         //Sets the buzzer as output
  pinMode(timer_Button, INPUT);    //Sets goalTime button as input
  pinMode(start_Button, INPUT);    //Sets goalTime button as input
  pinMode(photoCell_Button, INPUT);

  startAlarm = false;              //Sets startAlarm by default to false - the timer will not start until this is set to true
}

void loop() {
  photoCellReading = analogRead(photocell);
  //Serial.print("analog reading = ");
  //Serial.println(photoCellReading);

  digitalWrite(heatPad, HIGH);
  //Function that checks if the timer should start
  deviceOn();
  photoCellOn();
  if (beginTimer == true) {
    getTime();                      // Gets the current Time
    alarmManager();                 // Decides if alaram is on or off
    startBuzz();                    // Sets the buzzer on
  } else if (beginTimer == false) {
    analogWrite(buzzer, 0);       //Sets buzzer to not be active
    getTimeOnce = false;            //Bool that allows time to get minis() only once
  }
  goalTimeManager();                // Allows for the increase in time before alarm goes off
}

//Condition that checks what state startAlarm is on.
void startBuzz() {
  //If startAlarm is true, the buzzer starts, otherwise it is off
  if (startAlarm == true && photoCell == true) {
    //Sets off alarm and intensity according to buzzerIntensity
    analogWrite(buzzer, photoCellReading * 4);
  } else if (startAlarm == true && photoCell == false) {
    analogWrite(buzzer, buzzerIntensity);
    //Incrementally adds to buzzerIntensity
    buzzerIntensity ++;
    Serial.println(buzzerIntensity);
    //Condition that checks if intensity has surpassed 210
    if (buzzerIntensity >= 210) {
      //Resets the intensity to 0
      buzzerIntensity = 0;
      //Executes buzzerReset after one second
      delay(1000);
    }
  }
}

//Gets the current time according to millis()
void getTime() {
  //Condition that checks if time should be gotten
  if (getTimeOnce == false) {
    Serial.println("hi");
    //Sets currentTimer to current value of millis()
    currentTimer = millis();
    //Makes it so currentTimer only gets millis() once
    getTimeOnce = true;
  }
  //Sets timer starting from 0 seconds according to millis() - currentTimer
  correctedTimer = millis() - currentTimer;
  //Serial.println(currentTimer);
}

//Condition that checks if currentTimer is larger or equal to goalTime
void alarmManager() {
  //if currentTimer is larger/equal, startAlarm is set to true, otherwise it is false
  if (millis() - currentTimer >= goalTime) {
    startAlarm = true;
  } else {
    startAlarm = false;
  }
}

//Condition that checks if the button has been pressed, increasing goalTime
void goalTimeManager() {
  //Sets the value or button to buttonVal.`
  int timerButtonVal = digitalRead(timer_Button);
  //If the button is pressed, 3 seconds is added to goalTimer
  if (timerButtonVal == true) {
    goalTime += timeIncreaseInterval;

    Serial.println("currentTimer : ");
    Serial.println(currentTimer);
    Serial.println("goalTime : ");
    Serial.println(goalTime);

    //Delay set to control goalTime incremental increase
    delay(1000);
  }
}

void deviceOn() {
  int startButtonVal = digitalRead(start_Button);
  if (startButtonVal == true) {
    beginTimer = !beginTimer;
    Serial.println(beginTimer);
    delay(1000);
  }
}

void photoCellOn() {
  int photoButtonVal = digitalRead(photoCell_Button);
  if (photoButtonVal == true) {
    photoCell = !photoCell;
    Serial.println("woah");
    delay(1000);
  }
}

