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
motor tlMotor18 = motor(PORT18, ratio6_1, true);
motor mlMotor19 = motor(PORT19, ratio6_1, true);
motor blMotor20 = motor(PORT20, ratio6_1, true);
motor trMotor13 = motor(PORT13, ratio6_1, false);
motor mrMotor12 = motor(PORT12, ratio6_1, false);
motor brMotor11 = motor(PORT11, ratio6_1, false);
/*motor tlMotor18 = motor(PORT17, ratio18_1, true);
motor mlMotor19 = motor(PORT19, ratio18_1, true);
motor blMotor20 = motor(PORT20, ratio18_1, true);
motor trMotor13 = motor(PORT13, ratio18_1, false);
motor mrMotor12 = motor(PORT12, ratio18_1, false);
motor brMotor11 = motor(PORT11, ratio18_1, false);*/

motor catapaultMotor14 = motor(PORT14, ratio18_1, false);
motor intakeMotor2 = motor(PORT2, ratio18_1, true);
//motor armElevator3 = motor(PORT3, ratio18_1, false);
motor catapaultMotor4 = motor(PORT4, ratio18_1, true);
// not motors
controller mainController = controller(primary);
inertial inertialSensor5 = inertial(PORT5);
//encoder enc1 = encoder(Brain.ThreeWirePort.A);
//rotation rot1 = rotation(PORT8, true);
pneumatics solonoidF = pneumatics(Brain.ThreeWirePort.F); //hang

pneumatics solonoidH = pneumatics(Brain.ThreeWirePort.H); //forward wing
pneumatics solonoidG = pneumatics(Brain.ThreeWirePort.G); //forward wing 2

pneumatics solonoidE = pneumatics(Brain.ThreeWirePort.E); //rear wings

// select color brain goes to on auton completion, and autonomous program to be runned if any
int type = -1;
int team = -1;

// generate a position for the position pair grid with error checks use length 0 for calculated origin position
int det_Positionon_screen(char dimension, int length, int origin, int position_in_Series) {
  int object_origin;
  if (position_in_Series == 0) {
    object_origin = origin;
  } else {
    object_origin = origin;
    for (int i = 1; i <= position_in_Series; i++) {
      if (length == 0) {
        object_origin = object_origin + origin;
      } else {
        object_origin = object_origin + length + origin;
      }
    }
  }

  int end = object_origin + length;

  if (dimension == 'Y') {
    if (length == 0) {
      return end;
    } else {
      if (end < 272) {
        return end = object_origin + length + origin;
      } else {
        return end;
      }
    }
  } else if (dimension == 'X') {
    if (length == 0) {
      return end;
    } else {
      if (end < 480) {
        return end = object_origin + length + origin;
      } else {
        return end;
      }
    }
  }

  return -1;
}

// Select which team you are
void color_select() {
  bool unselected = true;
    int Y;
    int X;

    int originX = 20;
    int width = 100;
   
    int originY = 20;
    int height = 100;

    Brain.Screen.drawRectangle(det_Positionon_screen('X', 0, originX, 0), det_Positionon_screen('Y', 0, originY, 0), width, height, blue);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 0), det_Positionon_screen('Y', 0, originY, 0), width, height, purple);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 1), det_Positionon_screen('Y', 0, originY, 0), width, height, red);


 while(unselected == true){
        if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= originX && X <= originX+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                team = 0;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        } else if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= det_Positionon_screen('X', width, originX, 0) && X <= det_Positionon_screen('X', width, originX, 0)+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                team = 1;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        } else if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= det_Positionon_screen('X', width, originX, 1) && X <= det_Positionon_screen('X', width, originX, 1)+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                team = 2;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        }
 }
}

// select auton
void autonSelect_buttons() {
  bool unselected = true;
    int Y;
    int X;

    int originX = 10;
    int width = 100;
   
    int originY = 10;
    int height = 100;
    
    Brain.Screen.drawRectangle(det_Positionon_screen('X', 0, originX, 0), det_Positionon_screen('Y', 0, originY, 0), width, height, white);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 0), det_Positionon_screen('Y', 0, originY, 0), width, height, purple);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 1), det_Positionon_screen('Y', 0, originY, 0), width, height, green);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 2), det_Positionon_screen('Y', 0, originY, 0), width, height, yellow);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', 0, originX, 0), det_Positionon_screen('Y', height, originY, 0), width, height, cyan);
    Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 0), det_Positionon_screen('Y', height, originY, 0), width, height, cyan);   
    Brain.Screen.setPenColor(black);
    Brain.Screen.setCursor(4,5);
    Brain.Screen.setFillColor(white);
    Brain.Screen.print("None");
    Brain.Screen.setCursor(4,15);
    Brain.Screen.setFillColor(purple);
    Brain.Screen.print("Skills");
    Brain.Screen.setCursor(4,27);
    Brain.Screen.setFillColor(green);
    Brain.Screen.print("Right");
    Brain.Screen.setCursor(4,38);
    Brain.Screen.setFillColor(yellow);
    Brain.Screen.print("Left");
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.setPenColor(white);
    Brain.Screen.setFillColor(black);
      /*Brain.Screen.print("Entering Auton Diagnostic Mode");
      for (int i = 0; i <= 10; i++) {
        wait(200, msec);
        Brain.Screen.print(".");
      }*/
    //armElevator3.stop(coast);
    while(unselected == true){
        if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= originX && X <= originX+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                type = 0;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        } else if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= det_Positionon_screen('X', width, originX, 0) && X <= det_Positionon_screen('X', width, originX, 0)+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                type = 1;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        } else if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= det_Positionon_screen('X', width, originX, 1) && X <= det_Positionon_screen('X', width, originX, 1)+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                type = 2;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        } else if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= det_Positionon_screen('X', width, originX, 2) && X <= det_Positionon_screen('X', width, originX, 2)+width) && (Y >= originY && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                type = 3;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        }
        else if (Brain.Screen.pressing()){

            X = Brain.Screen.xPosition();//X pos of press
            Y = Brain.Screen.yPosition();// Y pos of press

            //Checks if press is within boundaries of rectangle
            if ((X >= det_Positionon_screen('X', 0, originX, 0) && X <= det_Positionon_screen('X', 0, originX, 0)+width) && (Y >= det_Positionon_screen('Y', height, originY, 0) && Y <= det_Positionon_screen('Y', height, originY, 0))){
                Brain.Screen.clearScreen();
                type = 3;
                unselected = false;
                printf("XPress %d\n", X);
                printf("YPress %d\n", Y);
            }
        }
    }
}

void set_screen_color(int A) {
  if (A == 0) {
    Brain.Screen.clearScreen(vex::color::blue);
  } else if(A == 2) {
    Brain.Screen.clearScreen(vex::color::red);
  } else {
    Brain.Screen.clearScreen(vex::color::purple);
  }
}

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
ZERO_TRACKER_ODOM,
//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(tlMotor18, mlMotor19, blMotor20),

//Right Motors:
motor_group(trMotor13, mrMotor12, brMotor11),

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

int current_auton_selection = 0;
bool auto_started = false;

// must always start as true!
bool halter = true;

void allowforskillsCata() {
  for (int i = 0; i <= 22 /*2*/; i++) {
    wait(985, msec);

    vex::task::sleep(15);
  }
  catapaultMotor14.stop(coast);
  catapaultMotor4.stop(coast);
  intakeMotor2.stop(coast);
  //chassis.set_coordinates(0,0,0);
  halter = false;
}

void motorsHalt(){
  // stop the motor with implicit type brake
  tlMotor18.stop(brake);
  mlMotor19.stop(brake);
  blMotor20.stop(brake);
  trMotor13.stop(brake);
  mrMotor12.stop(brake);
  brMotor11.stop(brake);
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
  tlMotor18.setStopping(mode);
  mlMotor19.setStopping(mode);
  blMotor20.setStopping(mode);
  trMotor13.setStopping(mode);
  mrMotor12.setStopping(mode);
  brMotor11.setStopping(mode);
}

void ensureCalibration(){
  // MAKE SURE THE INERTIAL SENSOR CALIBRATES
  if (inertialSensor5.isCalibrating() != false){
    wait(200,msec);
  }
}

double percentasVolt(double n) {
  double increment = 0.127;
  return (increment * n);
}

// volt = false percent = true
void spincataPerc(double P, bool inPercent) {
  if (inPercent) {
    catapaultMotor14.spin(forward, P, percent);
    catapaultMotor4.spin(forward, P, percent);
  } else {
    catapaultMotor14.spin(forward, percentasVolt(P), volt);
    catapaultMotor4.spin(forward, percentasVolt(P), volt);
  }
}

void spincataPercrev(double P, bool inPercent) {
  if (inPercent) {
    catapaultMotor14.spin(reverse, P, percent);
  } else {
    catapaultMotor14.spin(reverse, percentasVolt(P), volt);
  }
}

bool current_forwards = true;

void motorReverse() {
  //motor tlMotor18 = motor(PORT12, ratio6_1, false);
  //motor blMotor20 = motor(PORT11, ratio6_1, false);
  //motor trMotor2 = motor(PORT2, ratio6_1, true);
  //motor brMotor11 = motor(PORT4, ratio6_1, true);
  if (current_forwards == false) {
    current_forwards = true;
  } else {
    current_forwards = false;
  }
  //current_forwards = !current_forwards;
}

void pre_auton(void) {

  //int originX = 10;
  //int originY = 20;
  //int width = 80;
  //int height = 80;
  // purge inertial sensor
  inertialSensor5.calibrate();
  // ensure the inertial sensor is done calibrating before continuing on
  ensureCalibration();
  //Brain.Screen.drawRectangle(det_Positionon_screen('X', 0, originX, 0), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // purge encoder(s) rotation value
  //enc1.resetRotation();

  //purge rotation values
  //rot1.resetPosition();
  /*if (rot1.position(deg) != 0) {
    rot1.setPosition(0, deg);
  }*/

  // set brakes to the defined mode in the case of autonomous function it should be brake
  setdtBrakemode(brake);
  //Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 1), det_Positionon_screen('Y', 0, originY, 0), width, height, green);
  //mode is reset at the end of autonomous
  //Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 2), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // to prevent catapault motor strain we will set the motors to coast
  catapaultMotor14.setStopping(coast);
  //Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 3), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // ensure that the first solonoid is registering as closed visually confirming this fact
  solonoidH.set(false);
  solonoidH.close();
  solonoidF.close();
  //Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 4), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // ensure that the second solonoid is registering as closed visually confirming this fact
  solonoidG.set(false);
  solonoidG.close();
  //Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 5), det_Positionon_screen('Y', 0, originY, 0), width, height, green);
  wait(25, msec);
  Brain.Screen.clearScreen();

  //autonSelect_buttons();
  /*if (type != 0 && type != 4) {
    color_select();
  }*/

}

void endgame() {
  if (solonoidF.value() == true) {
    solonoidF.close();
  } else {
    solonoidF.open();
  }
}

// for competition

void wingsDeployRetract() {
  // snippet to deploy pneumatic wings
  if (solonoidH.value() == true) {
    solonoidH.close(); 
    solonoidG.close();
  } else {
    solonoidH.open(); 
    solonoidG.open();
  }
}

void deployBackWings() {
  if(solonoidE.value() == true) {
    solonoidE.close();
  } else {
    solonoidE.open();
  }
}

bool icReset = false;
bool intakecontrolEnabled = false;

int intakeControl() {
  double ticks = 0.0;
  // a modifier for voltage
  int pause = 0;
  // a timeout modifier that defaults to 0
  while(intakecontrolEnabled){
    if (icReset || intakeMotor2.power() < 4.6){
      ticks = 0.0;
      icReset = false;
    }

    pause = 0;

    if (ticks >= 11.0) {
      ticks = 12.7;
      pause += 450;
    }

    if(intakeMotor2.power() >= 4.6) {
      intakeMotor2.spin(forward, (12.7 - ticks), volt);
      ticks += 1.0;
    }
    int timeout = 50 + pause;
    vex::task::sleep(timeout);
  } 

  return 1;
}

// for competition
void competitionAutonL(){
  //armElevator3.stop(hold);
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  endgame();
  wait(500, msec);
  endgame();

  intakeMotor2.spin(forward, 12.7, volt);
  
  chassis.drive_distance(4);

  chassis.turn_to_angle(-45);
  
  chassis.drive_distance(32);

  chassis.turn_to_angle(0);

  intakeMotor2.spin(reverse, 12.7, volt);

  wait(250, msec);

  chassis.drive_distance(-2);

  chassis.turn_to_angle(180);
  
  intakeMotor2.stop(hold);

  chassis.drive_distance(-14);

  chassis.drive_distance(8);

  chassis.drive_distance(-14);

  chassis.drive_distance(8);

  chassis.turn_to_angle(90);

  /*wait(5, seconds);

  chassis.drive_distance(-10);
 
  chassis.turn_to_angle(225);

  chassis.drive_distance(18);

  chassis.drive_distance(-6);

  chassis.drive_distance(3);

  chassis.turn_to_angle(270);*/

  intakeMotor2.stop(coast);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
  //set_screen_color(team);
}


void competitionAutonR(){
  // set operating constant to their default values
  //armElevator3.stop(hold);
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  endgame();
  wait(500, msec);
  endgame();

  intakeMotor2.spin(forward, 12.7, volt);
  
  chassis.drive_distance(8);

  chassis.turn_to_angle(56);
  
  chassis.drive_distance(31);

  chassis.turn_to_angle(0);

  intakeMotor2.spin(reverse, 12.7, volt);
  wait(250, msec);

  chassis.drive_distance(-2);

  chassis.turn_to_angle(180);

  intakeMotor2.stop(hold);

  chassis.drive_distance(-14);

  chassis.drive_distance(8);

  chassis.drive_distance(-14);

  chassis.drive_distance(8);

  chassis.turn_to_angle(270);

  /*wait(5, seconds);

  chassis.drive_distance(-10);

  chassis.drive_distance(3);

  chassis.drive_distance(-6);

  chassis.drive_distance(3);

  chassis.turn_to_angle(270);*/

  intakeMotor2.stop(coast);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);

  //set_screen_color(team);
}

void skillsautoPos() {
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(8.5, 1.5, 0, 10, 0);
  chassis.set_heading_constants(8, .4, 0, 1, 0);
  chassis.set_turn_constants(4, .4, .03, 3, 15);
  chassis.set_swing_constants(10, .3, .001, 2, 15);

  // Each exit condition set is in the form (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 200, 1750);
  chassis.set_turn_exit_conditions(1, 200, 1750);
  chassis.set_swing_exit_conditions(1, 200, 1000);
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  endgame();
  wait(500, msec);
  endgame();

  chassis.turn_to_angle(-30);
  chassis.drive_distance(30);
  intakeMotor2.spin(reverse);
  chassis.drive_distance(13);
  chassis.drive_distance(-30);
  chassis.turn_to_angle(70);
  chassis.drive_distance(-6.5);
  chassis.turn_to_angle(65); 
}

void leftAutondev(){
  intakecontrolEnabled = true;
  vex::task iC( intakeControl );
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  deployBackWings();

  chassis.drive_distance(-18);

  chassis.right_swing_to_angle(-45);

  chassis.drive_distance(-26);

  chassis.turn_to_angle(-45);

  chassis.drive_distance(10);

  chassis.set_coordinates(0,0,0);

  deployBackWings();

  endgame();
  wait(100, msec);
  endgame();

  chassis.turn_to_angle(90);

  chassis.turn_to_angle(115);

  intakeMotor2.spin(forward, 12.7, volt);

  chassis.drive_distance(46);

  wait(50, msec);

  chassis.turn_to_angle(265);

  chassis.drive_distance(22);

  chassis.turn_to_angle(180);

  iC.suspend();

  chassis.drive_distance(10);

  intakeMotor2.spin(reverse, 100, percent);

  wait(150, msec);

  chassis.drive_distance(-8);

  iC.resume();

  chassis.turn_to_angle(120);

  intakeMotor2.spin(forward, 100, percent);

  chassis.drive_distance(26);

  chassis.right_swing_to_angle(80);

  intakecontrolEnabled = false;

  deployBackWings();

  chassis.set_drive_exit_conditions(1.5, 200, 900);

  chassis.drive_distance(-36);

  chassis.drive_distance(20);

  deployBackWings();

  intakeMotor2.stop(coast);
}

void skillsautonDev(){

  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  chassis.drive_distance(18);
  chassis.turn_to_angle(70);
  chassis.drive_distance(-20);
  chassis.turn_to_angle(65);

  chassis.set_coordinates(-14, 14.9, 65);
  //spincataPerc(65.0, true);
  

  //chassis.drive_distance(20);

  //chassis.turn_to_angle(0);
  intakeMotor2.spin(reverse, 90, percent);
  //chassis.turn_to_angle(-20);
  chassis.drive_distance(80);
  //chassis.turn_to_angle(80);
  //wingsDeployRetract();

  //chassis.set_drive_exit_conditions(1.5, 200, 3000);

  //wingsDeployRetract();

  //chassis.drive_distance(65);
  
  //chassis.turn_to_angle(25);
  //chassis.turn_to_angle(-25);
  //chassis.turn_to_angle(0);
  //chassis.drive_distance(20);
  //chassis.set_drive_exit_conditions(1.5, 200, 1250); // going over the middle
  //chassis.drive_distance(-20);

 // chassis.set_coordinates(0, 0, 0);
  //chassis.turn_to_angle(0);
  //chassis.drive_distance(30);

 // wingsDeployRetract();
  
 // chassis.drive_distance(-30);
 // intakeMotor2.stop(coast);
 // chassis.turn_to_angle(90);
 // chassis.drive_distance(20);
 // chassis.turn_to_angle(0);
 // wingsDeployRetract();
 // intakeMotor2.spin(reverse, 80, percent);
 // chassis.drive_distance(30);
 // wingsDeployRetract();
 // chassis.drive_distance(-30);
 // intakeMotor2.stop(coast); 
  /*wingsDeployRetract();

  chassis.drive_distance(30);

  wingsDeployRetract();

  chassis.drive_distance(-20);

  chassis.turn_to_angle(-90);

  chassis.drive_distance(14);

  chassis.turn_to_angle(0);

  chassis.drive_distance(30);

  wingsDeployRetract();

  chassis.drive_distance(-20);*/

}

// for skills
void skillsAuton() {
  //wingsDeployRetract();
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);
  
  //chassis.drive_distance(80);

  endgame();
  wait(200, msec);
  endgame();


  // begin the fun program of skills auton
  //achassis.drive_distance(18);
 //a chassis.turn_to_angle(70);
  //spincataPerc(65.0, true);
  chassis.turn_to_angle(-30);
  //chassis.drive_with_voltage(chassis.drive_max_voltage, chassis.drive_max_voltage); 
  chassis.drive_distance(-40);
  chassis.turn_to_angle(-20);
  chassis.drive_distance(-20);
  //wait(500, msec);
  chassis.drive_distance(20);
  chassis.turn_to_angle(-112);
  chassis.drive_distance(-4);
  chassis.left_swing_to_angle(-111); // 115
 /*thread task1(allowforskillsCata); // creates timer thread for catapult in skills
  task1.detach(); // "allows" for execution from handle
  spincataPerc(90,true);
  while (halter != false) {
    wait(2, msec); // sit there and wait while catapult is spinning
    //armElevator3.stop(hold);
  }*/
  wait(500, msec);
  chassis.set_coordinates(0,0,-111); // 115
  chassis.turn_to_angle(-60);
  chassis.drive_distance(26);
  chassis.turn_to_angle(90);
  intakeMotor2.spin(reverse);
  chassis.drive_distance(-155);
  chassis.turn_to_angle(45);
  chassis.drive_distance(-60);
  chassis.turn_to_angle(0);
  chassis.drive_distance(5);
  chassis.drive_distance(-60);
  wait(200, msec);
  chassis.turn_to_angle(93);
  chassis.drive_distance(30);
  chassis.turn_to_angle(135);
  chassis.drive_distance(30);
  chassis.turn_to_angle(75);
  deployBackWings();
  wait(200, msec);
  chassis.drive_distance(-60);
  chassis.drive_distance(10);
  deployBackWings();
  chassis.drive_distance(25);
  chassis.turn_to_angle(0);
  chassis.drive_distance(-30);
  chassis.turn_to_angle(115);
  deployBackWings();
  wait(200, msec);
  chassis.drive_distance(-60);
  chassis.drive_distance(10);
  deployBackWings();
  chassis.drive_distance(25);
  chassis.turn_to_angle(0);
  chassis.drive_distance(-10);
  chassis.turn_to_angle(90);
  deployBackWings();
  wait(200, msec);
  chassis.drive_distance(-40);
  chassis.drive_distance(10);
  deployBackWings();
  chassis.drive_distance(20);
  wait(5, seconds);
  chassis.left_swing_to_angle(-110);
  chassis.drive_distance(70);
  chassis.turn_to_angle(180);
  chassis.drive_distance(10);
  chassis.turn_to_angle(160);
  deployBackWings();
  wait(200, msec);
  chassis.drive_distance(-40);
}

void rightautonDev() {
  intakecontrolEnabled = true;
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  wingsDeployRetract();
  endgame();
  intakeMotor2.spin(forward, 12.7, volt);
  wait(200, msec);
  wingsDeployRetract();
  endgame();

  chassis.turn_to_angle(-90);
  vex::task iC( intakeControl );
  chassis.drive_distance(28);

  chassis.turn_to_angle(-45);

  //wingsDeployRetract();

  chassis.drive_distance(26);

  chassis.left_swing_to_angle(0);

  chassis.drive_distance(2);

  chassis.turn_to_angle(-2);

  intakecontrolEnabled = false;

  intakeMotor2.spin(reverse, 100, percent);

  wait(250, msec);

  chassis.turn_to_angle(0);

  chassis.drive_distance(-6);
  //wingsDeployRetract();

  chassis.turn_to_angle(180);

  chassis.drive_distance(-16);

  //wingsDeployRetract();

  chassis.drive_distance(22);

  chassis.turn_to_angle(-45);

  chassis.drive_distance(6);

  deployBackWings();

  wait(150, msec);

  chassis.drive_distance(-10);

  chassis.left_swing_to_angle(-90);

  chassis.turn_to_angle(-90);

  chassis.drive_distance(-8);

  chassis.turn_to_angle(180);
  wingsDeployRetract();
  wait(250,msec);
  wingsDeployRetract();
}

// 1 if by skills 2 if by right and 3 if by left 0 if by stupid (none loaded)
void autonType(int autonSelect) {
  // select different types of auton

  switch (autonSelect) {
    case 0:
      Brain.Screen.print("No Auton Loaded. Skipping...");
      set_screen_color(1);
      break;
    case 1:
      Brain.Screen.print("Skills Auton Loaded.");
      skillsAuton();
      break;
    case 2:
      Brain.Screen.print("Competition Auton Loaded. R");
      competitionAutonR();
      break;
    case 3:
      Brain.Screen.print("Competition Auton Loaded. L");
      competitionAutonL();
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


void autonP3()
{
  default_constants();
 // endgame();
  //wait(200, msec);
  //endgame();
  
  chassis.turn_to_angle(-30);
  chassis.drive_distance(-40);
  chassis.turn_to_angle(-20);
  chassis.drive_distance(-20);
  chassis.drive_distance(20);
  chassis.turn_to_angle(-112);
  chassis.drive_distance(-4);
  chassis.left_swing_to_angle(-115);
  /* -- CATAPULT TAKES ~APPROX 23 secs
  thread task1(allowforskillsCata); // creates timer thread for catapult in skills
  task1.detach(); // "allows" for execution from handle
  spincataPerc(65,true);
  while (halter != false) {
    wait(2, msec); // sit there and wait while catapult is spinning
    armElevator3.stop(hold);
  }*/
  chassis.set_coordinates(0,0,-115);
  chassis.drive_distance(25);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(15);
  chassis.turn_to_angle(0);
  chassis.drive_distance(-80);
  
  //chassis.drive_distance(20);
  //chassis.turn_to_angle(-90);
 // chassis.drive_distance(40);
}

void autonomous(void) {

  //autonType(type);
  //leftAutondev();
  //skillsautonDev();
  //skillsAuton();
  //chassis.turn_to_angle(180);
  /*armElevator3.spin(forward, 12.7, volt);
  wait(500, msec);
  armElevator3.stop(coast);
  wait(500,msec);
  armElevator3.spinFor(reverse, 90, degrees);
  catapaultMotor14.spin(forward, 100, percent);*/

  skillsAuton();
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


void drive_User(){
  // tank drive user control left side on left right side on right
  if (current_forwards == true) {
    tlMotor18.spin(vex::directionType::fwd, ((mainController.Axis3.value() + mainController.Axis1.value())/10), volt);
    mlMotor19.spin(vex::directionType::fwd, ((mainController.Axis3.value() + mainController.Axis1.value())/10), volt);
    blMotor20.spin(vex::directionType::fwd, ((mainController.Axis3.value() + mainController.Axis1.value())/10), volt);
    trMotor13.spin(vex::directionType::fwd, ((mainController.Axis3.value() - mainController.Axis1.value())/10), volt);
    mrMotor12.spin(vex::directionType::fwd, ((mainController.Axis3.value() - mainController.Axis1.value())/10), volt);
    brMotor11.spin(vex::directionType::fwd, ((mainController.Axis3.value() - mainController.Axis1.value())/10), volt);
  } else {
    tlMotor18.spin(vex::directionType::rev, ((mainController.Axis3.value() - mainController.Axis1.value())/10), volt);
    mlMotor19.spin(vex::directionType::rev, ((mainController.Axis3.value() - mainController.Axis1.value())/10), volt);
    blMotor20.spin(vex::directionType::rev, ((mainController.Axis3.value() - mainController.Axis1.value())/10), volt);
    trMotor13.spin(vex::directionType::rev, ((mainController.Axis3.value() + mainController.Axis1.value())/10), volt);
    mrMotor12.spin(vex::directionType::rev, ((mainController.Axis3.value() + mainController.Axis1.value())/10), volt);
    brMotor11.spin(vex::directionType::rev, ((mainController.Axis3.value() + mainController.Axis1.value())/10), volt);
  }

  // brake 
  if (mainController.ButtonB.pressing() == true) {
    motorsHalt();
  }

  // spin catapault
  if (mainController.ButtonY.pressing() == true){
    spincataPerc(100.0, true);
  } else {
    catapaultMotor14.stop(coast);
    catapaultMotor4.stop(coast);
  }

  /*if (mainController.ButtonRight.pressing() == true && percent25 == true) {
    //spincataPercrev(25.0, false);
    catapaultMotor14.spin(reverse, 25, percent);
  } else if (mainController.ButtonRight.pressing() == true && percent25 == false) {
    //spincataPercrev(73.0, false);
    catapaultMotor14.spin(reverse, 75, percent);
  }*/

  //deploy wings
  mainController.ButtonL2.pressed(wingsDeployRetract);

  mainController.ButtonL1.pressed(deployBackWings);

  mainController.ButtonX.pressed(motorReverse);

  mainController.ButtonA.pressed(endgame);

  // for intake
  if (mainController.ButtonR2.pressing() == true ) {
    intakeMotor2.spin(forward, 12.5, volt);
  } else if (mainController.ButtonR1.pressing() == true) {
    intakeMotor2.spin(reverse, 12.5, volt);
  } else {
    intakeMotor2.stop(coast);
  }

 /*if (mainController.ButtonUp.pressing() != false) {
    armElevator3.spin(reverse, 60, pct);
  } else if (mainController.ButtonDown.pressing() != false) {
    armElevator3.spin(forward, 60, pct);
  } else {
    armElevator3.stop(hold);
  }*/
}

void usercontrol(void) {
  // User control code here, inside the loop
  //wingsDeployRetract();
  //skillsautoPos();
  setdtBrakemode(coast);
  while (1) {
   drive_User();
  }
}

//
// Main will set up the competition functions and callbacks.
//
void assess() {
  wait(2, sec);
  if (type == 0) {
      skillsautoPos();
      setdtBrakemode(coast);
      while(true) {
        usercontrol();
      }
  } else {
    wait(2, sec);
    autonType(type);
  }
}

int main() {
  //wingsDeployRetract();
  pre_auton();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  //assess();
  /*if (type == 0) {
    usercontrol();
  } else {
    autonomous();
  }*/
  /*while(type == -1) {
    wait(1, msec);
  }*/
  //autonType(type);
}