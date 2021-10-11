
/************************************
 *        weight_collection.h       *
 *************************************/

 /* This header is for functions and tasks for
  *  finding and collecting weights  */

#ifndef WEIGHT_COLLECTION_H_
#define WEIGHT_COLLECTION_H_

#include <Servo.h>                  //control the DC motors
#include <Wire.h>                   //for I2C and SPI
#include "sensors.h"               //will need sensor library to detect weights

extern bool turning_to_weight;
extern bool weight_detected;
extern bool failed_collect_weight_flag;
extern int weight_detected_count;

void weight_scan(/* whatever parameters */);

void collect_weight();

void open_scoop(); //Resets gate to home position

#endif /* WEIGHT_COLLECTION_H_ */
