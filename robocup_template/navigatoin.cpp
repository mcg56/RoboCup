//         navigation.cpp       
//************************************

 // This file contains functions used to return to and
 // detect bases

#include "navigation.h"
#include "Arduino.h"
#include "motors.h"
#include "sensors.h"
#include "watchdog.h"
#include "weight_collection.h"

// Local definitions
#define CORNER_IR_LIMIT 200

void wall_detection()
{ 

  static int count = 0;
  static bool cw_rotation_flag = 0;
  static bool ccw_rotation_flag = 0;
  static int opposite_turn_count = 0;
  static bool previous_cw_turn = 0;
  static bool previous_ccw_turn = 0;
  if (turning_to_weight || weight_scan_disabled)
  {
    cw_rotation_flag = 0;
    ccw_rotation_flag = 0;    
  } 
  else
  {
    /*if (cw_rotation_flag || ccw_rotation_flag) 
    {
      if (cw_rotation_flag)
      {
        previous_cw_turn = 1;
      } 
      else
      {
        previous_ccw_turn = 1;
      }
    }
    
    {*/
      if (((rangeShortIR_0 < CORNER_IR_LIMIT) || (rangeLongIR_5) < 225)) //((rangeShortIR_0 < CORNER_IR_LIMIT) || (rangeLongIR_5) < 225))
      {
        wall_detected_flag = 1;
        Serial3.println("object left");
        cw_rotation_flag = 1;
        leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        rightMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
      }
      else if (((rangeShortIR_1 < CORNER_IR_LIMIT) || (rangeLongIR_4) < 225))  //(((rangeShortIR_1 < CORNER_IR_LIMIT) || (rangeLongIR_4) < 225))
      {
        wall_detected_flag = 1;
        Serial3.println("object right");
        ccw_rotation_flag = 1;
        leftMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
        rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;   
      }

      else // Not sensing walls, go forward
      {
        Serial3.println("Clear");
        rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        previous_cw_turn = 0;
        previous_ccw_turn = 0;

      }
    
  }
}

  
