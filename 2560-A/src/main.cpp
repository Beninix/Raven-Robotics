/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Administrator                                    */
/*    Created:      Mon Sep 30 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include <math.h>

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;
 
vex::controller Controller;

// define your global instances of motors and other devices here

vex::motor lDriveFront(vex::PORT1,false);
vex::motor lDriveBack(vex::PORT10,false);
vex::motor rDriveFront(vex::PORT11,true);
vex::motor rDriveBack(vex::PORT20,false);

int lStickY;
int lStickX;
int rStickY;
int rStickX;

bool flipX,flipY;

double calcVec(int a, int b) {

  if (lStickX != 0) {
    if (lStickX < 0) {
      lStickX*=-1;
      flipX = true;
    }
    if(lStickY < 0){
      lStickY*=-1;
      flipY = true;
    }
    calcVec(lStickX, lStickY);
  }

  int vector = sqrt(pow(a, 2) + pow(b, 2));

  return vector;
}
int count = 0;
int main() {
    while(1) {
      
        Brain.Screen.printAt( 10, 50, "Hello V5 %d", count++ );
        lStickY = Controller.Axis4.value();
        lStickX = Controller.Axis3.value();
        lDriveFront.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
        lDriveBack.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
        rDriveFront.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
        rDriveBack.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
        lDriveFront.spin(directionType::fwd);
        lDriveBack.spin(directionType::fwd);
        rDriveFront.spin(directionType::fwd);
        rDriveBack.spin(directionType::fwd);
    }
  return 0;
}

