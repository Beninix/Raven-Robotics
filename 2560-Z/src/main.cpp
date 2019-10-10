/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Brandon                                          */
/*    Created:      Sun Oct 06 2019                                           */
/*    Description:  V5 2560Z Robot                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*
Changelog:
v1.0
-Added all part declarations
-Added driving
v1.1
-Added claw and arm control
v1.2
-Troubleshooted motors
v1.3
-Change the drive system
v1.4
-Added function for getting the angle using controller inputs
-Started working on the arm
*/

#include "vex.h"
#include <math.h>

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;

// define your global instances of motors and other devices here

vex::controller controller1;
vex::motor rightClawMotor (vex::PORT19, vex::gearSetting::ratio18_1, true);
vex::motor leftClawMotor (vex::PORT20, vex::gearSetting::ratio18_1, true);
vex::motor rightShoulderMotor (vex::PORT6, vex::gearSetting::ratio18_1, true);
vex::motor leftShoulderMotor (vex::PORT5, vex::gearSetting::ratio18_1, false);
vex::motor rightFrontDriveMotor (vex::PORT3, vex::gearSetting::ratio18_1, true);
vex::motor leftFrontDriveMotor (vex::PORT4, vex::gearSetting::ratio18_1, false);
vex::motor rightBackDriveMotor (vex::PORT1, vex::gearSetting::ratio18_1, true);
vex::motor leftBackDriveMotor (vex::PORT2, vex::gearSetting::ratio18_1, false);

double armAngle(double x, double y) {
  float rawAngle;
  if (x >= 0) {
    rawAngle = atan(y/x)/M_PI*180;
  }
  else {
    if (rawAngle >= 400) {
      rawAngle = -90;
    }
    else{
      rawAngle = 90;
    }
  }
  
  if (rawAngle == 0) {
    return 90;
  }
  else {
    return 90+rawAngle;
  }
}

int main() {
  /*
  rightShoulderMotor.startRotateTo(400, vex::rotationUnits::deg);
  leftShoulderMotor.startRotateTo(400, vex::rotationUnits::deg);
  vex::task::sleep(5000);
  rightShoulderMotor.startRotateTo(200, vex::rotationUnits::deg);
  leftShoulderMotor.startRotateTo(200, vex::rotationUnits::deg);
  */
  int startTimer = Brain.timer(timeUnits::msec);
  int armRange = 800;
  double shoulderPower = 50;
  rightShoulderMotor.setVelocity(shoulderPower, velocityUnits::pct);
  leftShoulderMotor.setVelocity(shoulderPower, velocityUnits::pct);
  double clawPower = 25;
  rightClawMotor.setVelocity(clawPower, velocityUnits::pct);
  leftClawMotor.setVelocity(clawPower, velocityUnits::pct);
  rightShoulderMotor.setStopping(vex::hold);
  leftShoulderMotor.setStopping(vex::hold);
  rightClawMotor.setStopping(vex::hold);
  leftClawMotor.setStopping(vex::hold);
  while(true){
    controller1.Screen.print((armAngle(controller1.Axis1.value(),controller1.Axis2.value())/180*armRange));
    //controller1.Screen.print(armAngle(controller1.Axis1.value(),controller1.Axis2.value()));
    //controller1.Screen.print(controller1.Axis2.value());
    Brain.timer(timeUnits::msec);
    //Drive
    if (abs(controller1.Axis3.value()) > 10) {
      double leftJoyY = controller1.Axis3.value();
      rightFrontDriveMotor.setVelocity(leftJoyY, velocityUnits::pct);
      rightBackDriveMotor.setVelocity(leftJoyY, velocityUnits::pct);
      leftFrontDriveMotor.setVelocity(leftJoyY, velocityUnits::pct);
      leftBackDriveMotor.setVelocity(leftJoyY, velocityUnits::pct);
      rightFrontDriveMotor.spin(directionType::fwd);
      rightBackDriveMotor.spin(directionType::fwd);
      leftFrontDriveMotor.spin(directionType::fwd);
      leftBackDriveMotor.spin(directionType::fwd);
    } //End of if
    else if (abs(controller1.Axis4.value()) > 10) {
      double leftJoyX = controller1.Axis4.value();
      rightFrontDriveMotor.setVelocity(leftJoyX, velocityUnits::pct);
      rightBackDriveMotor.setVelocity(leftJoyX, velocityUnits::pct);
      leftFrontDriveMotor.setVelocity(leftJoyX, velocityUnits::pct);
      leftBackDriveMotor.setVelocity(leftJoyX, velocityUnits::pct);
      rightFrontDriveMotor.spin(directionType::rev);
      rightBackDriveMotor.spin(directionType::rev);
      leftFrontDriveMotor.spin(directionType::fwd);
      leftBackDriveMotor.spin(directionType::fwd);
    } //End of if
    else {
      rightFrontDriveMotor.stop();
      rightBackDriveMotor.stop();
      leftFrontDriveMotor.stop();
      leftBackDriveMotor.stop();
    } //End of else
    
    //Arm
    //All the way down is 0 deg, all the way up is 180 deg
    if (controller1.Axis1.value() > 10 && Brain.timer(timeUnits::msec) > startTimer + 500){
    //if(controller1.ButtonX.pressing() && Brain.timer(timeUnits::msec) > startTimer + 2000){
      rightShoulderMotor.startRotateTo(armAngle(controller1.Axis1.value(),controller1.Axis2.value())/180*armRange, vex::rotationUnits::deg);
      leftShoulderMotor.startRotateTo(armAngle(controller1.Axis1.value(),controller1.Axis2.value())/180*armRange, vex::rotationUnits::deg);
      startTimer = Brain.timer(timeUnits::msec);
    } //End of if
    /*
    else{
      rightShoulderMotor.stop();
      leftShoulderMotor.stop();
    } //End of else
    */
    //Kill Arm (for testing)
    if (controller1.ButtonA.pressing()){
      rightShoulderMotor.stop();
      leftShoulderMotor.stop();
    }
    if (controller1.ButtonB.pressing()){
      rightShoulderMotor.startRotateTo(200, vex::rotationUnits::deg);
      leftShoulderMotor.startRotateTo(200, vex::rotationUnits::deg);
    }

    //Claw
    if (controller1.ButtonL1.pressing()) {
      rightClawMotor.spin(directionType::fwd);
      leftClawMotor.spin(directionType::rev);
    } //End of if
    else if (controller1.ButtonR1.pressing()) {
      rightClawMotor.spin(directionType::rev);
      leftClawMotor.spin(directionType::fwd);
    } //End of else if
    else {
      rightClawMotor.stop();
      leftClawMotor.stop();
    } //End of else

  } //End of while
    
} //End of main