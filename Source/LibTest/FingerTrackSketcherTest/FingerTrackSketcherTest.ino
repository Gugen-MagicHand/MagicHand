/*
#include "FingerTrackSketcher.h"
#include "CanvasPrint.h"

FingerTrackSketcher ftSketcher;

Canvas toCanvas(8,8);
Canvas fromCanvas(8,8);

int deltaX;
int deltaY;

void setup() {
  ftSketcher.sketchCanvas.SetSize(8,8);
  deltaX = 5;
  deltaY = 1;
  Serial.begin(9600);
}

void loop() {
  //スケッチャーのキャンバスをコピー先にコピーする
  ftSketcher.SetDeltaXY(deltaX, deltaY);
  ftSketcher.Sketch();
  
  ftSketcher.CopyCanvas(&toCanvas);

  SerialPrintCanvas(toCanvas);

  delay(1000);

  ftSketcher.ClearCanvas();

  
  //コピー先からスケッチャーのキャンバスにコピーする
  
  

  

  
}

*/

#include "SketcherCanvas.h"
#include "CanvasPrint.h"

SketcherCanvas canvas(32,32);

void setup(){
  Serial.begin(9600);
  canvas.Boxf(5,13,19,21);
  SerialPrintCanvas(canvas);

  canvas.SeekUpperLeftCorner();
  canvas.SeekLowerRightCorner();
  Serial.print("UpperLeftX: ");
  Serial.println(canvas.GetUpperLeftX());
  Serial.print("UpperLeftY: ");
  Serial.println(canvas.GetUpperLeftY());
  Serial.print("LowerRightX: ");
  Serial.println(canvas.GetLowerRightX());
  Serial.print("LowerRightY: ");
  Serial.println(canvas.GetLowerRightY());
}

void loop(){
  
}



