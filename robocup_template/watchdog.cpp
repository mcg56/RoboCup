/************************************
 *        watchdog.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  watchdog timers to catch errors  */


#include "watchdog.h"
#include "Arduino.h"
#include "sensors.h"
#include "weight_collection.h"
#include <arduino-timer.h>          //timer for round end

#define WALL_WATCHDOG_TIMER_PERIOD 1000
#define WEIGHT_SCAN_DISABLE_TIMER_PERIOD 500

// Local definitions
bool wall_detected_flag = 0;
bool no_walls_detected_timeout = 0;
bool robot_tilting_flag = 0;
bool weight_scan_disabled = 0;
auto wall_detect_timer = timer_create_default(); // Timer for checking if a wall has been sensed
auto weight_scan_disable_timer = timer_create_default(); // Timer for disabling weight scan for a short time after detecting weight


void wall_detect_watchdog();
void weight_scan_disable_watchdog();
void check_tilt();
void watchdog_timers_init()
{
  wall_detect_timer.every(WALL_WATCHDOG_TIMER_PERIOD, wall_detect_watchdog);
  weight_scan_disable_timer.every(WEIGHT_SCAN_DISABLE_TIMER_PERIOD, weight_scan_disable_watchdog);
  
}

void check_watchdog()
{
  wall_detect_timer.tick();
  weight_scan_disable_timer.tick();
  check_tilt();
}

void weight_scan_disable_watchdog()
{
  static int count = 0;
  
  
  if (weight_scan_disabled)
  {
    count--;
  }
  if(turning_to_weight)
  {
    weight_scan_disabled = 1;
    count = 6;
  }
  if (count == 0)
  {
    weight_scan_disabled = 0;
  }
  
}

void wall_detect_watchdog()
{
  static int count = 0;
  if(wall_detected_flag)
  {
    count = 0;
    wall_detected_flag = 0;
  } else if (weight_detected)
  {
     count = 0;
     weight_detected = 0;
  }
  else
  {
    count++;
    wall_detected_flag = 0;
    if (count > 5)
    {
      no_walls_detected_timeout = 1;
      count  = 0;
    }
  }
}

void check_tilt()
{
  if (abs(g_current_pitch_euler) + abs(g_current_roll_euler) >= 15)
    {
      Serial3.print("tilting");
      robot_tilting_flag = 1;
    }
}
