
#include "FingerTrackDriver.h"

FingerTrackDriver ftd;

DeclareTaskLoop(LeftTask);
DeclareTaskLoop(RightTask);
DeclareTaskLoop(UpTask);
DeclareTaskLoop(DownTask);

void setup() {
  ftd.pinBTN = 2;
  ftd.pinLFT = 3;
  ftd.pinRHT = 4;
  ftd.pinUP = 5;
  ftd.pinDWN = 6;

  ftd.Begin();
  Serial.begin(19200);
  InitMainLoopStackSize(100);
  
  CreateTaskLoop(LeftTask, LOW_PRIORITY);
  CreateTaskLoop(RightTask, LOW_PRIORITY);
  CreateTaskLoop(UpTask, LOW_PRIORITY);
  CreateTaskLoop(DownTask, LOW_PRIORITY);
}

void loop() {
  Serial.print("DeltaX:");
  Serial.print(ftd.GetDeltaX());
  Serial.print("  DeltaY:");
  Serial.println(ftd.GetDeltaY());
  if(ftd.GetDeltaX() > 32){
    ftd.ResetDeltaX();
  }
  if(ftd.GetDeltaY() > 32){
    ftd.ResetDeltaY();
  }
}

TaskLoop(LeftTask){
  ftd.ReadLeft();
}

TaskLoop(RightTask){
  ftd.ReadRight();
}

TaskLoop(UpTask){
  ftd.ReadUp();
}

TaskLoop(DownTask){
  ftd.ReadDown();
}

