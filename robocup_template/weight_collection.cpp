/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "Arduino.h"
#include "sensors.h"
#include "motors.h"

// direction Digital 39 (CCW), pulses Digital 38 (CLK)
AccelStepper stepper(1, 38, 39);

#define GEAR_RATIO                            5.18  //Geared steppermotor ratio (typ. 400 step/rotation)
#define PULLEY_RATIO                          1     //Pully ratio for normal stepper
#define GEARED_STEPPER_STEP_COUNT             346   //Steps needed for 60 degree rotaion
#define STEPPER_STEP_COUNT                    67    //Steps needed for 60 degree rotaion

void stepper_init()
{
  stepper.setMaxSpeed(2000); //SPEED = Steps / second
  stepper.setAcceleration(1000); //ACCELERATION = Steps /(second)^2
  stepper.disableOutputs(); //disable outputs, so the motor is not getting warm (no current)
  Serial.println("Steppers Initialised");
}

void weight_scan(/* whatever parameters */) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
   Serial.println("Looking for weights \n");
}


void collect_weight()
{
  /* When ready, collect the weight */
   Serial.println("Collecting weight \n");
   static int state = 0;
   switch (state)
   {
     case 0:
      stepper.disableOutputs(); //disable outputs, so the motor is not getting warm (no current)
      if (dig_IR_0 == 0)
      {
        state = 1;
      }
      break;
    case 1:
      rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
      leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
  
      if (dig_IR_0 == 1)
      {
        stepper.setMaxSpeed(GEARED_STEPPER_STEP_COUNT); //set speed
        stepper.move(1000); //set distance
        stepper.enableOutputs(); //enable pins
        stepper.run(); //step the motor (this will step the motor by 1 step at each loop)
        state = 0;
      }
      break;
   }
}
