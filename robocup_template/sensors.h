//************************************
//         sensors.h     
//************************************

#ifndef SENSORS_H_
#define SENSORS_H_
#define SHORTIR_0_ADDRESS A4
#define SHORTIR_1_ADDRESS A5
#define MEDIUMIR_2_ADDRESS A7
#define MEDIUMIR_3_ADDRESS A3
#define LONGIR_4_ADDRESS A6
#define LONGIR_5_ADDRESS A2
#define ULTRASOUND_A_TRIG_PIN 31
#define ULTRASOUND_A_ECHO_PIN 30
#define DIG_IR_0_PIN 38
#define BASE_INPUT_SWITCH_PIN 43

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
extern bool dig_IR_0;

extern uint16_t clear, red, green, blue;

typedef DFRobot_BNO055_IIC    BNO;    // ******** use abbreviations instead of full names ********
extern BNO   bno;    // input TwoWire interface and IIC address
extern float g_current_heading_euler;
extern float g_current_pitch_euler;
extern float g_current_roll_euler;

//TIMING CODE
extern unsigned long duration;
extern bool newdurationflag;

// Read ultrasonic value
void read_ultrasonic(/* Parameters */);

// Read infrared value
void read_infrared(/* Parameters */);

void read_colour(/* Parameters */);
void init_colour_sensor();

// Pass in data and average the lot
void sensor_average(/* Parameters */);


float IR_to_range(int sensorNum, int filtered_IR);

void read_imu();
void imu_init();

#endif /* SENSORS_H_ */
