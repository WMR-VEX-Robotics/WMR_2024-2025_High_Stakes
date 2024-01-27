#include "vex.h"

using namespace vex;
competition Competition;

brain Brain;

//Other Devices
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
pneumatics wall = pneumatics(Brain.ThreeWirePort.H);
pneumatics hanglock = pneumatics(Brain.ThreeWirePort.G);

bool reversed_controls = true;

//Motor Devices
motor LeftFront = motor(PORT18, ratio6_1, true);
motor LeftRear = motor(PORT19, ratio6_1, true);
motor LeftAux = motor(PORT20, ratio6_1, true);
motor RightFront = motor(PORT9, ratio6_1, false);
motor RightRear = motor(PORT8, ratio6_1, false);
motor RightAux = motor(PORT6, ratio6_1, false);

motor Cata = motor(PORT17, ratio18_1, false);
motor IntakeVacuum = motor(PORT5, ratio18_1, true);



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
ZERO_TRACKER_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(LeftFront,LeftRear, LeftAux),

//Right Motors:
motor_group(RightFront,RightRear, RightAux),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT7,

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
PORT4,

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
bool driver_skills = true;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!  
  vexcodeInit();
  default_constants();
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
  default_constants();
  Brain.Screen.clearScreen();            //brain screen for auton selection.
    switch(current_auton_selection){       //Tap the brain screen to cycle through autons.
      case 0:
        default_constants();
        Brain.Screen.print("Auton Running: Qual Match, Left Side");
        default_constants();
        LeftFront.setVelocity(100, percent);
        LeftRear.setVelocity(100, percent);
        RightFront.setVelocity(100, percent);
        RightRear.setVelocity(100, percent);
        Brain.Screen.print("Auton Running: Qual Match, Right Side");
        chassis.right_swing_to_angle(32);
        chassis.drive_distance(-40);
        chassis.left_swing_to_angle(32);
        chassis.drive_distance(15);
        chassis.turn_to_angle(-167);
        chassis.drive_distance(-15);
        wall.open();
        wait(1,seconds);
        chassis.turn_to_angle(-90);
        wall.close();
        chassis.turn_to_angle(-120);
        chassis.drive_distance(-20);
        chassis.turn_to_angle(-90);
        chassis.drive_distance(-23.5);
        wall.open();
        wait(0.5,seconds);
        chassis.left_swing_to_angle(-130);
        break;
      case 1:
        default_constants();
        LeftFront.setVelocity(100, percent);
        LeftRear.setVelocity(100, percent);
        RightFront.setVelocity(100, percent);
        RightRear.setVelocity(100, percent);
        Brain.Screen.print("Auton Running: Qual Match, Right Side");
        chassis.left_swing_to_angle(-32);
        chassis.drive_distance(-40);
        chassis.right_swing_to_angle(-32);
        chassis.drive_distance(15);
        chassis.turn_to_angle(167);
        chassis.drive_distance(-15);
        wall.open();
        wait(1,seconds);
        chassis.turn_to_angle(90);
        wall.close();
        chassis.turn_to_angle(120);
        chassis.drive_distance(-20);
        chassis.turn_to_angle(90);
        chassis.drive_distance(-23.5);
        wall.open();
        wait(0.5,seconds);
        chassis.right_swing_to_angle(130);
        //chassis.drive_distance(-12);
        //chassis.drive_distance(8);
        //chassis.left_swing_to_angle(-90);
        //chassis.drive_distance(30);
        //wall.close();
        //chassis.drive_distance(30);
        break;
      case 2:
        Brain.Screen.print("Auton Running: Qual Match, Right Side Alternate");
        default_constants();
        LeftFront.setVelocity(100, percent);
        LeftRear.setVelocity(100, percent);
        RightFront.setVelocity(100, percent);
        RightRear.setVelocity(100, percent);
        chassis.drive_distance(-50);
        wait(1,seconds);
        chassis.drive_distance(20);
        chassis.turn_to_angle(-5);
        wall.open();
        wait(250,msec);
        chassis.drive_distance(10);
        chassis.turn_to_angle(-10);
        wall.close();
        chassis.turn_to_angle(-60);
        chassis.drive_distance(4);
        chassis.turn_to_angle(125);
        chassis.drive_distance(-40);
        //IntakeVacuum.spinFor(2000,degrees,false);
        //wait(1,sec);
        //wall.close();
        break;
      case 3:
        Brain.Screen.print("Dummy plug system engaged: Running: Skills Auton, Shooting Left");
        IntakeVacuum.spinFor(-1200,degrees,false);
        chassis.drive_distance(20);
        chassis.turn_to_angle(70);
        chassis.drive_distance(-19);
        chassis.turn_to_angle(70);
        Cata.setVelocity(60,pct);
        Cata.spinFor(35,seconds);
        chassis.set_coordinates(-14, 14.9, 70);
        chassis.turn_to_angle(110);
        chassis.drive_distance(24);
        chassis.turn_to_angle(90);
        chassis.drive_distance(70);
        chassis.turn_to_angle(0);
        chassis.drive_distance(20);
        chassis.turn_to_angle(-45);
        chassis.drive_distance(23);
        chassis.turn_to_angle(0);
        chassis.drive_distance(20);
        chassis.turn_to_angle(90);
        wall.open();
        chassis.drive_distance(33);
        wall.close();
        chassis.drive_distance(-33);
        chassis.turn_to_angle(0);
        chassis.drive_distance(10);
        chassis.turn_to_angle(90);
        wall.open();
        chassis.drive_distance(33);
        wall.close();
        chassis.drive_distance(-33);
        chassis.turn_to_angle(0);
        chassis.drive_distance(10);
        chassis.turn_to_angle(90);
        wall.open();
        chassis.drive_distance(33);
        wall.close();
        chassis.drive_distance(-33);
        //LeftCata.setVelocity(55, percent);
        //ightCata.setVelocity(55, percent);
        //LeftCata.spinFor(25100,degrees,false);
        //RightCata.spinFor(25100,degrees,false);
        Brain.Screen.print("Dummy plug system disengaged.");
        break;
      case 4:
        Brain.Screen.print("Dummy plug system engaged: Running: Skills Auton, Shooting Left");
        IntakeVacuum.spinFor(-1200,degrees,false);
        chassis.drive_distance(20);
        chassis.turn_to_angle(70);
        chassis.drive_distance(-19);
        chassis.turn_to_angle(70);
        Cata.setVelocity(60,pct);
        Cata.spinFor(35,seconds);
        //chassis.turn_to_angle(70);
        //chassis.drive_distance(-15);
        //Cata.setVelocity(60,pct);
        //Cata.spinFor(35,seconds);
        //Brain.Screen.print("Dummy plug system disengaged.");
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


void disengagelock(){
  hanglock.open();
  Brain.Screen.printAt(0,50, "open");
}

void engagelock() {
  hanglock.close();
  Brain.Screen.printAt(0,50, "closed");
}

void lock_hang() {
  if (hanglock.value() == true) {
    hanglock.close();
  } else {
    hanglock.open();
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
    Cata.spin(forward, 60, percent);
  } else {
    Cata.stop(hold);
  }

  /*if (Controller1.ButtonL2.pressing() == true){
    travelMode();
  }*/
  //@TODO: Create Travel Mode Toggle

  Controller1.ButtonB.pressed(pneumaticsSwitch);
  Controller1.ButtonY.pressed(lock_hang);
  
  if (Controller1.ButtonA.pressing() == true){
    reverse_drive();
    wait(200,msec);
  }
}

void usercontrol(void) {
  //current_auton_selection=4;
  //autonomous();
  reversed_controls = false;
  while(1){standardControl_1();}
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  wall.close();
  //hanglock.open();
  // Set up callbacks for autonomous and driver control periods.
  pre_auton();
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  //Brain.Screen.print("Dummy plug system engaged: Running: Skills Auton, Shooting Left");

  // Run the pre-autonomous function.
  
}
