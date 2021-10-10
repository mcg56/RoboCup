/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "Arduino.h"
#include "sensors.h"
#include "motors.h"

Servo servo_big;      // create servo object to control a servo

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
   servo_big.write(120);      // sets the servo position full speed forward
   delay(1000);                           // waits for the servo to get there 
   servo_big.write(0);
   delay(1000);
}
