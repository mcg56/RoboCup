
/********************************************************************************
 *                               ROBOCUP TEMPLATE         MCG  MATAFAKA                   
 *        
 *  
 *  This is a template program design with modules for 
 *  different components of the robot, and a task scheduler
 *  for controlling how frequently tasks should run
 *  
 *  written by: Logan Chatfield, Ben Fortune, Lachlan McKenzie, Jake Campbell
 *  https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573
 *  
 *   
 *  CPU Load timing code is currently in read IMU
 *  
 *  TO DO:
 *    -Color sensor, dont pick up weights on their base
 *    -if we happen to be on our own base in last 20 seconds, stay there.
 *    
 *    
 *TO DO: DAY BEFORE COMPETITION
 *  -Ensure no serial prints
 *  -check battery solder connection
 *  -Check resoldered IR are turning on
 *  -Check tightness of bolts
 *  -check scoop clamps (tighten)
 *  -Check cables are clear of tracks
 *  -check everthing is enabled correctly in code
 ******************************************************************************/
 
#include <Servo.h>                  //control the DC motors
#include <Adafruit_TCS34725.h>      //colour sensor
#include <Wire.h>                   //for I2C and SPI
#include <TaskScheduler.h>          //scheduler 
#include <arduino-timer.h>          //timer for round end

// Custom headers
#include "motors.h"
#include "sensors.h"
#include "weight_collection.h"
#include "return_to_base.h" 
#include "navigation.h"
#include "watchdog.h"


//**********************************************************************************
// Local Definitions
//**********************************************************************************

// Task period Definitions
// ALL OF THESE VALUES WILL NEED TO BE SET TO SOMETHING USEFUL !!!!!!!!!!!!!!!!!!!!
#define US_READ_TASK_PERIOD                 80
#define IR_READ_TASK_PERIOD                 10
#define COLOUR_READ_TASK_PERIOD             40
#define SET_MOTOR_TASK_PERIOD               10
#define WEIGHT_SCAN_TASK_PERIOD             10
#define COLLECT_WEIGHT_TASK_PERIOD          40
#define RETURN_TO_BASE_TASK_PERIOD          40
#define DETECT_BASE_TASK_PERIOD             40
#define CHECK_WATCHDOG_TASK_PERIOD          40
#define WALL_DETECTION_TASK_PERIOD          10
#define IMU_READ_TASK_PERIOD                15




// Task execution amount definitions
// -1 means indefinitely
#define US_READ_TASK_NUM_EXECUTE           -1
#define IR_READ_TASK_NUM_EXECUTE           -1
#define COLOUR_READ_TASK_NUM_EXECUTE       -1
#define SET_MOTOR_TASK_NUM_EXECUTE         -1
#define WEIGHT_SCAN_TASK_NUM_EXECUTE       -1
#define COLLECT_WEIGHT_TASK_NUM_EXECUTE    -1
#define RETURN_TO_BASE_TASK_NUM_EXECUTE    -1
#define DETECT_BASE_TASK_NUM_EXECUTE       -1
#define UNLOAD_WEIGHTS_TASK_NUM_EXECUTE    -1
#define CHECK_WATCHDOG_TASK_NUM_EXECUTE    -1
#define WALL_DETECTION_TASK_NUM_EXCECUTE   -1
#define IMU_READ_TASK_NUM_EXCECUTE         -1

// Pin deffinitions
#define IO_POWER  49

// Serial deffinitions
#define BAUD_RATE 9600

// Timer definitions TO DO: DELETE
#define RETURN_HOME_TIMER_PERIOD 90000



//**********************************************************************************
// Task Scheduler and Tasks
//**********************************************************************************

/* The first value is the period, second is how many times it executes
   (-1 means indefinitely), third one is the callback function */

// Tasks for reading sensors 
Task tRead_infrared(IR_READ_TASK_PERIOD,           IR_READ_TASK_NUM_EXECUTE,          &read_infrared);
Task tRead_colour(COLOUR_READ_TASK_PERIOD,         COLOUR_READ_TASK_NUM_EXECUTE,      &read_colour);
Task tRead_imu(IMU_READ_TASK_PERIOD,               IMU_READ_TASK_NUM_EXCECUTE,        &read_imu);

// Task to set the motor speeds and direction
Task tSet_motor(SET_MOTOR_TASK_PERIOD,             SET_MOTOR_TASK_NUM_EXECUTE,        &set_motor);

// Tasks to scan for weights and collection upon detection
Task tWeight_scan(WEIGHT_SCAN_TASK_PERIOD,         WEIGHT_SCAN_TASK_NUM_EXECUTE,      &weight_scan);
Task tCollect_weight(COLLECT_WEIGHT_TASK_PERIOD,   COLLECT_WEIGHT_TASK_NUM_EXECUTE,   &collect_weight);

// Tasks to search for bases and unload weights
Task tReturn_to_base(RETURN_TO_BASE_TASK_PERIOD,   RETURN_TO_BASE_TASK_NUM_EXECUTE,   &return_to_base);
Task tDetect_base(DETECT_BASE_TASK_PERIOD,         DETECT_BASE_TASK_NUM_EXECUTE,      &detect_base);

// Tasks to check the 'watchdog' timer (These will need to be added in)
Task tCheck_watchdog(CHECK_WATCHDOG_TASK_PERIOD, CHECK_WATCHDOG_TASK_NUM_EXECUTE,   &check_watchdog);

// Tasks for navigation
Task tWall_detection(WALL_DETECTION_TASK_PERIOD,   WALL_DETECTION_TASK_NUM_EXCECUTE,  &wall_detection);

Scheduler taskManager;

//**********************************************************************************
// Function Definitions
//**********************************************************************************
void pin_init();
void robot_init();
void timer_init();
void task_init();

//**********************************************************************************
// put your setup code here, to run once:
//**********************************************************************************
void setup() {
  Serial.begin(BAUD_RATE);
  Serial3.begin(BAUD_RATE);
  pin_init();
  imu_init();
  robot_init();
  task_init();
  watchdog_timers_init(); 
  Wire.begin();
  
}


//**********************************************************************************
// Initialise the pins as inputs and outputs (otherwise, they won't work) 
// Set as high or low
//**********************************************************************************
void pin_init(){
    
    Serial.println("Pins have been initialised \n"); 

    pinMode(IO_POWER, OUTPUT);              //Pin 49 is used to enable IO power
    digitalWrite(IO_POWER, 1);              //Enable IO power on main CPU board
    pinMode(DIG_IR_0_PIN,INPUT);            //Digital IR detection
   
}

//**********************************************************************************
// Set default robot state
//**********************************************************************************
void robot_init() {
    right_motor.attach(LEFT_MOTOR_ADDRESS);  // attaches the servo pin 3 to the servo object LEFT_MOTOR_ADDRESS
    left_motor.attach(RIGHT_MOTOR_ADDRESS);  // attaches the servo pin 2 to the servo object
    servo_big.attach(SERVO_BIG_ADDRESS);
    leftMotorSpeed = MOTOR_STOP_SPEED;
    rightMotorSpeed = MOTOR_STOP_SPEED;
    servo_big.write(0);
    Serial.println("Robot is ready \n");
}

//**********************************************************************************
// Initialise the tasks for the scheduler
//**********************************************************************************
void task_init() {  
  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
 
  // Add tasks to the scheduler
  taskManager.addTask(tRead_infrared);
  taskManager.addTask(tRead_colour);
  taskManager.addTask(tRead_imu);
  taskManager.addTask(tSet_motor); 
  taskManager.addTask(tWeight_scan);
  taskManager.addTask(tDetect_base);
  taskManager.addTask(tWall_detection);
  taskManager.addTask(tCheck_watchdog);
     
  //enable the tasks
  tRead_infrared.enable();
  tRead_imu.enable();
  tSet_motor.enable();
  tCheck_watchdog.enable();
  tWall_detection.enable(); //CHECK
  //tWeight_scan.enable();
  tRead_colour.enable();
  //tCollect_weight.enable();
  //tDetect_base.enable();

 Serial.println("Tasks have been initialised \n");
}



//**********************************************************************************
// put your main code here, to run repeatedly
//**********************************************************************************
void loop() {
  taskManager.execute();    //execute the scheduler
  static int state = 0;
  static int attempted_collection_count;
  //Serial.println("Another scheduler execution cycle has oocured \n");
  switch (state)
    {
      case 0:
        Serial3.println("Roaming");
        Serial.print(red);
        tWall_detection.enable();
        tCheck_watchdog.enable();
        tWeight_scan.enable();
        //tWeight_scan.disable();
        weight_detected_count = 0; 
        if (dig_IR_0 == 0)
        {
          state = 1;
        }
        if (no_walls_detected_timeout)
        {
          state = 2;                           
        }
        if (robot_tilting_flag)
        {
          state = 2;
        }
        break;
      case 1:
        Serial3.println("Weight Collection");
        weight_detected_count = 0;
        rightMotorSpeed = MOTOR_STOP_SPEED;
        leftMotorSpeed = MOTOR_STOP_SPEED;
        set_motor();
        tWall_detection.disable();
        tCheck_watchdog.disable();
        tWeight_scan.disable();
        collect_weight();

        
        if (attempted_collection_count >=3)
        {
          failed_collect_weight_flag = 1;
          state = 3;
          open_scoop();
        }
        if ((dig_IR_0 == 0) && (attempted_collection_count)>=1) //(dig_IR_1 == 0)
        {
          attempted_collection_count++;
          collect_weight();
        }
        else
        {
          attempted_collection_count = 0;
          state = 0;
          tWeight_scan.enable();
        }
        break;
     case 2: // Watchdog no wall detected time out state
       Serial3.println("Watch dog");
       if (no_walls_detected_timeout)
       {
          tCheck_watchdog.disable();
          //Reverse
          right_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          left_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          delay(1000);
          //turn 180
          right_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          left_motor.writeMicroseconds(MOTOR_FORWARD_TEST_SPEED);      // 
          delay(700);
          //go back to search state
          state = 0;
          no_walls_detected_timeout = 0;
       } else if(robot_tilting_flag)
       {
          robot_tilting_flag = 0;
          tCheck_watchdog.disable();
          //Reverse
          right_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          left_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          delay(1000);
          //turn 180
          right_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          left_motor.writeMicroseconds(MOTOR_FORWARD_TEST_SPEED);      // 
          delay(700);
          //go back to search state
          state = 0;
       } else if (failed_collect_weight_flag)
       {
          failed_collect_weight_flag = 0;
          tCheck_watchdog.disable();
          //Reverse
          right_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          left_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          delay(1000);
          //turn 180
          right_motor.writeMicroseconds(MOTOR_REVERSE_TEST_SPEED);      // 
          left_motor.writeMicroseconds(MOTOR_FORWARD_TEST_SPEED);      // 
          delay(700);
          //go back to search state
          state = 0;
       }
        break;
    

    }
  
}
