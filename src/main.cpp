#include "vex.h"

using namespace vex;
competition Competition;

brain Brain;

//Other Devices
controller Controller1 = controller(primary);
//controller Controller2 = controller(partner);
pneumatics wall = pneumatics(Brain.ThreeWirePort.C);

//Motor Devices
motor LeftFront = motor(PORT5, ratio6_1, true);
motor LeftRear = motor(PORT1, ratio6_1, true);
motor RightFront = motor(PORT4, ratio6_1, false);
motor RightRear = motor(PORT3, ratio6_1, false);
motor LeftCata = motor(PORT15, ratio36_1, true);
motor RightCata = motor(PORT14, ratio36_1, false);
motor IntakeVacuum = motor(PORT11, ratio18_1, false);
motor HangingArm = motor(PORT13,ratio36_1,false);



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
PORT17,

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
PORT9,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
0,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
PORT10,

//Sideways tracker diameter (reverse to make the direction switch):
2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
0

);

int current_auton_selection = 5;
bool auto_started = true;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!  
  vexcodeInit();
  default_constants();


/*  while(auto_started == false){            //Changing the names below will only change their names on the
    Brain.Screen.clearScreen();            //brain screen for auton selection.
    switch(current_auton_selection){       //Tap the brain screen to cycle through autons.
      case 0:
        Brain.Screen.printAt(50, 50, "Drive Test");
        break;
      case 1:
        Brain.Screen.printAt(50, 50, "Drive Test");
        break;
      case 2:
        Brain.Screen.printAt(50, 50, "Turn Test");
        break;
      case 3:
        Brain.Screen.printAt(50, 50, "Swing Test");
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
  } */
}

void autonomous(void) {

  /*auto_started = true;
  switch(current_auton_selection){  
    case 0:
      odom_test();
      //drive_test(); //This is the default auton, if you don't select from the brain.
      break;        //Change these to be your own auton functions in order to use the auton selector.
    case 1:         //Tap the screen to cycle through autons.
      drive_test();
      break;
    case 2:
      turn_test();
      break;
    case 3:
      swing_test();
      break;
    case 4:
      full_test();
      break;
    case 5:
      odom_test();
      break;
    case 6:
      tank_odom_test();
      break;
    case 7:
      holonomic_odom_test();
      break;
 }*/
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

void jeffControl(){
  wait(2,seconds);
  chassis.set_coordinates(0, 0, 0);
  IntakeVacuum.setBrake(hold);
  //chassis.drive_to_point(0,6);
  //chassis.turn_to_angle(90);
  //chassis.drive_to_point(75,8);
  //chassis.turn_to_point(75,30);
  //chassis.drive_to_point(75,30);
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    //Replace this line with chassis.control_tank(); for tank drive 
    //or chassis.control_holonomic(); for holo drive.
    chassis.control_arcade();
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(0,50, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(0,70, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(0,90, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(0,110, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(0,130, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    

    if(Controller1.ButtonL1.pressing()) {
      IntakeVacuum.setVelocity(100,pct);
    }
    else if(Controller1.ButtonL2.pressing()) {
      IntakeVacuum.setVelocity(-100,pct);
    }
    else
    {
      IntakeVacuum.setVelocity(0,pct);
      IntakeVacuum.stop(brake);
    }

    if(Controller1.ButtonR1.pressing()) {
      HangingArm.setVelocity(100,pct);
    }
    else if(Controller1.ButtonR2.pressing()) {
      HangingArm.setVelocity(-100,pct);
    }
    else
    {
      HangingArm.setVelocity(0,pct);
      HangingArm.stop(hold);
    }
    

   if(Controller1.ButtonA.pressing()) {
      RightCata.setVelocity(100,pct);
      LeftCata.setVelocity(100,pct);
    }
    else if(Controller1.ButtonB.pressing()) {
      RightCata.setVelocity(0,pct);
      LeftCata.setVelocity(0,pct);
      LeftCata.stop(coast);
      RightCata.stop(coast);
    }
    else
    {
      LeftCata.stop(coast);
      RightCata.stop(coast);
    }

    Controller1.ButtonX.pressed(openWall);
    Controller1.ButtonY.pressed(closeWall);
    
    if (Controller1.ButtonX.pressing()) {
      wall.set(true);
    }
    if (Controller1.ButtonY.pressing()) {
      wall.set(false);
    }
    

    IntakeVacuum.spin(forward);
    HangingArm.spin(forward);
    RightCata.spin(reverse);
    LeftCata.spin(forward);
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

void pneumaticsSwitch() {
  if (wall.value() == true) {
    wall.close();
  } else {
    wall.open();
  }
}

void travelMode() {

}

void standardControl(){
  //Start Controller1 Scheme
  //Two Stick Arcade Mode
 /*LeftFront.spin(forward, ((Controller1.Axis3.position()) + Controller1.Axis1.position()), percent);
  LeftRear.spin(forward, ((Controller1.Axis3.position()) - Controller1.Axis1.position()), percent);
  RightFront.spin(forward, ((Controller1.Axis3.position()) + Controller1.Axis1.position()), percent);
  RightRear.spin(forward, ((Controller1.Axis3.position()) - Controller1.Axis1.position()), percent);*/
  chassis.control_arcade();

  //Run intake inwards/outwards bound r1,r2
  if (Controller1.ButtonL1.pressing() == true){
    IntakeVacuum.spin(forward, 100, percent);
  } else if (Controller1.ButtonR1.pressing() == true){
    IntakeVacuum.spin(reverse, 100, percent);
  } else {
    IntakeVacuum.stop(hold);
  }

  //Run catapault
  if (Controller1.ButtonR2.pressing() == true){
    LeftCata.spin(forward, 100, percent);
    RightCata.spin(forward, 100, percent);
  } else {
    LeftCata.stop(coast);
    RightCata.stop(coast);
  }

  //@TODO: Create Travel Mode Toggle

  //End Controller1 Scheme

  //Start Controller2 Scheme
  Controller1.ButtonB.pressed(pneumaticsSwitch);
  HangingArm.spin(forward, Controller1.Axis1.position(), percent);
  //End Controller2 Scheme
}

void usercontrol(void) {
  standardControl();
}

//
// Main will set up the competition functions and callbacks.
//
int main() {

  // Set up callbacks for autonomous and driver control periods.
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  //chassis.turn_to_angle(20);

  // Prevent main from exiting with an infinite loop.
   chassis.set_coordinates(0, 0, 0);
  while(true){
    jeffControl();
  }
}
