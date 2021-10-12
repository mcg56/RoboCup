/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "Arduino.h"
#include "sensors.h"
#include "motors.h"
#include "navigation.h"
#include "watchdog.h"

//Buffer definitions
#define WEIGHT_DETECTION_BUFFER 8
#define WEIGHT_DETECT_TOLERANCE 250
#define WEIGHT_SCAN_MIN_COUNT 8

Servo servo_big;      // create servo object to control a servo
bool turning_to_weight = 0;
bool weight_detected = 0;
bool failed_collect_weight_flag = 0;
int weight_detected_count = 0;

void weight_scan(/* whatever parameters */) 
{
  static int right_count = 0;
  static int left_count = 0;
  static bool cw_rotation_flag = 0;
  static bool ccw_rotation_flag = 0;
  
  if (((rangeLongIR_4-(rangeMedIR_2 - 80)) > 200) && (rangeMedIR_2 > 400))
    {
      right_count++;
      if (right_count >= WEIGHT_SCAN_MIN_COUNT)
      {
        cw_rotation_flag = 1;
        Serial3.println("Right Side Weight");
        leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        rightMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
        set_motor();
        right_count = 0;
        
        delay(150);
      }
    }
  else if (((rangeLongIR_5-(rangeMedIR_3 - 80)) > 200) && (rangeMedIR_3 > 400))
    {
      left_count++;
      if (left_count >= WEIGHT_SCAN_MIN_COUNT)
      {
        ccw_rotation_flag = 1;
        Serial3.println("Left Side Weight");
        leftMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
        rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        set_motor();
        left_count = 0;
        delay(150);
      }
    }
  else
    {
      left_count = 0;
      right_count = 0;
      turning_to_weight = 0;
    }
}


void collect_weight()
{
  /* When ready, collect the weight */
   Serial.println("Collecting weight \n");
   servo_big.write(120);      // sets the servo position full speed forward
   delay(1000);                           // waits for the servo to get there 
   servo_big.write(0);
   delay(1000);
}

void open_scoop()
{
  servo_big.write(0);
  delay(1000);
}
