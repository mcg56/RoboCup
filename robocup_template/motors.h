//************************************
//         motors.h    
//************************************

#ifndef MOTORS_H_
#define MOTORS_H_

#include <Servo.h>

// SET THIS TO REAL VALUES
#define RIGHT_MOTOR_ADDRESS 3      //Pin corresponding to the left dc motor
#define LEFT_MOTOR_ADDRESS 2     //Pin corresponding to the right dc motor
#define MIN_SPEED_CAP 1           //Set the minimum speed value that can be written to the motors
#define MAX_SPEED_CAP 1           //Set the maximum speed value that can be written to the motors
#define MOTOR_STOP_SPEED 1500    // Down to 1000 reverse, 2000 forward
#define MOTOR_FORWARD_MAX_SPEED 2000
#define MOTOR_REVERSE_MAX_SPEED 1000
#define MOTOR_FORWARD_TEST_SPEED 1900
#define MOTOR_REVERSE_TEST_SPEED 1100



//************************************
//Globals  
//************************************
extern int leftMotorSpeed;
extern int rightMotorSpeed;
extern Servo right_motor;
extern Servo left_motor;


void check_speed_limits(/*parameters*/);
void set_motor(/*parameters*/);


#endif /* MOTORS_H_ */
