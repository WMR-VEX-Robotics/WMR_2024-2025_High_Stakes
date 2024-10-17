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

#pragma region Global
competition Competition;
brain Brain;
motor tplf_motor = motor(PORT12, true);
motor tprt_motor = motor(PORT1, false);
motor dwlf_motor = motor(PORT20, true);
motor dwrt_motor = motor(PORT10, true);
motor intake_motor = motor(PORT4, true);
motor hook_motor = motor(PORT3, true); 
motor_group MotorGrouplf = motor_group(tplf_motor, dwlf_motor);
motor_group MotorGrouprt = motor_group(tprt_motor, dwrt_motor);
controller mainController = controller(primary);
pneumatics solonoidA = pneumatics(Brain.ThreeWirePort.F);
pneumatics solonoidB = pneumatics(Brain.ThreeWirePort.E);
#pragma endregionpne
#pragma region Chassis
Drive chassis(

//Specify your drive setup below. There are eight options:
//ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
//For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
ZERO_TRACKER_ODOM,
//Left Motors:
MotorGrouplf,
//Right Motors:
MotorGrouprt,

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT5,

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
0,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
-2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
5.5

);
#pragma endregion
#pragma region Motors

void motorsHalt(){
  // stop the motor with implicit type brake
  tplf_motor.stop(coast);
  tprt_motor.stop(coast);
  dwlf_motor.stop(coast);
  dwrt_motor.stop(coast);
  intake_motor.stop(brake);
  hook_motor.stop(brake);
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
bool current_forwards = true;
void setdtBrakemode(brakeType mode){
  // change brake mode
  tplf_motor.setStopping(mode);
  tprt_motor.setStopping(mode);
  dwlf_motor.setStopping(mode);
  dwrt_motor.setStopping(mode);
  intake_motor.setStopping(mode);
  hook_motor.setStopping(mode);
}

void motorReverse() {
  if (current_forwards == false) {
    current_forwards = true;
  } else {
    current_forwards = false;
  }
}

#pragma endregion
#pragma region Pneumatics

void toggle_A(){
  if(solonoidA.value() == true){
    solonoidA.close();
  }
  else{
    solonoidA.open();
  }
}

void toggle_B(){
  if(solonoidB.value() == true){
    solonoidB.close();
  }
  else{
    solonoidB.open();
  }
}

#pragma endregion
#pragma region Autonomous

void pre_auton(void) {
  //inertialSensor5.calibrate();
  //ensureCalibration();
  setdtBrakemode(brake);
  wait(25, msec);
  Brain.Screen.clearScreen();
}

// 1 if by skills 2 if by right and 3 if by left 0 if by stupid (none loaded)
void autonType(int autonSelect) {
  // select different types of auton

  switch (autonSelect) {
    case 0:
      Brain.Screen.print("No Auton Loaded. Skipping...");
      //set_screen_color(1);
      break;
    case 1:
      Brain.Screen.print("Skills Auton Loaded.");
      // insert skills auton
      break;
    case 2:
      Brain.Screen.print("Competition Auton Loaded. R");
      // offensive auton
      break;
    case 3:
      Brain.Screen.print("Competition Auton Loaded. L");
      // defensive auton
      break;
    case 4:
      default_constants();
      // initialize position as (0,0,0)
      chassis.set_coordinates(0,0,0);
      drive_test();
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
  // insert auton
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
#pragma endregion
#pragma region Driver
void usercontrol(void) {
  setdtBrakemode(coast);
  while (1) {
   // tank drive user control left side on left right side on right
    mainController.ButtonL2.pressed(toggle_A);
    mainController.ButtonY.pressed(toggle_B);

    if (mainController.ButtonR2.pressing() == true ) {
      intake_motor.spin(forward, 12.5, volt);
      //hook_motor.spin(forward, 12.5, volt);
    } else if (mainController.ButtonR1.pressing() == true) {
      intake_motor.spin(reverse, 100, percent);
      //hook__motor.spin(reverse, 12.5, volt);
    } else {
      intake_motor.stop(coast);
      //hook_motor.stop(coast);
    }
    // /*
    if (mainController.ButtonR2.pressing() == true ) {
      hook_motor.spin(forward, 100, percent);
    } else if (mainController.ButtonR1.pressing() == true) {
      hook_motor.spin(reverse, 100, percent);
    } else {
      hook_motor.stop(coast);
    }
    // */
      MotorGrouplf.spin(vex::directionType::fwd, (mainController.Axis3.value() + (mainController.Axis1.value()/(5/2))), percent);
      MotorGrouprt.spin(vex::directionType::rev, (mainController.Axis3.value() - (mainController.Axis1.value()/(5/2))), percent);
      wait(20, msec);
  }
}
#pragma endregion

int main() {
  pre_auton();
  
  // wait for button press to map button to solonoid B.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

}