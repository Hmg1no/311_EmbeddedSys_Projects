#define PIN_LED1 2  // LED #1
#define PIN_LED2 3  // LED #2
#ifndef LOW
#define LOW  0
#endif
#ifndef HIGH
#define HIGH 1
#endif


// unsigned long for time
unsigned long prevLed1Ms = 0;
unsigned long prevLed2Ms = 0;

// function prototyping
void task_LED1();
void task_LED2();
void task_SerialCheck();

// User Input Variables
int ledSelection = 0; // Waiting for which LED to change
unsigned long currentTime = 0; // Just declare it here

// 0 = waiting for LED number
// 1 = waiting for interval
int inputState = 0;   


// Store Led states
int led1State = LOW;
int led2State = LOW;

// Default intervals 
long led1Interval = 1000;
long led2Interval = 500;

void setup() {
  Serial.begin(9600); //set baud rate 
  pinMode(PIN_LED1, OUTPUT); //define pins
  pinMode(PIN_LED2, OUTPUT);

  digitalWrite(PIN_LED1, LOW); //define starting states
  digitalWrite(PIN_LED2, LOW);

  Serial.println("Which LED do you want to change (1 or 2)?");
}



void task_LED1(){


   // LED 1 control 
  if (currentTime - prevLed1Ms >= led1Interval) {
    prevLed1Ms = currentTime;
    
    
    if (led1State == LOW) {
      led1State = HIGH;
    } else {
      led1State = LOW;
    }

    digitalWrite(PIN_LED1, led1State); //change the states for led 1 
  }

}

void task_LED2(){

  //LED 2 control 
  if (currentTime - prevLed2Ms >= led2Interval) {
    prevLed2Ms = currentTime;
    
    if (led2State == LOW) {
      led2State = HIGH;
    } else {
      led2State = LOW;
    }

    digitalWrite(PIN_LED2, led2State); //change the states for led 2 
  }

}


void task_SerialCheck(){
  
  // Asynchronously handle serial input
  if (Serial.available() > 0) {
    switch (inputState) {
      case 0: { // Waiting for user to select an LED
        int ledNum = Serial.parseInt();
        
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (ledNum == 1 || ledNum == 2) {
          ledSelection = ledNum; // Store the chosen LED
          Serial.println("What interval (in ms)?");
          inputState = 1; // Move to the next state
        }
        break;
      }
      case 1: { // Wait for the interval
        long newInterval = Serial.parseInt();
        
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (newInterval > 0) {
          if (ledSelection == 1) {
            led1Interval = newInterval;
          } else if (ledSelection == 2) {
            led2Interval = newInterval;
          }
          Serial.println(" nterval has been set.");
          Serial.println("-------------------------------------");
          Serial.println("Which LED do you want to change (1 or 2)?");
          inputState = 0; // Reset for new led selection
        }
        break;
      }
    }
  }


}

void (*taskList[])() = { 
  task_LED1,
  task_LED2,
  task_SerialCheck
};


void loop() {

  // Get the current time at the start of the loop
  currentTime = millis();

  for( int i = 0; i < 3; i++){
    taskList[i]();
  }

}