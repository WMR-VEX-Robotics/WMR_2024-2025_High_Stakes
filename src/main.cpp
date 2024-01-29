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
motor tlMotor17 = motor(PORT17, ratio6_1, true);
motor mlMotor19 = motor(PORT19, ratio6_1, true);
motor blMotor20 = motor(PORT20, ratio6_1, true);
motor trMotor13 = motor(PORT16, ratio6_1, false);
motor mrMotor12 = motor(PORT7, ratio6_1, false);
motor brMotor11 = motor(PORT14, ratio6_1, false);
/*motor tlMotor17 = motor(PORT17, ratio18_1, true);
motor mlMotor19 = motor(PORT19, ratio18_1, true);
motor blMotor20 = motor(PORT20, ratio18_1, true);
motor trMotor13 = motor(PORT13, ratio18_1, false);
motor mrMotor12 = motor(PORT12, ratio18_1, false);
motor brMotor11 = motor(PORT11, ratio18_1, false);*/

motor catapaultMotor4 = motor(PORT1, ratio18_1, true);
motor intakeMotor2 = motor(PORT2, ratio18_1, true);
motor armElevator3 = motor(PORT5, ratio18_1, true);
// not motors
controller mainController = controller(primary);
inertial inertialSensor8 = inertial(PORT9);
//encoder enc1 = encoder(Brain.ThreeWirePort.A);
//rotation rot1 = rotation(PORT8, true);
pneumatics solonoidF = pneumatics(Brain.ThreeWirePort.F);
pneumatics solonoidH = pneumatics(Brain.ThreeWirePort.H);
pneumatics solonoidG = pneumatics(Brain.ThreeWirePort.G);

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
    armElevator3.stop(coast);
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
motor_group(tlMotor17, mlMotor19, blMotor20),

//Right Motors:
motor_group(trMotor13, mrMotor12, brMotor11),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT9,

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

// must always start as true!
bool halter = true;

void allowforskillsCata() {
  for (int i = 0; i <= 30 /*2*/; i++) {
    wait(985, msec);

    vex::task::sleep(15);
  }
  catapaultMotor4.stop(coast);
  intakeMotor2.stop(coast);
  chassis.set_coordinates(0,0,0);
  halter = false;
}

void motorsHalt(){
  // stop the motor with implicit type brake
  tlMotor17.stop(brake);
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
  tlMotor17.setStopping(mode);
  mlMotor19.setStopping(mode);
  blMotor20.setStopping(mode);
  trMotor13.setStopping(mode);
  mrMotor12.setStopping(mode);
  brMotor11.setStopping(mode);
}

void ensureCalibration(){
  // MAKE SURE THE INERTIAL SENSOR CALIBRATES
  if (inertialSensor8.isCalibrating() != false){
    wait(200,msec);
  }
}

double percentasVolt(double n) {
  double increment = 0.127;
  return increment * n;
}

// volt = false percent = true
void spincataPerc(double P, bool inPercent) {
  if (inPercent) {
    catapaultMotor4.spin(forward, P, percent);
  } else {
    catapaultMotor4.spin(forward, percentasVolt(P), volt);
  }
}

void spincataPercrev(double P, bool inPercent) {
  if (inPercent) {
    catapaultMotor4.spin(reverse, P, percent);
  } else {
    catapaultMotor4.spin(reverse, percentasVolt(P), volt);
  }
}

bool current_forwards = true;

void motorReverse() {
  //motor tlMotor17 = motor(PORT12, ratio6_1, false);
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

  int originX = 10;
  int originY = 20;
  int width = 80;
  int height = 80;
  // purge inertial sensor
  inertialSensor8.calibrate();
  // ensure the inertial sensor is done calibrating before continuing on
  ensureCalibration();
  Brain.Screen.drawRectangle(det_Positionon_screen('X', 0, originX, 0), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // purge encoder(s) rotation value
  //enc1.resetRotation();

  //purge rotation values
  //rot1.resetPosition();
  /*if (rot1.position(deg) != 0) {
    rot1.setPosition(0, deg);
  }*/

  // set brakes to the defined mode in the case of autonomous function it should be brake
  setdtBrakemode(brake);
  Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 1), det_Positionon_screen('Y', 0, originY, 0), width, height, green);
  //mode is reset at the end of autonomous
  Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 2), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // to prevent catapault motor strain we will set the motors to coast
  catapaultMotor4.setStopping(coast);
  Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 3), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // ensure that the first solonoid is registering as closed visually confirming this fact
  solonoidH.set(false);
  solonoidH.close();
  solonoidF.close();
  Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 4), det_Positionon_screen('Y', 0, originY, 0), width, height, green);

  // ensure that the second solonoid is registering as closed visually confirming this fact
  solonoidG.set(false);
  solonoidG.close();
  Brain.Screen.drawRectangle(det_Positionon_screen('X', width, originX, 5), det_Positionon_screen('Y', 0, originY, 0), width, height, green);
  wait(25, msec);
  Brain.Screen.clearScreen();

  autonSelect_buttons();
  if (type != 0 && type != 4) {
    color_select();
  }

}

// for competition
void competitionAutonL(){
  armElevator3.stop(hold);
// set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  armElevator3.spin(forward, 12.7, volt);
  wait(500, msec);
  armElevator3.stop(coast);

  intakeMotor2.spin(forward, 12.7, volt);
  
  chassis.drive_distance(4);

  chassis.turn_to_angle(-56);
  
  chassis.drive_distance(30);

  chassis.turn_to_angle(0);

  intakeMotor2.spin(reverse, 12.7, volt);

  chassis.drive_distance(-2);

  chassis.turn_to_angle(180);
  
  intakeMotor2.stop(hold);

  chassis.drive_distance(-8);

  chassis.drive_distance(6);

  chassis.drive_distance(-6);

  chassis.drive_distance(6);

  chassis.turn_to_angle(270);

  wait(5, seconds);

  chassis.drive_distance(-10);
 
  chassis.turn_to_angle(225);

  chassis.drive_distance(18);

  chassis.drive_distance(-6);

  chassis.drive_distance(3);

  chassis.turn_to_angle(270);

  intakeMotor2.stop(coast);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);
  set_screen_color(team);
}

//blegh
void goofy_auton() {
  default_constants();
  // initialize position as (0,0,0)
    chassis.set_coordinates(0,0,0);

  while (true) {
    for (int i = 0; i <= 3; i++) {
      chassis.turn_to_angle(90*i);
      chassis.drive_distance(100);
    }
  }
}

// for competition
void competitionAutonR(){
  // set operating constant to their default values
  armElevator3.stop(hold);
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  armElevator3.spin(reverse, 12.7, volt);
  wait(500, msec);
  armElevator3.stop(coast);

  intakeMotor2.spin(forward, 12.7, volt);
  
  chassis.drive_distance(8);

  chassis.turn_to_angle(56);
  
  chassis.drive_distance(33);

  chassis.turn_to_angle(0);

  intakeMotor2.spin(reverse, 12.7, volt);

  chassis.drive_distance(-2);

  chassis.turn_to_angle(180);

  intakeMotor2.stop(hold);

  chassis.drive_distance(-8);

  chassis.drive_distance(6);

  chassis.drive_distance(-6);

  chassis.drive_distance(6);

  chassis.turn_to_angle(90);

  wait(5, seconds);

  chassis.drive_distance(-10);

  chassis.drive_distance(3);

  chassis.drive_distance(-6);

  chassis.drive_distance(3);

  chassis.turn_to_angle(270);

  intakeMotor2.stop(coast);

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);

  set_screen_color(team);
}

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

void endgame() {
  if (solonoidF.value() == true) {
    solonoidF.close();
  } else {
    solonoidF.open();
  }
}

// for skills
void skillsAuton() {
  //wingsDeployRetract();
  // set operating constant to their default values
  default_constants();
  // initialize position as (0,0,0)
  chassis.set_coordinates(0,0,0);

  armElevator3.spin(forward, 12.7, volt);
  wait(500, msec);
  armElevator3.stop(coast);


  // begin the fun program of skills auton
  chassis.drive_distance(6);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(7);
  catapaultMotor4.stop(coast);
  chassis.turn_to_angle(45);
  chassis.drive_distance(-12);
  chassis.turn_to_angle(55);
  catapaultMotor4.stop(brake);

  spincataPerc(75.0, true); // spins catapult at a given percent (swapping bool allows for different precisions)
  intakeMotor2.spin(reverse, 12.7, volt);


  thread task1(allowforskillsCata); // creates timer thread for catapult in skills
  task1.detach(); // "allows" for execution from handle

  // gross programming but if the bot moves the bot moves. It shouldn't.
  while (halter != false) {
    wait(2, msec); // sit there and wait while catapult is spinning
  }

  //wait(2, sec);

  intakeMotor2.spin(forward, 12.7, volt);

  chassis.drive_distance(1);
  //chassis.turn_to_angle(113);
  //chassis.drive_distance(12);
  chassis.turn_to_angle(35);
  chassis.drive_distance(80);
  chassis.set_coordinates(0, 0, 0);
  wait(10, msec);
  chassis.turn_to_angle(90);
  chassis.drive_distance(-18);

  chassis.turn_to_angle(45);
  //-45
  chassis.drive_distance(-34);

  chassis.turn_to_angle(90);
  //360

  chassis.drive_distance(-12);

  chassis.turn_to_angle(180);
  //90

  wingsDeployRetract();
  chassis.drive_distance(-36);
  wingsDeployRetract();
  chassis.drive_distance(34);
  chassis.turn_to_angle(90);
  //360 

  chassis.drive_distance(-20);

  chassis.turn_to_angle(185);

  wingsDeployRetract();
  chassis.drive_distance(-36);
  wingsDeployRetract();
  /*chassis.turn_to_angle(155);
  chassis.drive_distance(38);
  chassis.turn_to_angle(180);
  intakeMotor2.spinFor(360, degrees, false);
  chassis.drive_distance(18);
  chassis.turn_to_angle(90);
  chassis.drive_distance(40);*/

  // set the mode of braking to coast for user post execution
  setdtBrakemode(coast);

  set_screen_color(team);
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
  autonType(type);
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
bool percent25 = true;

void swapPerc() {
  if (percent25 != true) {
    percent25 = true;
    wait(150, msec);
  } else {
    percent25 = false;
    wait(150, msec); 
  }
}

void drive_User(){
  // tank drive user control left side on left right side on right
  if (current_forwards == true) {
    tlMotor17.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    mlMotor19.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    blMotor20.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    trMotor13.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    mrMotor12.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    brMotor11.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
  } else {
    tlMotor17.spin(vex::directionType::rev, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    mlMotor19.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    blMotor20.spin(vex::directionType::rev, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    trMotor13.spin(vex::directionType::rev, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    mrMotor12.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    brMotor11.spin(vex::directionType::rev, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
  }

  // brake 
  if (mainController.ButtonB.pressing() == true) {
    motorsHalt();
  }

  // spin catapault
  if (mainController.ButtonL1.pressing() == true){
    if (percent25 == true){
      spincataPerc(100.0, true);
    } else {
      spincataPerc(100.0, false);
    }
  } else {
    catapaultMotor4.stop(coast);
  }

  /*if (mainController.ButtonRight.pressing() == true && percent25 == true) {
    //spincataPercrev(25.0, false);
    catapaultMotor4.spin(reverse, 25, percent);
  } else if (mainController.ButtonRight.pressing() == true && percent25 == false) {
    //spincataPercrev(73.0, false);
    catapaultMotor4.spin(reverse, 75, percent);
  }*/

  //deploy wings
  mainController.ButtonL2.pressed(wingsDeployRetract);

  mainController.ButtonY.pressed(swapPerc);

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

  if (mainController.ButtonUp.pressing() != false) {
    armElevator3.spin(reverse, 60, pct);
  } else if (mainController.ButtonDown.pressing() != false) {
    armElevator3.spin(forward, 60, pct);
  } else {
    armElevator3.stop(hold);
  }
}

void reporter() {
  //while (true) {
    int X = mainController.Axis1.value();
    int Y = mainController.Axis3.value();
    printf("Turning @ %d\n", X);
    printf("Driving @ %d\n", Y);
    if (mainController.ButtonB.pressing() == true) {
      printf("B Pressed \n");
    }
    if (mainController.ButtonX.pressing() == true) {
      printf("X Pressed \n");
    }
    if (mainController.ButtonR1.pressing() == true) {
      printf("R1 Pressed \n");
    }
    if (mainController.ButtonR2.pressing() == true) {
      printf("R2 Pressed \n");
    }
    if (mainController.ButtonL1.pressing() == true) {
      printf("L1 Pressed \n");
    }
    if (mainController.ButtonL2.pressing() == true) {
      printf("L2 Pressed \n");
    }    
    if (mainController.ButtonUp.pressing() == true) {
      printf("Up Pressed \n");
    }    
    if (mainController.ButtonDown.pressing() == true) {
      printf("Down Pressed \n");
    }
    vex::task::sleep(250);
  //}
}

void diagdrive_User(){
  // tank drive user control left side on left right side on right
  if (current_forwards == true) {
    tlMotor17.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    mlMotor19.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    blMotor20.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    trMotor13.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    mrMotor12.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    brMotor11.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
  } else {
    tlMotor17.spin(vex::directionType::rev, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    mlMotor19.spin(vex::directionType::fwd, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    blMotor20.spin(vex::directionType::rev, (mainController.Axis3.value() - mainController.Axis1.value()), vex::velocityUnits::pct);
    trMotor13.spin(vex::directionType::rev, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    mrMotor12.spin(vex::directionType::fwd, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
    brMotor11.spin(vex::directionType::rev, (mainController.Axis3.value() + mainController.Axis1.value()), vex::velocityUnits::pct);
  }

  // brake 
  if (mainController.ButtonB.pressing() == true) {
    motorsHalt();
  }

  // spin catapault
  if (mainController.ButtonL1.pressing() == true){
    spincataPerc(73.0, false);
  } else {
    catapaultMotor4.stop(coast);
  }

  //deploy wings
  mainController.ButtonL2.pressed(wingsDeployRetract);


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

  if (mainController.ButtonUp.pressing() != false) {
    armElevator3.spin(reverse, 60, pct);
  } else if (mainController.ButtonDown.pressing() != false) {
    armElevator3.spin(forward, 60, pct);
  } else {
    armElevator3.stop(hold);
  }

  thread task1(allowforskillsCata); // creates timer thread for catapult in skills
  task1.detach(); // "allows" for execution from handle
}

void usercontrol(void) {
  // User control code here, inside the loop
  //wingsDeployRetract();
  while (1) {
   drive_User();
  }
}

//
// Main will set up the competition functions and callbacks.
//
void assess() {
    while (type == 0) {
    diagdrive_User();
  }
  wait(2, sec);
  autonType(type);
}

int main() {
  //wingsDeployRetract();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  assess();
  //autonType(type);
}