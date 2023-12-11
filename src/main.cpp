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
motor tlMotor12 = motor(PORT12, ratio6_1, true);
motor blMotor11 = motor(PORT11, ratio6_1, true);
motor trMotor2 = motor(PORT2, ratio6_1, false);
motor brMotor4 = motor(PORT4, ratio6_1, false);
motor lcatapaultMotor20 = motor(PORT20, ratio18_1, true);
motor rcatapaultMotor7 = motor(PORT7, ratio18_1, true);
motor armMotor13 = motor(PORT13, ratio36_1, true);

// not motors
controller mainController = controller(primary);
inertial inertialSensor = inertial(PORT10);
//encoder enc1 = encoder(Brain.ThreeWirePort.A);
rotation rot1 = rotation(PORT8, true);
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
TANK_ONE_ROTATION,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(tlMotor12, blMotor11),

//Right Motors:
motor_group(trMotor2, brMotor4),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT10,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
0.5,

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

int current_auton_selection = 0;
bool auto_started = false;

bool halter = true;

void allowforskillsCata() {
  for (int i = 0; i <= 30; i++) {
    wait(985, msec);
    vex::task::sleep(15);
  }
  lcatapaultMotor20.stop(coast);
  rcatapaultMotor7.stop(coast);
  halter = false;
}

void motorsHalt(){
  // stop the motor with implicit type brake
  tlMotor12.stop(brake);
  blMotor11.stop(brake);
  trMotor2.stop(brake);
  brMotor4.stop(brake);
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
  tlMotor12.setStopping(mode);
  blMotor11.setStopping(mode);
  trMotor2.setStopping(mode);
  brMotor4.setStopping(mode);
}

void ensureCalibration(){
  // MAKE SURE THE INERTIAL SENSOR CALIBRATES
  if (inertialSensor.isCalibrating() != false){
    wait(200,msec);
  }
}

double percentasVolt(double n) {
  double increment = 0.127;
  return increment * n;
}

// volt = false percent = true
void spincataPerc(double P, bool VorP) {
  if (VorP != false) {
    lcatapaultMotor20.spin(forward, P, percent);
    rcatapaultMotor7.spin(forward, P, percent);
  } else {
    lcatapaultMotor20.spin(forward, percentasVolt(80.0), volt);
    rcatapaultMotor7.spin(forward, percentasVolt(80.0), volt);
  }
}



void pre_auton(void) {
  // purge inertial sensor
  inertialSensor.calibrate();
  // ensure the inertial sensor is done calibrating before continuing on
  ensureCalibration();

  // purge encoder(s) rotation value
  //enc1.resetRotation();

  //purge rotation values
  rot1.resetPosition();
  if (rot1.position(deg) != 0) {
    rot1.setPosition(0, deg);
  }

  // set brakes to the defined mode in the case of autonomous function it should be brake
  setdtBrakemode(brake);
  //mode is reset at the end of autonomous

  // to prevent catapault motor strain we will set the motors to coast
  lcatapaultMotor20.setStopping(coast);
  rcatapaultMotor7.setStopping(coast);

  // ensure that the first solonoid is registering as closed visually confirming this fact
  solonoid1.set(false);
  solonoid1.close();

  // ensure that the second solonoid is registering as closed visually confirming this fact
  solonoid2.set(false);
  solonoid2.close();
}

// for competition
void competitionAutonL(){
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  chassis.drive_distance(2);
  chassis.turn_to_angle(-90);
  armMotor13.spinFor(425, degrees, false);
  chassis.drive_distance(36);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
}

// for competition
void competitionAutonR(){
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);
  
  chassis.drive_distance(2);
  chassis.turn_to_angle(90);
  armMotor13.spinFor(425, degrees, false);
  chassis.drive_distance(36);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
}

void wingsDeployRetract(){
  // snippet to deploy pneumatic wings
  if (solonoid1.value() == true) {
    solonoid1.close(); solonoid2.close();
  } else {
    solonoid1.open(); solonoid2.open();
  }
}

// for skills
void skillsAuton() {
  //wingsDeployRetract();
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  // begin the fun program of skills auton
  chassis.drive_distance(8);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(7);
  chassis.turn_to_angle(45);
  chassis.drive_distance(-12);
  chassis.turn_to_angle(55);

  spincataPerc(95.0, false); // spins catapult at a given percent (swapping bool allows for different precisions)

  thread task1(allowforskillsCata); // creates timer thread for catapult in skills
  task1.detach(); // "allows" for execution from handle

  // gross programming but if the bot moves the bot moves. It shouldn't.
  while (halter != false) {
    wait(2, msec);
  }

  chassis.drive_distance(1);
  chassis.turn_to_angle(113);
  chassis.drive_distance(12);
  chassis.turn_to_angle(90);
  chassis.drive_distance(72);
  chassis.turn_to_angle(360);
  chassis.drive_distance(18);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(36);
  chassis.turn_to_angle(360);
  chassis.drive_distance(12);
  chassis.turn_to_angle(90);
  wingsDeployRetract();
  chassis.drive_distance(36);
  wingsDeployRetract();
  chassis.drive_distance(-34);
  chassis.turn_to_angle(360);
  chassis.drive_distance(20);
  chassis.turn_to_angle(100);
  wingsDeployRetract();
  chassis.drive_distance(36);
  wingsDeployRetract();
  /*chassis.turn_to_angle(155);
  chassis.drive_distance(38);
  chassis.turn_to_angle(180);
  armMotor13.spinFor(360, degrees, false);
  chassis.drive_distance(18);
  chassis.turn_to_angle(90);
  chassis.drive_distance(40);*/

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
}

// 1 if by match 2 if by skills 0 if by stupid (none loaded)
void autonType(uint8_t type) {
  // select different types of auton
  if (type == 0) {
    Brain.Screen.print("No Auton Loaded. Skipping...");
  } else if (type == 1) {
    Brain.Screen.print("Competition Auton Loaded. R");
    competitionAutonL();
  } else if (type == 2) {
    Brain.Screen.print("Skills Auton Loaded.");
    skillsAuton();
  } else if (type == 3) {
    Brain.Screen.print("Competition Auton Loaded. L");
    competitionAutonR();
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
  autonType(3);
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

void tankDrive_user(){
  // tank drive user control left side on left right side on right
  /*tlMotor12.spin(forward, mainController.Axis3.position(), percent);
  trMotor2.spin(forward, mainController.Axis2.position(), percent);
  blMotor11.spin(forward, mainController.Axis3.position(), percent);
  brMotor4.spin(forward, mainController.Axis2.position(), percent);*/
  chassis.control_arcade();

  // brake 
  if (mainController.ButtonB.pressing() == true) {
    motorsHalt();
  }

  // spin catapault
  if (mainController.ButtonR2.pressing() == true){
    spincataPerc(95.0, false);
  } else {
    lcatapaultMotor20.stop(hold);
    rcatapaultMotor7.stop(hold);
  }

  //deploy wings
  mainController.ButtonL2.pressed(wingsDeployRetract);

  // for hanging/blocker arm
  if (mainController.ButtonUp.pressing() == true) {
    armMotor13.spin(forward);
  } else if (mainController.ButtonDown.pressing() == true) {
    armMotor13.spin(reverse);
  } else {
    armMotor13.stop(hold);
  }
}

void usercontrol(void) {
  // User control code here, inside the loop
  //wingsDeployRetract();
  while (1) {
   tankDrive_user();
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  //wingsDeployRetract();
  spincataPerc(95.0, false);
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

}
