//************************************
//         sensors.h     
//************************************

#ifndef SENSORS_H_
#define SENSORS_H_
#define SHORTIR_0_ADDRESS A0
#define SHORTIR_1_ADDRESS A1
#define MEDIUMIR_2_ADDRESS A2
#define MEDIUMIR_3_ADDRESS A3
#define LONGIR_4_ADDRESS A4
#define LONGIR_5_ADDRESS A5
#define ULTRASOUND_A_TRIG_PIN 31
#define ULTRASOUND_A_ECHO_PIN 30
#define DIG_IR_0_PIN 42

#include "Arduino.h"
#include "DFRobot_BNO055.h"
//************************************
//Globals  
//************************************
extern float rangeShortIR_0;
extern float rangeShortIR_1;
extern float rangeMedIR_2;
extern float rangeMedIR_3;
extern float rangeLongIR_4;
extern float rangeLongIR_5;
extern long rangeUltrasonic_A;
extern bool dig_IR_0;

typedef DFRobot_BNO055_IIC    BNO;    // ******** use abbreviations instead of full names ********
extern BNO   bno;    // input TwoWire interface and IIC address
extern BNO::sEulAnalog_t    sEulAnalog;

// Read ultrasonic value
void read_ultrasonic(/* Parameters */);

// Read infrared value
void read_infrared(/* Parameters */);

void read_colour(/* Parameters */);

// Pass in data and average the lot
void sensor_average(/* Parameters */);


float IR_to_range(int sensorNum, int filtered_IR);

void read_imu();
void imu_init();

#endif /* SENSORS_H_ */
