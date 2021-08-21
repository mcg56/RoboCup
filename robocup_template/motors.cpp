#include "motors.h"
#include "Arduino.h"

#include <Servo.h>

int leftMotorSpeed;
int rightMotorSpeed;
Servo right_motor;
Servo left_motor;



/* Check whether the speed value to be written is within the maximum
 *  and minimum speed caps. Act accordingly.
 *
 */
void check_speed_limits(/*parameters*/) {
  //Serial.println("Check the motor speed limit \n");
}


/* In this section, the motor speeds should be updated/written.
 *It is also a good idea to check whether value to write is valid.
 *It is also a good idea to do so atomically!
 */
void set_motor(/*parameters*/) {

  //Serial.println("Change the motor speed \n");
  right_motor.writeMicroseconds(leftMotorSpeed);      //Stop 
  left_motor.writeMicroseconds(rightMotorSpeed);      // 
  check_speed_limits();

}
