#include <Arduino_FreeRTOS.h>

int stepFWD = 11;
int stepREV = 10;

int stepPin = 9;
int dirPin = 8;

int carDetect = 2;
int startWash = 3;
int humanDetect = 4;

int greenLED = 7;
int blueLED = 6;
int redLED = 5;

int BuzzerPin = 13;
int relayPin = 12;

int speed = 1;

int fwdRotation = 0;
int revRotation = 0;

void Task_StepperMotor(void *param);
void Task_InputButton(void *param);

TaskHandle_t StepperMotor;
TaskHandle_t InputButton;

void forwardRotation();
void reverseRotation();

void setup() {
  
  Serial.begin(115200);

  pinMode(stepFWD, INPUT);
  pinMode(stepREV, INPUT);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(carDetect, INPUT);
  pinMode(startWash, INPUT);
  pinMode(humanDetect, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(relayPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  digitalWrite(BuzzerPin, LOW);
  digitalWrite(relayPin, LOW);

  //xTaskCreate(Task, "Task_Name", Stack_Memory, Parameter, Priority, &Task_Handle)

  xTaskCreate(Task_StepperMotor, "Task_1", 100, NULL, 1, &StepperMotor);
  xTaskCreate(Task_InputButton, "Task_2", 100, NULL, 1, &InputButton);
}

void loop() {}

void Task_StepperMotor(void *param) {
  for(;;) {
    // Stepper Motor Code

    //Serial.println();
    if(digitalRead(blueLED) == 1) {
      
      //for(int i=0; i<=1; i++) {
        do {
          forwardRotation();
          digitalWrite(relayPin, HIGH);
        } while((digitalRead(stepREV) != 1)&&(digitalRead(blueLED)!=0));
        digitalWrite(relayPin, LOW);
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        do {
          reverseRotation();
          digitalWrite(relayPin, HIGH);
        } while((digitalRead(stepFWD) != 1)&&(digitalRead(blueLED)!=0));
        digitalWrite(relayPin, LOW);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      //}
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      digitalWrite(BuzzerPin, HIGH);
      digitalWrite(blueLED, LOW);
    }
  }
}

void Task_InputButton(void *param) {
  for(;;) {
    // Button Code

    if(digitalRead(carDetect) == 1) {
      digitalWrite(greenLED, HIGH);
    }   

    if( digitalRead(greenLED) == 1) {
      do {
       
        if(digitalRead(startWash) == 1){
          digitalWrite(blueLED, HIGH);
        }
            
        if(digitalRead(humanDetect) == 1) {
          digitalWrite(BuzzerPin,HIGH);
          digitalWrite(greenLED, LOW);
          digitalWrite(blueLED, LOW);
          digitalWrite(redLED, HIGH);
           do {
              reverseRotation();
            } while((digitalRead(stepFWD) != 1));
        }
      } while((digitalRead(greenLED) == 1) && (digitalRead(humanDetect) != 1));   
    }

    while(digitalRead(redLED) == 1) {
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, LOW);
    }
  }
}

void forwardRotation() {
  // Stepper Motor Forward Rotation

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delay(speed);
  digitalWrite(stepPin, LOW);
  delay(speed);
}

void reverseRotation() {
  // Stepper Motor Reverse Rotation

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delay(speed);
  digitalWrite(stepPin, LOW);
  delay(speed);
}
