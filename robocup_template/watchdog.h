/************************************
 *        watchdog.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  watchdog timers to catch errors  */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#define RED_LED_PIN 26
#define YELLOW_LED_PIN 27
#define GREEN_LED_PIN 28

extern bool wall_detected_flag;
extern bool no_walls_detected_timeout;
extern bool robot_tilting_flag;
extern bool weight_scan_disabled;
extern bool weight_detected;

void watchdog_timers_init();
void check_watchdog();
#endif
