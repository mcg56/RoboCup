//         navigation.cpp       
//************************************

 // This file contains functions used to return to and
 // detect bases

#include "navigation.h"
#include "Arduino.h"
#include "motors.h"
#include "sensors.h"

// Local definitions
#define ROTATION_OVERSHOOT_BUFFER 15
#define DEG_IN_CIRCLE 360
#define SMALL_ANGLE_INCREMENT 5
#define LARGE_ANGLE_INCREMENT 20

bool cw_rotation_flag = 0;
bool ccw_rotation_flag = 0;



void check_rotation(float target_head, bool rollover_occurred) // TO FIX: Need to pass in angle increment and use that to check for overflow of 0-360 or 360-0
{
  if (cw_rotation_flag)
  {
    if (target_head > (DEG_IN_CIRCLE - ROTATION_OVERSHOOT_BUFFER))
    {
      float upper_lim = ROTATION_OVERSHOOT_BUFFER - (DEG_IN_CIRCLE - target_head);
      if ((sEulAnalog.head > target_head) || (sEulAnalog.head < upper_lim))
      {
        cw_rotation_flag = 0;
      }
    } else if ((rollover_occurred)&& (sEulAnalog.head > target_head) && (sEulAnalog.head < DEG_IN_CIRCLE/2))
    {
      cw_rotation_flag = 0;
    } else if ((!rollover_occurred) && (sEulAnalog.head > target_head))
    {
      cw_rotation_flag = 0;
    }
  }else if (ccw_rotation_flag)
  {
    if (target_head < ROTATION_OVERSHOOT_BUFFER)
    {
      float lower_lim = DEG_IN_CIRCLE - (ROTATION_OVERSHOOT_BUFFER - target_head);
      if ((sEulAnalog.head < target_head) || (sEulAnalog.head > lower_lim))
      {
        ccw_rotation_flag = 0;
      }
    } else if ((rollover_occurred) && (sEulAnalog.head < target_head) && (sEulAnalog.head > DEG_IN_CIRCLE/2))
    {
      ccw_rotation_flag = 0;
    } else if ((!rollover_occurred) && (sEulAnalog.head < target_head))
    {
      ccw_rotation_flag = 0;
    }
  }
}

void wall_detection()
{
  static float target_head;
  //Serial3.println(target_head);
  static int count = 0;
  static bool rollover_occurred = 0;
  /*if (cw_rotation_flag || ccw_rotation_flag) 
  {
    Serial3.println("1");
    check_rotation(target_head, rollover_occurred);
  } else
  */
  {
    if (rangeMedIR_2 < 300)
    {
      Serial3.println("2");
      cw_rotation_flag = 1;
      rollover_occurred = 0;
      leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
      rightMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
      target_head = sEulAnalog.head + SMALL_ANGLE_INCREMENT;
      if (target_head > DEG_IN_CIRCLE - 1)
      {
        target_head = target_head - DEG_IN_CIRCLE;
        rollover_occurred = 1;
      }
      
    }
    else if (rangeMedIR_3 < 300)
    {
      Serial3.println("3");
      ccw_rotation_flag = 1;
      rollover_occurred = 0;
      leftMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
      rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
      target_head = sEulAnalog.head - SMALL_ANGLE_INCREMENT;
      
      if (target_head < 0)
      {
        target_head = DEG_IN_CIRCLE + target_head; //CHECK WORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        rollover_occurred = 1;
      }
    }
    /*else if (rangeLongIR_4 < 300)
    {
      count += 1;
      if (count %2 == 0)
      {
        cw_rotation_flag = 1;
        rollover_occurred = 0;
        leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        rightMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
        target_head = sEulAnalog.head + LARGE_ANGLE_INCREMENT;
        if (target_head > DEG_IN_CIRCLE - 1)
        {
          target_head = target_head - DEG_IN_CIRCLE;
          rollover_occurred = 1;
        }
      } else
      {
        ccw_rotation_flag = 1;
        rollover_occurred = 0;
        leftMotorSpeed = MOTOR_REVERSE_TEST_SPEED;
        rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
        target_head = sEulAnalog.head - LARGE_ANGLE_INCREMENT;
        if (target_head < 0)
        {
          target_head = DEG_IN_CIRCLE + target_head; //CHECK WORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          rollover_occurred = 1;
        }
      }
      
    }*/else {
      Serial3.println("4");
      rightMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
      leftMotorSpeed = MOTOR_FORWARD_TEST_SPEED;
    }
  }
  
  /*Serial.print("cw flag ");
  Serial.print(cw_rotation_flag);
  Serial.print("  ");
  Serial.print("ccw flag ");
  Serial.println(ccw_rotation_flag);*/

}
