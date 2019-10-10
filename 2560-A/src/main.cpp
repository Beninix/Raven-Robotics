/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Thomas                                                    */
/*    Created:      Mon Sep 30 2019                                           */
/*    Description:  2560-A                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

vex::controller Controller;

//motor declaration
vex::motor lDriveFront(PORT1);
vex::motor lDriveBack(PORT11);
vex::motor rDriveFront(PORT10);
vex::motor rDriveBack(PORT20);

vex::motor lIntake(PORT2);
vex::motor rIntake(PORT9);

int const MAX_POWER = 180;

int lStickY;
int lStickX;
int rStickY;
int rStickX;
int lStickQuad;

int findQuadrant()
{
  /*quadrant mapping
    2|1
    3|4
    1: +x +y
    2: -x +y
    3: -x -y
    4: +x -y
  */
  if (lStickX > 0) //positive x
  {
    if (lStickY > 0) //positive y
    {
      return 1; //quadrant 1
    }
    else return 4; //negative y quadrant 4
  }
  else //negative x
  {
    if (lStickY > 0) //positve y
    {
      return 2; //quadrant 2
    }
    else return 3; //negative y quadrant 3
  }
}

void spinIntake(bool dir) 
{
  //spin motors inward if A pressed
  if (dir) 
  {
    lIntake.setVelocity(MAX_POWER, percentUnits::pct);
    rIntake.setVelocity(-MAX_POWER, percentUnits::pct);
  }
  else //spin outward if B pressed
  {
    lIntake.setVelocity(-MAX_POWER, percentUnits::pct);
    rIntake.setVelocity(MAX_POWER, percentUnits::pct);
  }
}
void strafe(int dir) 
{
  //strafe at power of right stick X
  if (dir > 0) //strafe right
  {
    lDriveFront.setVelocity(-dir, percentUnits::pct);
    lDriveBack.setVelocity(dir, percentUnits::pct);
    rDriveFront.setVelocity(dir, percentUnits::pct);
    rDriveBack.setVelocity(-dir, percentUnits::pct);
  }
  else //strafe left
  {
    lDriveFront.setVelocity(dir, percentUnits::pct);
    lDriveBack.setVelocity(-dir, percentUnits::pct);
    rDriveFront.setVelocity(-dir, percentUnits::pct);
    rDriveBack.setVelocity(dir, percentUnits::pct);
  }
}
void applyDriveVelocity()
{
  lDriveFront.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
  lDriveBack.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
  rDriveFront.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
  rDriveBack.setVelocity(calcVec(lStickX, lStickY), percentUnits::pct);
}

void spinMotors() 
{
  lDriveFront.spin(directionType::fwd);
  lDriveBack.spin(directionType::fwd);
  rDriveFront.spin(directionType::fwd);
  rDriveBack.spin(directionType::fwd);
}

double calcVec(int a, int b) 
{
  /*TODO
    create strafe steering using vectors
  */ 
  int quad = findQuadrant();
  int vector;

  //check if left stick has X value
  if (lStickX == 0)
  {
    return MAX_POWER;
  }
  switch (quad)
  {
  case 1:
      vector = sqrt(pow(a, 2) + pow(b, 2));
    break;
  case 2:
    vector = sqrt(pow(a, 2) + pow(b, 2));
    break;
  case 3:
    vector = -1 * sqrt(pow(a, 2) + pow(b, 2));
    break;
  case 4:
    vector = -1 * sqrt(pow(a, 2) + pow(b, 2));
    break;
  //handle unexpected values 
  default:
    break;
  }
  //return vector for motor power
  return sqrt(pow(a, 2) + pow(b, 2));
}

int main() 
{
    while(true) 
    {
        //get left stick axis values
        lStickY = Controller.Axis4.value();
        lStickX = Controller.Axis3.value();

        rStickY = Controller.Axis2.value();
        rStickX = Controller.Axis1.value();

        if (rStickX < 20 && rStickX > -20)
        {
          applyDriveVelocity();
        }
        else
        {
          strafe(rStickX);
        }
        
        if (Controller.ButtonA.pressing())
        {
          //turn on intake motors
          spinIntake(true);
        }
        else if (Controller.ButtonB.pressing())
        {
          //reverse intake motors
          spinIntake(false);
        }
        
        spinMotors();
    }
  return 0;
}