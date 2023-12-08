#include "vex.h"

using namespace vex;
competition Competition;

brain Brain;

//Other Devices
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
pneumatics wall = pneumatics(Brain.ThreeWirePort.E);

bool reversed_controls = false;

//Motor Devices
motor LeftFront = motor(PORT20, ratio6_1, true);
motor LeftRear = motor(PORT9, ratio6_1, true);
motor RightFront = motor(PORT11, ratio6_1, false);
motor RightRear = motor(PORT14, ratio6_1, false);
motor LeftCata = motor(PORT19, ratio18_1, false);
motor RightCata = motor(PORT12, ratio18_1, true);
motor IntakeVacuum = motor(PORT18, ratio18_1, true);
motor HangingArm = motor(PORT13,ratio36_1, false);
motor Flipper = motor(PORT16,ratio18_1, false);



/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors using the V5 port icon in the top right of the screen. Doing     */
/*  so will update robot-config.cpp and robot-config.h automatically, so     */
/*  you don't have to. Ensure that your motors are reversed properly. For    */
/*  the drive, spinning all motors forward should drive the robot forward.   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your robot manually with the sidebar configurer. */
/*---------------------------------------------------------------------------*/

Drive chassis(

//Specify your drive setup below. There are seven options:
//ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
//For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
TANK_TWO_ROTATION,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(LeftFront,LeftRear),

//Right Motors:
motor_group(RightFront,RightRear),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT4,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
0.6,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
360,

//---------------------------------------------------------------------------
//                                  PAUSE!                                   
//                                                                           
//  The rest of the drive constructor is for robots using POSITION TRACKING. 
//  If you are not using position tracking, leave the rest of the values as  
//  they are.                                                                
//---------------------------------------------------------------------------

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
PORT3,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
0,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
PORT2,

//Sideways tracker diameter (reverse to make the direction switch):
2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
0

);

int current_auton_selection = 0;
bool auto_started = true;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!  
  vexcodeInit();
  default_constants();
  HangingArm.setBrake(hold);
  HangingArm.setVelocity(100, percent);
  IntakeVacuum.setVelocity(100, percent);
  chassis.set_coordinates(0, 0, 0);


  while(auto_started == false){            //Changing the names below will only change their names on the
    Brain.Screen.clearScreen();            //brain screen for auton selection.
    switch(current_auton_selection){       //Tap the brain screen to cycle through autons.
      case 0:
        Brain.Screen.printAt(50, 50, "Auton Selected: Qual Match, Left Side");
        break;
      case 1:
        Brain.Screen.printAt(50, 50, "Auton Selected: Qual Match, Right Side");
        break;
      case 2:
        Brain.Screen.printAt(50, 50, "Auton Selected: Skills Auton, Shooting Left");
        break;
      case 3:
        Brain.Screen.printAt(50, 50, "Auton Selected: Skills Auton, Shooting Right");
        break;
      case 4:
        Brain.Screen.printAt(50, 50, "Full Test");
        break;
      case 5:
        Brain.Screen.printAt(50, 50, "Odom Test");
        break;
      case 6:
        Brain.Screen.printAt(50, 50, "Tank Odom Test");
        break;
      case 7:
        Brain.Screen.printAt(50, 50, "Holonomic Odom Test");
        break;
    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection ++;
    } else if (current_auton_selection == 8){
      current_auton_selection = 0;
    }
    task::sleep(10);
  }
}

void autonomous(void) {
  Brain.Screen.clearScreen();            //brain screen for auton selection.
    switch(current_auton_selection){       //Tap the brain screen to cycle through autons.
      case 0:
        Brain.Screen.print("Auton Running: Qual Match, Left Side");
        HangingArm.spinFor(1465,degrees,false);
        IntakeVacuum.spinFor(6000,degrees,false);
        chassis.drive_distance(-15);
        chassis.drive_distance(15);
        chassis.set_drive_constants(2.5, 1.5, .03, 10, 0);
        chassis.drive_distance(-15);
        //wait(2,sec);
        HangingArm.spinFor(-1465,degrees,false);
        IntakeVacuum.spinFor(-6000,degrees,false);
        wait(1,seconds);
        chassis.set_drive_constants(11, 1.5, .03, 10, 0);
        chassis.drive_distance(-20);
        chassis.turn_to_angle(45);
        chassis.drive_distance(-20);
        chassis.turn_to_angle(90);
        HangingArm.spinFor(1300,degrees,false);
        wait(0.5,seconds);
        chassis.drive_distance(-25);
        chassis.turn_to_angle(86);
        chassis.drive_distance(55);
        default_constants();
        //HangingArm.spinFor(-1430,degrees,false);
        break;
      case 1:
        Brain.Screen.print("Auton Running: Qual Match, Right Side");
        break;
      case 2:
        Brain.Screen.print("Auton Running: Skills Auton, Shooting Left");
        HangingArm.spinFor(1315,degrees,false);
        chassis.drive_distance(5);
        chassis.turn_to_angle(-45);
        chassis.drive_distance(7);
        chassis.turn_to_angle(35);
        chassis.drive_distance(-10);
        chassis.turn_to_angle(35);
        LeftCata.setVelocity(55, percent);
        RightCata.setVelocity(55, percent);
        LeftCata.spinFor(25100,degrees,false);
        RightCata.spinFor(25100,degrees,false);
        wait(35,sec);
        HangingArm.spinFor(-600,degrees,false);
        chassis.set_coordinates(0, 0, 0);
        chassis.set_heading(-10);
        chassis.turn_to_angle(45);
        chassis.drive_distance(28);
        chassis.turn_to_angle(-180);
        chassis.drive_distance(-43);
        HangingArm.spinFor(-730,degrees,false);
        break;
      case 3:
        Brain.Screen.print("Auton Running: Skills Auton, Shooting Right");
        break;
    }

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

void openWall(){
  wall.open();
  Brain.Screen.printAt(0,50, "open");
}

void closeWall() {
  wall.close();
  Brain.Screen.printAt(0,50, "closed");
}

void pneumaticsSwitch() {
  if (wall.value() == true) {
    wall.close();
  } else {
    wall.open();
  }
}

void reverse_drive() {
  //Brain.Screen.clearScreen();  
  reversed_controls = !reversed_controls;
  Brain.Screen.print(reversed_controls);
  //LeftFront.setReversed(!reversed_controls);
  //LeftRear.setReversed(!reversed_controls);
  //RightFront.setReversed(reversed_controls);
  //RightRear.setReversed(reversed_controls);
  task::sleep(200);
}

/*void travelMode() {
  int timer = 0;
  while(LeftCata.torque(Nm)>0.10 && timer<200){
    LeftCata.spin(forward,100,pct);
    RightCata.spin(forward,100,pct);
    timer+=1;
  }
  LeftCata.setVelocity(100,pct);
  RightCata.setVelocity(100,pct);
  LeftCata.spinFor(300,degrees,false);
  RightCata.spinFor(300,degrees,false);
  LeftCata.setBrake(hold);
  RightCata.setBrake(hold);
  wait(1,seconds);
}*/



void standardControl_1(){
  //Start Controller1 Scheme
  //Two Stick Arcade Mode
  
  chassis.control_arcade(reversed_controls);

  //Run intake inwards/outwards bound r1,r2
  if (Controller1.ButtonL1.pressing() == true){
    IntakeVacuum.spin(forward, 80, percent);
  } else if (Controller1.ButtonR1.pressing() == true){
    IntakeVacuum.spin(reverse, 80, percent);
  } else {
    IntakeVacuum.stop(hold);
  }

  //Run catapault
  if (Controller1.ButtonR2.pressing() == true){
    LeftCata.spin(forward, 60, percent);
    RightCata.spin(forward, 60, percent);
  } else {
    LeftCata.stop(hold);
    RightCata.stop(hold);
  }

  //flipper
  if (Controller1.ButtonLeft.pressing() == true){
    Flipper.spin(forward, 80, percent);
  } else if (Controller1.ButtonRight.pressing() == true){
    Flipper.spin(reverse, 80, percent);
  } else {
    Flipper.stop(hold);
  }

  /*if (Controller1.ButtonL2.pressing() == true){
    travelMode();
  }*/
  //@TODO: Create Travel Mode Toggle

  Controller1.ButtonB.pressed(pneumaticsSwitch);
  if (Controller1.ButtonA.pressing() == true){
    reverse_drive();
    wait(200,msec);
  }

  if (Controller1.ButtonUp.pressing() == true){
    HangingArm.spin(forward, 100, pct);
  } else if (Controller1.ButtonDown.pressing() == true){
    HangingArm.spin(reverse, 100, pct);
  } else {
    HangingArm.stop(hold);
  }
  //End Controller1 Scheme

}

void usercontrol(void) {
  while(1){standardControl_1();}
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  wall.close();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  //chassis.turn_to_angle(20);

}