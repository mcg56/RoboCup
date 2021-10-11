#include "motors.h"
#include "Arduino.h"

#include <Servo.h>

int leftMotorSpeed;
int rightMotorSpeed;
Servo right_motor;
Servo left_motor;




void set_motor(/*parameters*/) {

  //Serial.println("Change the motor speed \n");
  right_motor.writeMicroseconds(leftMotorSpeed);      //Stop 
  left_motor.writeMicroseconds(rightMotorSpeed);      // 


}
