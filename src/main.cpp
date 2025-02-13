/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       bowma                                                     */
/*    Created:      10/2/2023, 8:41:19 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <iostream>
using namespace vex;

#pragma region Global
competition Competition;
brain Brain;
motor tplf_motor = motor(PORT12, true);
motor tprt_motor = motor(PORT5, false);
motor dwlf_motor = motor(PORT20, true);
motor dwrt_motor = motor(PORT10, false);
motor intake_motor = motor(PORT4, true);
motor hook_motor = motor(PORT3, true); 
motor wall_stake_motor = motor(PORT16, true);
motor_group MotorGrouplf = motor_group(tplf_motor, dwlf_motor);
motor_group MotorGrouprt = motor_group(tprt_motor, dwrt_motor);
//motor_group allDrive = motor_group(tplf_motor, dwlf_motor, tprt_motor, dwrt_motor))
controller mainController = controller(primary);
pneumatics solonoidA = pneumatics(Brain.ThreeWirePort.F); //goal grabber pneumatics
pneumatics solonoidB = pneumatics(Brain.ThreeWirePort.E); //goal doinker pneumatics
pneumatics solonoidC = pneumatics(Brain.ThreeWirePort.G); //color doinker pneumatics
color Red = color(0xFF0000);
color Blue = color(0x0000FF);
optical colorSensor = optical(PORT14);


#pragma endregion
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
PORT9,

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
  wall_stake_motor.stop(brake);
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

void toggle_A(){                  //goal grabber 
  if(solonoidA.value() == true){
    solonoidA.close();
  }
  else{
    solonoidA.open();
  }
}

void toggle_B(){                  //goal doinker
  if(solonoidB.value() == true){
    solonoidB.close();
  }
  else{
    solonoidB.open();
  }
}

void toggle_C(){                 //color doinker
  if(solonoidC.value() == true){
    solonoidC.close();
  }
  else{
    solonoidC.open();
  }
}

#pragma endregion
#pragma region Autonomous


void autoDoinkRed() {
    int read = colorSensor.hue();
    std::string result = "";
    if(read <= 20){
      result = "red";
      hook_motor.stop();
    }
    else if(read >= 100){
      result = "blue";
    }
    else {
      result = "other";
    }
    /*Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);

    Brain.Screen.print(result.c_str());*/
}

void autoDoinkBlue() {
    int read = colorSensor.hue();
    std::string result = "";
    if(read >= 100){
      result = "blue";
      hook_motor.stop();
    }
    else if(read <= 20){
      result = "red";
    }
    else {
      result = "other";
    }
    /*Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);

    Brain.Screen.print(result.c_str());*/
}

void autoDoink() {
  //autoDoinkRed(); //set to autoDoinkRed() if on blue side else set to autoDoinkBlue()
  autoDoinkBlue();
}

int threadedAutoDoink(void* p) {
  while (true) {
    autoDoink();
  }
  return 0;
}

void turnTo(float x) {
  setdtBrakemode(vex::brakeType::brake); // added
  wait(0.3, sec);
  chassis.turn_to_angle(x);
  setdtBrakemode(vex::brakeType::coast);
}


void pre_auton(void) {
  //inertialSensor.calibrate();
  //ensureCalibration();
  solonoidA.open();
  solonoidB.close();
  solonoidC.close();
  setdtBrakemode(brake);
  wait(25, msec);
  setdtBrakemode(brake);
  default_constants();
  chassis.set_coordinates(0,0,0);
  wait(25, msec);
  Brain.Screen.clearScreen();
  Brain.Screen.drawImageFromFile("Name.bmp", 0, 0);
  autoDoink();
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
void blue_negative_auton(){
  thread(threadedAutoDoink, nullptr);
  chassis.drive_distance(-12);
  wait(0.1, sec);
  solonoidA.close();
  wait(0.1, sec);
  hook_motor.spin(vex::directionType::fwd, 100, percent);
  chassis.drive_distance(-2.3);
  wait(0.1, sec);
  chassis.turn_to_angle(-90);
  wait(0.1,sec);
  intake_motor.spin(vex::directionType::fwd, 75, percent);
  chassis.drive_distance(7.5);
  wait(0.1, sec);
  chassis.turn_to_angle(-31);
  solonoidB.open();
  wait(0.1,sec);
  chassis.drive_distance(22);
  chassis.turn_to_angle(60);
  wait(0.1, sec);
  solonoidB.close();
  wait(0.1, sec);
  chassis.drive_distance(17);
  chassis.turn_to_angle(90);
  chassis.drive_distance(4);
  chassis.turn_to_angle(150);
  chassis.drive_distance(11.5);
}

void blue_positive_auton() {
  thread(threadedAutoDoink, nullptr);
  intake_motor.spin(fwd, 75, percent);
  chassis.drive_distance(-12);
  wait(0.1,sec);
  solonoidA.close();
  wait(0.1, sec);
  hook_motor.spin(fwd, 100, percent);
  chassis.drive_distance(-1.5);
  wait(0.1, sec);
  chassis.turn_to_angle(90);
  wait(0.1,sec);
  chassis.drive_distance(7.5);
  wait(0.1, sec);
  chassis.turn_to_angle(0);
  chassis.drive_distance(7.5);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(15);
  chassis.drive_distance(-5);
  chassis.drive_distance(10);
  chassis.drive_distance(-7);
  chassis.turn_to_angle(180);
  chassis.drive_distance(15);
}

void red_negative_auton() {
  thread(threadedAutoDoink, nullptr);
  intake_motor.spin(fwd, 75, percent);
  chassis.drive_distance(-12);
  wait(0.1,sec);
  solonoidA.close();
  wait(0.1, sec);
  hook_motor.spin(fwd, 100, percent);
  chassis.drive_distance(-1.5);
  wait(0.1, sec);
  chassis.turn_to_angle(90);
  wait(0.1,sec);
  chassis.drive_distance(7.5);
  wait(0.1, sec);
  chassis.turn_to_angle(0);
  chassis.drive_distance(7.5);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(15);
  chassis.drive_distance(-5);
  chassis.drive_distance(10);
  chassis.drive_distance(-7);
  chassis.turn_to_angle(180);
  chassis.drive_distance(15);
}

void red_positive_auton(){
  thread(threadedAutoDoink, nullptr);
  chassis.drive_distance(-12);
  wait(0.1, sec);
  solonoidA.close();
  wait(0.1, sec);
  hook_motor.spin(vex::directionType::fwd, 100, percent);
  chassis.drive_distance(-2.3);
  wait(0.1, sec);
  chassis.turn_to_angle(-90);
  wait(0.1,sec);
  intake_motor.spin(vex::directionType::fwd, 75, percent);
  chassis.drive_distance(7.5);
  wait(0.1, sec);
  chassis.turn_to_angle(-31);
  solonoidB.open();
  wait(0.1,sec);
  chassis.drive_distance(22);
  chassis.turn_to_angle(60);
  wait(0.1, sec);
  solonoidB.close();
  wait(0.1, sec);
  chassis.drive_distance(17);
  chassis.turn_to_angle(90);
  chassis.drive_distance(4);
  chassis.turn_to_angle(150);
  chassis.drive_distance(11.5);
}




void skillsAuton(){
  default_constants();
  //mid-field start code
  wall_stake_motor.setVelocity(100, percent);
  wall_stake_motor.spinFor(forward, 1.3, seconds);
  chassis.drive_distance(2.66);
  wall_stake_motor.spinFor(reverse, 1.4, seconds);
  turnTo(-90);
  chassis.drive_distance(-4.5);
  chassis.drive_distance(-2.75);
  solonoidA.close();
  wait(0.75, sec);
  turnTo(0);

  //1/4th field code
  intake_motor.spin(fwd, 100, percent);
  hook_motor.spin(vex::directionType::fwd, 75, percent);
  chassis.drive_distance(6);
  turnTo(90);
  chassis.drive_distance(7.35);
  chassis.drive_distance(-1.35);
  turnTo(180);
  chassis.drive_distance(6);
  chassis.drive_distance(4);
  wait(0.3, sec);
  chassis.drive_distance(-4.5);
  wait(0.3, sec);
  turnTo(90);
  chassis.drive_distance(4.5);
  chassis.drive_distance(-1);
  turnTo(0.5);
  chassis.drive_distance(9.41);
  chassis.drive_distance(4.5);
  chassis.drive_distance(-15);
  turnTo(-30);
  hook_motor.spin(reverse, 10, percent);
  hook_motor.stop();
  solonoidA.open();
  chassis.drive_distance(-4);
  chassis.drive_distance(2.3);
  turnTo(90);

  //1/2 fieldactual
  chassis.drive_distance(-22.625);
  chassis.drive_distance(-2.75);
  solonoidA.close();
  wait(0.75, sec);
  turnTo(0);
  intake_motor.spin(fwd, 100, percent);
  hook_motor.spin(vex::directionType::fwd, 75, percent);
  chassis.drive_distance(6);
  turnTo(-90);
  chassis.drive_distance(7.35);
  chassis.drive_distance(-1.35);
  turnTo(180);
  chassis.drive_distance(6);
  chassis.drive_distance(4);
  wait(0.3, sec);
  chassis.drive_distance(-4.5);
  wait(0.3, sec);
  turnTo(-90);
  chassis.drive_distance(4.5);
  chassis.drive_distance(-1);
  turnTo(-0.5);
  chassis.drive_distance(9.41);
  chassis.drive_distance(4.5);
  chassis.drive_distance(-15);
  turnTo(30);
  hook_motor.spin(reverse, 10, percent);
  hook_motor.stop();
  solonoidA.open();
  chassis.drive_distance(-4);
  chassis.drive_distance(7);
  turnTo(0);
  chassis.drive_distance(16);
  turnTo(30);
  solonoidB.open();
  chassis.drive_distance(10);
  chassis.left_swing_to_angle(70);
  chassis.drive_distance(15);
  chassis.drive_distance(-1.5);
  chassis.turn_to_angle(90);
  chassis.drive_distance(-32.6);
  /*turnTo(1.2);
  hook_motor.spin(fwd, 20, percent);
  chassis.drive_distance(12);
  chassis.drive_distance(4);
  chassis.drive_distance(-5.35);
  wait(0.75, sec);
  hook_motor.spin(reverse, 25, percent);
  turnTo(-90);
  chassis.drive_distance(-4.76);
  wall_stake_motor.spinFor(fwd, 1.4, sec);
  chassis.drive_distance(3.5);
  wall_stake_motor.spinFor(reverse, 1.5, sec);
  /*solonoidA.open();
  chassis.turn_to_angle(-22);
  hook_motor.spin(fwd, 15.8, percent);
  chassis.drive_distance(7.5);
  chassis.turn_to_angle(-15);
  chassis.drive_distance(-4.7);
  hook_motor.spin(reverse, 50, percent);
  chassis.turn_to_angle(-65);
  chassis.drive_distance(-4.5);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(-1);
  wall_stake_motor.spinFor(fwd, 1.4, sec);
  chassis.drive_distance(3.5);
  wall_stake_motor.spinFor(reverse, 1.5, sec);
  chassis.turn_to_angle(180);
  chassis.drive_distance(8.6);
  chassis.drive_distance(10);
  chassis.drive_distance(-4.5);
  chassis.turn_to_angle(90);
  chassis.drive_distance(5);
  chassis.turn_to_angle(-25);
  hook_motor.spin(vex::directionType::rev, 100, percent);
  solonoidA.open();
  chassis.drive_distance(-7.5);
  
  //1/2 field code
  chassis.drive_distance(3); //subject to change

  hook_motor.spin(vex::directionType::fwd, 100, percent);
  setdtBrakemode(vex::brakeType::brake); // added
  chassis.turn_to_angle(90);
  wait(0.5, sec);
  setdtBrakemode(vex::brakeType::coast); // added
  hook_motor.spin(vex::directionType::fwd, 100, percent);
  chassis.drive_distance(-23, -90, 8, 12, 10, 10, 9000);
  chassis.drive_distance(-5, -90, 3, 12, 10, 10, 9000);
  solonoidA.close();
  chassis.turn_to_angle(0);
  intake_motor.spin(fwd, 80, percent);
  hook_motor.spin(vex::directionType::fwd, 75, percent);
  chassis.drive_distance(7);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(8.5);
  chassis.turn_to_angle(-30);
  chassis.drive_distance(8);
  chassis.turn_to_angle(30);
  hook_motor.spin(fwd, 4, volt);
  chassis.drive_distance(7);
  chassis.drive_distance(-4);
  wait(0.3, sec);
  hook_motor.spin(reverse, 50, percent);
  chassis.turn_to_angle(65);
  chassis.drive_distance(-4.5);
  chassis.turn_to_angle(90);
  wall_stake_motor.spinFor(fwd, 1.4, sec);
  chassis.drive_distance(4.5);
  wall_stake_motor.spinFor(reverse, 1.5, sec);
  chassis.turn_to_angle(180);
  chassis.drive_distance(8.6);
  chassis.drive_distance(10);
  chassis.drive_distance(-4.5);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(5);
  chassis.turn_to_angle(25);
  hook_motor.spin(vex::directionType::rev, 100, percent);
  solonoidA.open();
  chassis.drive_distance(-7.5);*/
}

void pidTest() {
  default_constants();
  
  solonoidA.close();

  wait(1.5, sec);
  //chassis.drive_distance(-8.5);
  //chassis.drive_distance(7);
  //hook_motor.spin(fwd);
  //chassis.drive_distance(-4);
  //solonoidA.close();
  chassis.drive_distance(3);
  wait(0.3, sec);
  turnTo(90);
  
}

void autonomous(void) {
  //blue_negative_auton(); //best slot 1

  //blue_positive_auton(); //slot 2

  //red_negative_auton(); //slot 3
  
  //red_positive_auton(); //slot 4

  skillsAuton();          //slot 5

  //pidTest();
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
    mainController.ButtonL2.pressed(toggle_A); //goal grabber
    mainController.ButtonL1.pressed(toggle_B);  //goal doinker
    
    
    

    if (mainController.ButtonR2.pressing() == true ) {
      intake_motor.spin(forward, 75, percent);
      hook_motor.spin(forward, 100, percent);
    } else if (mainController.ButtonR1.pressing() == true) {
      intake_motor.spin(reverse, 75, percent);
      hook_motor.spin(reverse, 62, percent);
    } else if (mainController.ButtonX.pressing() == true) { // for getting onto wall stake contraption
      intake_motor.spin(forward, 4, volt);
      hook_motor.spin(forward , 4, volt);
    } else if(mainController.ButtonDown.pressing() == true){
      wall_stake_motor.spin(vex::directionType::fwd, 100, percent);
    } else if(mainController.ButtonUp.pressing() == true){
      wall_stake_motor.spin(vex::directionType::rev, 100, percent);
    } else {
      intake_motor.stop(coast);
      hook_motor.stop(coast);
      wall_stake_motor.stop(brake);
    }
 
    autoDoink();


    
     /*
    if (mainController.ButtonR2.pressing() == true ) {
      hook_motor.spin(forward, 12.5, volt);
    } else if (mainController.ButtonR1.pressing() == true) {
      hook_motor.spin(reverse, 12.5, volt);
    } else {
      hook_motor.stop(coast);
    }
    // */
      MotorGrouplf.spin(vex::directionType::fwd, (mainController.Axis3.value() + (mainController.Axis1.value()/(5/2))), percent);
      MotorGrouprt.spin(vex::directionType::fwd, (mainController.Axis3.value() - (mainController.Axis1.value()/(5/2))), percent);
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