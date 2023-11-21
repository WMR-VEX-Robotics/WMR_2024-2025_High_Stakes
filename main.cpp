/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       bowma                                                     */
/*    Created:      10/2/2023, 8:41:19 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

brain Brain; 

// define your global instances of motors and other devices here
// motors
motor tlMotor1 = motor(PORT1, ratio6_1, true);
motor blMotor11 = motor(PORT11, ratio6_1, true);
motor trMotor10 = motor(PORT10, ratio6_1, false);
motor brMotor20 = motor(PORT20, ratio6_1, false);
motor lcatapaultMotor = motor(PORT5, ratio18_1, true);
motor rcatapaultMotor = motor(PORT6, ratio18_1, false);

// not motors
controller mainController = controller(primary);
inertial inertialSensor = inertial(PORT8);
encoder enc1 = encoder(Brain.ThreeWirePort.A);
pneumatics solonoid1 = pneumatics(Brain.ThreeWirePort.H);
pneumatics solonoid2 = pneumatics(Brain.ThreeWirePort.G);

//odometry
/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your robot manually with the sidebar configurer. */
/*---------------------------------------------------------------------------*/

Drive chassis(

//Specify your drive setup below. There are eight options:
//ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
//For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
TANK_ONE_ENCODER,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(tlMotor1, blMotor11),

//Right Motors:
motor_group(trMotor10, brMotor20),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT8,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
0.6,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
360,

/*---------------------------------------------------------------------------*/
/*                                  PAUSE!                                   */
/*                                                                           */
/*  The rest of the drive constructor is for robots using POSITION TRACKING. */
/*  If you are not using position tracking, leave the rest of the values as  */
/*  they are.                                                                */
/*---------------------------------------------------------------------------*/

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
3,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
-2,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
-2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
5.5

);

int current_auton_selection = 0;
bool auto_started = false;

void motorsHalt(){
  // stop the motor with implicit type brake
  tlMotor1.stop(brake);
  blMotor11.stop(brake);
  trMotor10.stop(brake);
  brMotor20.stop(brake);
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void setdtBrakemode(brakeType mode){
  // change brake mode
  tlMotor1.setStopping(mode);
  blMotor11.setStopping(mode);
  trMotor10.setStopping(mode);
  brMotor20.setStopping(mode);
}

void ensureCalibration(){
  // MAKE SURE THE INERTIAL SENSOR CALIBRATES
  if (inertialSensor.isCalibrating() != false){
    wait(200,msec);
  }
}

void pre_auton(void) {
  // purge inertial sensor
  inertialSensor.calibrate();
  // ensure the inertial sensor is done calibrating before continuing on
  ensureCalibration();

  // purge encoder(s) rotation value
  enc1.resetRotation();

  // set brakes to the defined mode in the case of autonomous function it should be brake
  setdtBrakemode(brake);
  //mode is reset at the end of autonomous

  // to prevent catapault motor strain we will set the motors to coast
  lcatapaultMotor.setStopping(coast);
  rcatapaultMotor.setStopping(coast);

  // ensure that the first solonoid is registering as closed visually confirming this fact
  solonoid1.set(false);
  solonoid1.close();

  // ensure that the second solonoid is registering as closed visually confirming this fact
  solonoid2.set(false);
  solonoid2.close();
}

  // for skills
void competitionAuton(){
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
}

  // for skills
void skillsAuton() {
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);
  
  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
}

void autonType(uint8_t type) {
  // select different types of auton
  if (type == 0) {
    Brain.Screen.print("No Auton Loaded");
  } else if (type == 1) {
    competitionAuton();
  } else if (type == 2) {
    skillsAuton();
  }

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void autonomous(void) {
  autonType(0);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
bool popD = true;
void wingsDeployRetract(bool swapper){
  // snippet to deploy pneumatic wings
if (popD == true) {
      // if open make closed
      solonoid1.close();
      solonoid2.close();
      solonoid1.set(false);
      solonoid2.set(false);
      popD = false;

      // debugging
      Brain.Screen.setFillColor(red);
      Brain.Screen.drawCircle(0,0,10);
    }
    if (popD == false){
      // if closed make open
      solonoid1.open();
      solonoid2.open();
      solonoid1.set(true);      
      solonoid2.set(true);
      popD = true;

      // debugging
      Brain.Screen.setFillColor(blue);
      Brain.Screen.drawCircle(0,50,10);
    }
}

void tankDrive_user(){
  // tank drive user control left side on left right side on right
  tlMotor1.spin(forward, mainController.Axis3.position(), percent);
  trMotor10.spin(forward, mainController.Axis2.position(), percent);
  blMotor11.spin(forward, mainController.Axis3.position(), percent);
  brMotor20.spin(forward, mainController.Axis2.position(), percent);

  // brake 
  if (mainController.ButtonB.pressing() == true) {
    motorsHalt();
  }

  // spin catapault at maximum
  while (mainController.ButtonR2.pressing() == true){
    lcatapaultMotor.spin(forward, 12.7, volt);
    rcatapaultMotor.spin(forward, 12.7, volt);
  }

  //deploy wings
  if (mainController.ButtonL2.pressing() != false){
    wingsDeployRetract(!popD);
  }
}

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
   tankDrive_user();
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  
}
