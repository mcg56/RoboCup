/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "Arduino.h"
#include "sensors.h"
#include "motors.h"

int weight_found = 0; // TO DELETE
void weight_scan(/* whatever parameters */) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
   Serial.println("Looking for weights \n");
   switch(weight_found) {
    case 0:
      leftMotorSpeed = 1700;
      rightMotorSpeed = 1300;
        if (((rangeLongIR_4 < 1000)&& ((rangeMedIR_2 - rangeLongIR_4) > 100))||((rangeLongIR_5 < 1000) && ((rangeMedIR_2 - rangeLongIR_5) > 100))){
          weight_found = 1;
    case 1:
      leftMotorSpeed = 1800;
      rightMotorSpeed = 1800;
      }
   }
}


void collect_weight()
{
  /* When ready, collect the weight */
   Serial.println("Collecting weight \n");
}
