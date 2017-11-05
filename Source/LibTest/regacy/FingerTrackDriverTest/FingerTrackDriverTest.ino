
#include "FingerTrackDriver.h"

FingerTrackDriver ftd;

DeclareTaskLoop(LeftTask);
DeclareTaskLoop(RightTask);
DeclareTaskLoop(UpTask);
DeclareTaskLoop(DownTask);

SemaphoreHandle leftSemaphore;
SemaphoreHandle rightSemaphore;
SemaphoreHandle upSemaphore;
SemaphoreHandle downSemaphore;

//SemaphoreHandle serialSemaphore;




void setup() {

  ftd.Begin(2, 3, 4, 5, 6);
  Serial.begin(9600);

  CreateBinarySemaphore(leftSemaphore);
  CreateBinarySemaphore(rightSemaphore);
  CreateBinarySemaphore(upSemaphore);
  CreateBinarySemaphore(downSemaphore);

  //CreateBinarySemaphore(serialSemaphore);

  CreateTaskLoop(LeftTask, LOW_PRIORITY);
  CreateTaskLoop(RightTask, LOW_PRIORITY);
  CreateTaskLoop(UpTask, LOW_PRIORITY);
  CreateTaskLoop(DownTask, LOW_PRIORITY);

  Serial.print("a");
}

void loop() {

  Serial.println("loop");
/*
  if (Acquire(leftSemaphore, 1000)) {
    ftd.AddLeftToDeltaX();
    Release(leftSemaphore);
  }

  if (Acquire(rightSemaphore, 1000)) {
    ftd.AddRightToDeltaX();
    Release(rightSemaphore);
  }

  if (Acquire(upSemaphore, 1000)) {
    ftd.AddUpToDeltaY();
    Release(upSemaphore);
  }

  if (Acquire(downSemaphore, 1000)) {
    ftd.AddDownToDeltaY();
    Release(downSemaphore);
  }

  if (Acquire(serialSemaphore, 1000)) {
    Serial.print("deltaX:");
    Serial.print(ftd.GetDeltaX());
    Serial.print("  deltaY:");
    Serial.println(ftd.GetDeltaY());
    ftd.ResetDeltaXY();
    Release(serialSemaphore);
  }
  */
}





//LeftTask-------------------------------------------------------------

TaskLoop(LeftTask) {
  ftd.ReadLeft();
  if (Acquire(leftSemaphore, 1000)) {
    ftd.AddLeftToSum();
    Release(leftSemaphore);

    /*
      if (Acquire(serialSemaphore, 1000)) {
      Serial.println("left");
      Release(serialSemaphore);
      }
    */
  }
}

//-----------------------------------------------------------------------

//rightTask--------------------------------------------------------------

TaskLoop(RightTask) {
  ftd.ReadRight();

  if (Acquire(rightSemaphore, 1000)) {
    ftd.AddRightToSum();
    Release(rightSemaphore);

    /*
      if (Acquire(serialSemaphore, 1000)) {
      Serial.println("right");
      Release(serialSemaphore);
      }

    */

  }
}

//--------------------------------------------------------------------------

//UpTask--------------------------------------------------------------------

TaskLoop(UpTask) {
  ftd.ReadUp();

  if (Acquire(upSemaphore, 1000)) {
    ftd.AddUpToSum();
    Release(upSemaphore);

    /*
      if (Acquire(serialSemaphore, 1000)) {
      Serial.println("up");
      Release(serialSemaphore);
      }
    */

  }
}

//----------------------------------------------------------------------------

//DownTask---------------------------------------------------------------------
/*
TaskLoop(DownTask) {
  ftd.ReadDown();

  if (Acquire(downSemaphore, 1000)) {
    ftd.AddDownToSum();
    Release(downSemaphore);

    
      if (Acquire(serialSemaphore, 1000)) {
      Serial.println("down");
      Release(serialSemaphore);
      //Yield();
      }
    

  }

}
*/

//-------------------------------------------------------------------------------

